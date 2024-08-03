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

int	check_built_in(t_token *token, t_dict *m_env)
{
	if (!token || !token->data)
		return (-1);
	if (!ft_strcmp(token->data, "pwd"))
		print_pwd();
	else if (!ft_strcmp(token->data, "env"))
	{
		if (token->next && !(token->next->flags == PIPE || token->next->flags == REDIRECT))
			return (ft_printf(2, ENV_ERROR, token->next->data), 1);
		print_env(m_env);
	}
	else if (!ft_strcmp(token->data, "export"))
	{
		if (!token->next || token->next->flags == PIPE || token->next->flags == REDIRECT)
			return (ft_export(token->data, m_env, 1));
		token = token->next;
		while (token && !token->flags)
		{
			ft_export(token->data, m_env, 0);
			token = token->next;
		}
	}
	else if (!ft_strcmp(token->data, "unset"))
	{
		if (!token->next || token->next->flags == PIPE || token->next->flags == REDIRECT)
			return (ft_unset(token->data, m_env));
		token = token->next;
		while (token && !token->flags)
		{
			ft_unset(token->data, m_env);
			token = token->next;
		}
	}
	else if (!ft_strcmp(token->data, "cd"))
	{
		if (!token->next || token->next->flags == PIPE || token->next->flags == REDIRECT)
			return (ft_cd(NULL, m_env));
		token = token->next;
		if (token->next && !(token->next->flags == PIPE || token->next->flags == REDIRECT))
			return (ft_printf(2, CD_ERROR_ARGS), 1);
		if (token->data)
			ft_cd(token->data, m_env);
	}
	return (0);
}
