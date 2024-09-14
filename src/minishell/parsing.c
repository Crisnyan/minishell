/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:49:22 by cristian          #+#    #+#             */
/*   Updated: 2024/09/14 14:20:10 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_global_signal;

int	parse(t_token *token, t_process *process)
{
	int		heredoc_count;
	t_token	*temp;

	heredoc_count = 0;
	if (!token)
		return (1);
	temp = token;
	if (temp->flags == PIPE)
	{
		ft_printf(STDERR_FILENO,
			"minishell: syntax error near unexpected token '%s'\n", temp->data);
		process->m_env->err_code = 2;
		return (2);
	}
	while (temp)
	{
		if ((temp->flags == I_REDIRECT || temp->flags == O_REDIRECT
				|| temp->flags == HEREDOC || temp->flags == O_APPEND)
			&& (!temp->next || temp->next->flags == PIPE
				|| temp->next->flags == I_REDIRECT
				|| temp->next->flags == O_REDIRECT
				|| temp->next->flags == HEREDOC
				|| temp->next->flags == O_APPEND))
		{
			ft_printf(STDERR_FILENO,
				"minishell: syntax error near unexpected token '%s'\n",
				temp->data);
			process->m_env->err_code = 2;
			return (2);
		}
		else if (temp->flags == PIPE && (!temp->next
				|| temp->next->flags == PIPE))
		{
			ft_printf(STDERR_FILENO,
				"minishell: syntax error near unexpected token '%s'\n",
				temp->data);
			process->m_env->err_code = 2;
			return (2);
		}
		if (temp->flags == HEREDOC)
			heredoc_count++;
		temp = temp->next;
	}
	if (heredoc_count > 15)
	{
		ft_printf(STDERR_FILENO,
			"minishell: maximum here-document count exceeded\n");
		process->m_env->err_code = 2;
		exit(2);
	}
	return (0);
}

int	count_quotes(char *line)
{
	int	i;
	int	sq;
	int	dq;

	i = -1;
	sq = 0;
	dq = 0;
	while (line[++i])
	{
		if (line[i] == '\'' && !dq)
		{
			if (!sq)
				sq = 1;
			else
				sq = 0;
		}
		else if (line[i] == '\"' && !sq)
		{
			if (!dq)
				dq = 1;
			else
				dq = 0;
		}
	}
	return (sq + dq);
}

void	sig_and_line(char **str, t_dict m_env)
{
	signal(SIGINT, handle_c);
	signal(SIGQUIT, SIG_IGN);
	str[PROMPT] = format_prompt(&m_env);
	str[LINE] = readline(str[PROMPT]);
}

t_token	*null_var(char **str)
{
	str[PROMPT] = NULL;
	str[LINE] = NULL;
	g_global_signal = 0;
	return (NULL);
}
