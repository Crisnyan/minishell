/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:59:13 by vpf               #+#    #+#             */
/*   Updated: 2024/09/14 13:43:41 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_prompt(t_process *process, char *delim, int fd)
{
	char	*line;
	int		og_stdin;

	og_stdin = dup(process->og_fd[0]);
	if (!isatty(STDIN_FILENO))
	{
		dup2(og_stdin, STDIN_FILENO);
		close(og_stdin);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			return ;
		if (!ft_strcmp(line, delim))
		{
			free(line);
			return ;
		}
		line = ft_strappend(&line, "\n");
		ft_printf(fd, line);
		free(line);
	}
}

int	open_heredoc(t_process *process, t_token *temp)
{
	int		fd;
	char	*filename;

	fd = 0;
	filename = NULL;
	filename = ft_itoa(process->heredoc_count + 1);
	filename = ft_strattach("/tmp/here-doc", &filename);
	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		if (access(filename, F_OK))
			process->m_env->err_code = (exec_err(ERR_NOFILE, filename));
		else if (access(filename, R_OK))
			process->m_env->err_code = (exec_err(ERR_PERM, filename));
		else
			process->m_env->err_code = (exec_err(ERR_STD, NULL));
		free(filename);
		return (1);
	}
	heredoc_prompt(process, temp->next->data, fd);
	close(fd);
	free(filename);
	process->heredoc_count++;
	return (0);
}

void	create_heredocs(t_process *process, t_token *cmd_list)
{
	t_token	*temp;

	temp = cmd_list;
	signal(SIGINT, handle_c_heredoc);
	signal(SIGQUIT, SIG_IGN);
	while (temp && !g_global_signal)
	{
		if (temp->flags == HEREDOC)
		{
			if (open_heredoc(process, temp))
				return ;
		}
		temp = temp->next;
	}
	if (g_global_signal)
		process->m_env->err_code = g_global_signal;
}
