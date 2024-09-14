/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 14:16:50 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/14 19:26:22 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_no_pipes(t_process *process)
{
	t_cmd	cmd;
	pid_t	child;
	t_token	*cmd_tokens;

	ft_bzero(&cmd, sizeof(t_cmd));
	create_heredocs(process, process->cmd_list);
	cmd_tokens = ft_redirect(process, &cmd, process->cmd_list[0]);
	process->cmd_list[0] = cmd_tokens;
	if (process->m_env->err_code)
		return (process->m_env->err_code);
	if (check_built_in(cmd_tokens, process) == 99)
	{
		child = fork();
		if (child < 0)
		{
			process->m_env->err_code = exec_err(ERR_MEM, " fork:");
			return (process->m_env->err_code);
		}
		else if (child == 0)
			child_exec_no_pipes(process, &cmd, cmd_tokens);
		process->m_env->err_code = wait_child_processes(&child, 1);
	}
	return (process->m_env->err_code);
}

int	exec_pipes(t_process *prcs)
{
	int		i;
	t_cmd	cmd;
	pid_t	*child;

	i = -1;
	init_exec_pipes(&cmd, &child, prcs);
	create_heredocs(prcs, prcs->cmd_list);
	if (prcs->m_env->err_code)
	{
		free(child);
		return (1);
	}
	while (++i < (prcs->n_pipes + 1))
	{
		if (pipe_and_heredocs(prcs, child, i))
			return (prcs->m_env->err_code);
		child[i] = fork();
		if (child[i] < 0)
		{
			prcs->m_env->err_code = exec_err(ERR_MEM, " fork:");
			break ;
		}
		else if (child[i] == 0)
			child_exec_pipes(prcs, &cmd, i);
		if (dup2(prcs->pipe[0], STDIN_FILENO) < 0)
			exit(exec_err(ERR_STD, NULL));
		close_pipes(prcs->pipe);
	}
	prcs->m_env->err_code = wait_child_processes(child, prcs->n_pipes + 1);
	free(child);
	return (prcs->m_env->err_code);
}

int	ft_executor(t_process *process)
{
	if (!process->cmd_list)
		return (-1);
	process->m_env->err_code = 0;
	g_global_signal = 0;
	if (!process->n_pipes)
		exec_no_pipes(process);
	else
		exec_pipes(process);
	clean_here_docs(process);
	dup2(process->og_fd[0], STDIN_FILENO);
	dup2(process->og_fd[1], STDOUT_FILENO);
	return (process->m_env->err_code);
}
