/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 20:12:00 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/06 17:38:38 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_built_in(t_token *token, t_process *process)
{
	process->stat = 0;
	if (!token || !token->data)
		return (99);
	if (!ft_strcmp(token->data, "pwd"))
	{
		process->stat = print_pwd();
		return (0);
	}
	else if (!ft_strcmp(token->data, "env"))
	{
		if (token->next && !(token->next->flags == PIPE))
		{
			process->stat = 1;
			return (ft_printf(2, ENV_ERROR, token->next->data), 1);
		}
		print_env(process->m_env);
		return (0);
	}
	else if (!ft_strcmp(token->data, "export"))
	{
		if (!token->next || token->next->flags == PIPE)
		{
			return (process->stat = ft_export(token->data, process->m_env, 1));
		}
		token = token->next;
		while (token && !token->flags)
		{
			process->stat = ft_export(token->data, process->m_env, 0);
			token = token->next;
		}
		return (0);
	}
	else if (!ft_strcmp(token->data, "unset"))
	{
		if (!token->next || token->next->flags == PIPE)
			return (process->stat = ft_unset(token->data, process->m_env));
		token = token->next;
		while (token && !token->flags)
		{
			process->stat = ft_unset(token->data, process->m_env);
			token = token->next;
		}
		return (0);
	}
	else if (!ft_strcmp(token->data, "cd"))
	{
		if (!token->next || token->next->flags == PIPE)
			return (process->stat = ft_cd(NULL, process->m_env));
		token = token->next;
		if (token->next && !(token->next->flags == PIPE))
		{
			process->stat = 1;
			return (ft_printf(2, CD_ERROR_ARGS), 1);
		}
		if (token->data)
			process->stat = ft_cd(token->data, process->m_env);
		return (0);
		
	}
	return (99);
}
