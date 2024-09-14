/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:03:38 by vpf               #+#    #+#             */
/*   Updated: 2024/09/14 20:45:24 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_redirection(t_process *process)
{
	char	*filename;
	int		fd;

	fd = 0;
	filename = ft_itoa(process->heredoc_count + 1);
	filename = ft_strattach("/tmp/here-doc", &filename);
	fd = open(filename, O_RDONLY);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		process->m_env->err_code = exec_err(ERR_STD, NULL);
		close(fd);
		free(filename);
		return ;
	}
	process->heredoc_count++;
	close(fd);
	free(filename);
}

void	input_redirection(t_process *process, t_cmd *cmd, t_token *target)
{
	cmd->in_file = open(target->data, O_RDONLY);
	if (cmd->in_file < 0)
	{
		if (access(target->data, F_OK))
			process->m_env->err_code = (exec_err(ERR_NOFILE, target->data));
		else if (access(target->data, R_OK))
			process->m_env->err_code = (exec_err(ERR_PERM, target->data));
		else
			process->m_env->err_code = (exec_err(ERR_STD, NULL));
		return ;
	}
	if (dup2(cmd->in_file, STDIN_FILENO) < 0)
	{
		process->m_env->err_code = exec_err(ERR_STD, NULL);
		close(cmd->in_file);
		return ;
	}
	close(cmd->in_file);
}

void	append_redirection(t_process *process, t_cmd *cmd, t_token *target)
{
	cmd->out_file = open(target->data, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (cmd->out_file < 0)
	{
		if (access(target->data, R_OK))
			process->m_env->err_code = (exec_err(ERR_PERM, target->data));
		else
			process->m_env->err_code = (exec_err(ERR_STD, NULL));
		return ;
	}
	if (dup2(cmd->out_file, STDOUT_FILENO) < 0)
	{
		process->m_env->err_code = exec_err(ERR_STD, NULL);
		close(cmd->out_file);
		return ;
	}
	close(cmd->out_file);
}

void	output_redirection(t_process *process, t_cmd *cmd, t_token *target)
{
	cmd->out_file = open(target->data, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->out_file < 0)
	{
		if (access(target->data, R_OK))
			process->m_env->err_code = (exec_err(ERR_PERM, target->data));
		else
			process->m_env->err_code = (exec_err(ERR_STD, NULL));
		return ;
	}
	if (dup2(cmd->out_file, STDOUT_FILENO) < 0)
	{
		process->m_env->err_code = exec_err(ERR_STD, NULL);
		close(cmd->out_file);
		return ;
	}
	close(cmd->out_file);
}

t_token	*ft_redirect(t_process *process, t_cmd *cmd, t_token *cmd_list)
{
	t_token	*redir_list[4];

	redir_list[REDIR_CMD] = cmd_list;
	redir_list[PREV_EXE] = NULL;
	while (cmd_list)
	{
		if (is_redir(cmd_list->flags))
		{
			redir_list[TEMP_EXE] = cmd_list;
			cmd_list = cmd_list->next;
			if (cmd_list)
			{
				if (check_redirs(process, cmd, redir_list[TEMP_EXE], cmd_list))
					return (redir_list[REDIR_CMD]);
				advance_redir(redir_list, &cmd_list);
			}
			check_previous(redir_list, &cmd_list);
			free_list(redir_list[TEMP_EXE]);
			continue ;
		}
		redir_list[PREV_EXE] = cmd_list;
		cmd_list = cmd_list->next;
	}
	return (redir_list[REDIR_CMD]);
}
