/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:10:25 by vpf               #+#    #+#             */
/*   Updated: 2024/09/15 20:20:20 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

void	child_exec_no_pipes(t_process *process, t_cmd *cmd, t_token *cmd_tokens)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (format_cmd(cmd, process->m_env, cmd_tokens, &process->m_env->err_code))
	{
		free_cmd(cmd);
		exit (process->m_env->err_code);
	}
	execve(cmd->path, cmd->args, cmd->envp);
	free_cmd(cmd);
	process->m_env->err_code = exec_err(ERR_STD, NULL);
	exit (process->m_env->err_code);
}

void	child_exec_pipes(t_process *process, t_cmd *cmd, int i)
{
	t_token	*cmd_tokens;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	if (i != process->n_pipes)
	{
		if (dup2(process->pipe[1], STDOUT_FILENO) < 0)
			exit(exec_err(ERR_STD, NULL));
	}
	close_pipes(process->pipe);
	cmd_tokens = ft_redirect(process, cmd, process->cmd_list[i]);
	if (process->m_env->err_code)
		exit(process->m_env->err_code);
	process->cmd_list[i] = cmd_tokens;
	if (check_built_in(cmd_tokens, process) != 99)
		exit(process->m_env->err_code);
	if (format_cmd(cmd, process->m_env, cmd_tokens, &process->m_env->err_code))
	{
		free_cmd(cmd);
		exit(process->m_env->err_code);
	}
	execve(cmd->path, cmd->args, cmd->envp);
	free_cmd(cmd);
	process->m_env->err_code = exec_err(ERR_STD, NULL);
	exit (process->m_env->err_code);
}

int	pipe_and_heredocs(t_process *process, pid_t *child, int i)
{
	if (process->m_env->err_code)
	{
		free(child);
		return (1);
	}
	if (pipe(process->pipe))
	{
		free(child);
		process->m_env->err_code = exec_err(ERR_STD, NULL);
		return (1);
	}
	count_prev_heredocs(process, i);
	return (0);
}

int	init_exec_pipes(t_cmd *cmd, pid_t **child, t_process *process)
{
	ft_bzero(cmd, sizeof(t_cmd));
	*child = (pid_t *)malloc((process->n_pipes + 1) * sizeof(pid_t));
	if (!child)
	{
		process->m_env->err_code = exec_err(ERR_MEM, " malloc:");
		return (process->m_env->err_code);
	}
	return (0);
}
