/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 20:12:00 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/29 17:16:01 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_built_in(t_token *token, t_process *process)
{
	process->m_env->err_code = 0;
	if (!token || !token->data)
		return (99);
	if (!ft_strcmp(token->data, "pwd"))
	{
		process->m_env->err_code = print_pwd();
		return (0);
	}
	else if (!ft_strcmp(token->data, "env"))
	{
		if (token->next && !(token->next->flags == PIPE))
		{
			process->m_env->err_code = 1;
			return (ft_printf(2, ENV_ERROR, token->next->data), 1);
		}
		print_env(process->m_env);
		return (0);
	}
	else if (!ft_strcmp(token->data, "export"))
	{
		if (!token->next || token->next->flags == PIPE)
		{
			return (process->m_env->err_code = ft_export(token->data, process->m_env, 1));
		}
		token = token->next;
		while (token && !token->flags)
		{
			process->m_env->err_code = ft_export(token->data, process->m_env, 0);
			token = token->next;
		}
		return (0);
	}
	else if (!ft_strcmp(token->data, "unset"))
	{
		if (!token->next || token->next->flags == PIPE)
			return (process->m_env->err_code = ft_unset(token->data, process->m_env));
		token = token->next;
		while (token && !token->flags)
		{
			process->m_env->err_code = ft_unset(token->data, process->m_env);
			token = token->next;
		}
		return (0);
	}
	else if (!ft_strcmp(token->data, "cd"))
	{
		if (!token->next || token->next->flags == PIPE)
			return (process->m_env->err_code = ft_cd(NULL, process->m_env));
		token = token->next;
		if (token->next && !(token->next->flags == PIPE))
		{
			process->m_env->err_code = 1;
			return (ft_printf(2, CD_ERROR_ARGS), 1);
		}
		if (token->data)
			process->m_env->err_code = ft_cd(token->data, process->m_env);
		return (0);
		
	}
	else if (!ft_strcmp(token->data, "exit"))
	{
		return (ft_exit(token));
	}
	else if (!ft_strcmp(token->data, "echo"))
	{
		process->m_env->err_code = ft_echo(token);
		return (0);
	}
	return (99);
}
