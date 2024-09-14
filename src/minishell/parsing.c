/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:49:22 by cristian          #+#    #+#             */
/*   Updated: 2024/09/14 20:34:56 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_global_signal;

int	check_conditions(t_process *process, t_token *temp)
{
	if ((is_redir(temp->flags)) && (!temp->next
		|| temp->next->flags == PIPE || is_redir(temp->next->flags)))
	{
		ft_printf(STDERR_FILENO, ERR_SYNTAX, temp->data);
		process->m_env->err_code = 2;
		return (2);
	}
	else if (temp->flags == PIPE && (!temp->next
		|| temp->next->flags == PIPE))
	{
		ft_printf(STDERR_FILENO, ERR_SYNTAX, temp->data);
		process->m_env->err_code = 2;
		return (2);
	}
	else
		return (0);
}

int	parse(t_token *token, t_process *process)
{
	t_token	*temp;

	if (!token)
		return (1);
	temp = token;
	if (temp->flags == PIPE)
	{
		ft_printf(STDERR_FILENO, ERR_SYNTAX, temp->data);
		process->m_env->err_code = 2;
		return (2);
	}
	while (temp)
	{
		if (check_conditions(process, temp))
			return (2);
		if (temp->flags == HEREDOC)
			process->current_heredoc++;
		temp = temp->next;
	}
	if (process->current_heredoc > 15)
	{
		ft_printf(STDERR_FILENO, ERR_HDMAX_MSG);
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
