/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:38:53 by cristian          #+#    #+#             */
/*   Updated: 2024/09/14 18:10:42 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_global_signal;

int	count_pipes(t_token *list)
{
	int		pipes;
	t_token	*temp;

	pipes = 0;
	temp = list;
	while (temp)
	{
		if (temp->flags == PIPE)
			pipes++;
		temp = temp->next;
	}
	return (pipes);
}

void	free_cmd_list(t_token **cmd_list, int n)
{
	int	i;

	i = 0;
	if (cmd_list)
	{
		while (i < (n + 1))
		{
			if (cmd_list[i])
				free_list(cmd_list[i]);
			i++;
		}
		free(cmd_list);
	}
}

static void	replace_token(t_token **tokens)
{
	t_token	*temp;

	temp = (*tokens)->next;
	(*tokens)->next = NULL;
	(*tokens) = temp->next;
	temp->next = NULL;
	free_list(temp);
}

t_token	**split_cmd(t_token *tokens, int pipes)
{
	int		i;
	t_token	**cmd_list;

	i = 0;
	cmd_list = (t_token **)calloc(pipes + 1, sizeof(t_token *));
	if (!cmd_list)
		return (NULL);
	while (i < (pipes + 1) && tokens)
	{
		cmd_list[i] = tokens;
		while (tokens)
		{
			if (tokens->next && tokens->next->flags == PIPE)
			{
				replace_token(&tokens);
				break ;
			}
			tokens = tokens->next;
		}
		i++;
	}
	return (cmd_list);
}

void	init_process(t_process *process, t_dict *m_env)
{
	process->heredoc_count = 0;
	process->current_heredoc = 0;
	process->m_env = m_env;
	process->cmd_list = NULL;
	process->og_fd[0] = dup(0);
	process->og_fd[1] = dup(1);
}
