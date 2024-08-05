/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 20:12:00 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/03 14:02:04 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_built_in(t_token *token, t_dict *m_env, int *stat)
{
	*stat = 0;
	if (!token || !token->data)
		return (99);
	if (!ft_strcmp(token->data, "pwd"))
	{
		*stat = print_pwd();
		return (0);
	}
	else if (!ft_strcmp(token->data, "env"))
	{
		if (token->next && !(token->next->flags == PIPE || token->next->flags == REDIRECT))
		{
			*stat = 1;
			return (ft_printf(2, ENV_ERROR, token->next->data), 1);
		}
		print_env(m_env);
		return (0);
	}
	else if (!ft_strcmp(token->data, "export"))
	{
		if (!token->next || token->next->flags == PIPE || token->next->flags == REDIRECT)
			return (*stat = ft_export(token->data, m_env, 1));
		token = token->next;
		while (token && !token->flags)
		{
			*stat = ft_export(token->data, m_env, 0);
			token = token->next;
		}
		return (0);
	}
	else if (!ft_strcmp(token->data, "unset"))
	{
		if (!token->next || token->next->flags == PIPE || token->next->flags == REDIRECT)
			return (*stat = ft_unset(token->data, m_env));
		token = token->next;
		while (token && !token->flags)
		{
			*stat = ft_unset(token->data, m_env);
			token = token->next;
		}
		return (0);
	}
	else if (!ft_strcmp(token->data, "cd"))
	{
		if (!token->next || token->next->flags == PIPE || token->next->flags == REDIRECT)
			return (*stat = ft_cd(NULL, m_env));
		token = token->next;
		if (token->next && !(token->next->flags == PIPE || token->next->flags == REDIRECT))
		{
			*stat = 1;
			return (ft_printf(2, CD_ERROR_ARGS), 1);
		}
		if (token->data)
			*stat = ft_cd(token->data, m_env);
		return (0);
		
	}
	return (99);
}
