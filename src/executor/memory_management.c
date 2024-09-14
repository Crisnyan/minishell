/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 14:16:50 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/14 14:26:08 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_pipes(int *pipefd)
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
			i++;
		}
		free(arr);
		arr = NULL;
	}
}

void	clean_here_docs(t_process *process)
{
	int		i;
	char	*filename;

	i = 0;
	process->heredoc_count = 0;
	while (i < 16)
	{
		filename = ft_itoa(i + 1);
		filename = ft_strattach("/tmp/here-doc", &filename);
		if (!access(filename, F_OK))
			unlink(filename);
		free(filename);
		i++;
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->path)
		{
			free(cmd->path);
			cmd->path = NULL;
		}
		if (cmd->name)
		{
			free(cmd->name);
			cmd->name = NULL;
		}
		free_arr(cmd->args);
		free_arr(cmd->envp);
	}
	cmd = NULL;
}
