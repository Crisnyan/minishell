/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 20:12:00 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 20:12:25 by vperez-f         ###   ########.fr       */
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
		print_env(m_env);
	else if (!ft_strcmp(token->data, "export"))
	{
		if (!token->next || token->next->flags)
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
		if (!token->next || token->next->flags)
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
		ft_cd(token->next->data);
	}
	return (0);
}
