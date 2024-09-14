/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 20:12:00 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/14 19:26:08 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	built_in_cd(t_token *token, t_process *process)
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

int	built_in_unset(t_token *token, t_process *prcs)
{
	if (!token->next || token->next->flags == PIPE)
		return (prcs->m_env->err_code = ft_unset(token->data, prcs->m_env));
	token = token->next;
	while (token && !token->flags)
	{
		prcs->m_env->err_code = ft_unset(token->data, prcs->m_env);
		token = token->next;
	}
	return (0);
}

int	built_in_export(t_token *tok, t_process *prcs)
{
	if (!tok->next || tok->next->flags == PIPE)
	{
		return (prcs->m_env->err_code = ft_export(tok->data, prcs->m_env, 1));
	}
	tok = tok->next;
	while (tok && !tok->flags)
	{
		prcs->m_env->err_code = ft_export(tok->data, prcs->m_env, 0);
		tok = tok->next;
	}
	return (0);
}

int	built_in_env(t_token *token, t_process *process)
{
	if (token->next && !(token->next->flags == PIPE))
	{
		process->m_env->err_code = 1;
		return (ft_printf(2, ENV_ERROR, token->next->data), 1);
	}
	print_env(process->m_env);
	return (0);
}

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
		return (built_in_env(token, process));
	else if (!ft_strcmp(token->data, "export"))
		return (built_in_export(token, process));
	else if (!ft_strcmp(token->data, "unset"))
		return (built_in_unset(token, process));
	else if (!ft_strcmp(token->data, "cd"))
		return (built_in_cd(token, process));
	else if (!ft_strcmp(token->data, "exit"))
		return (ft_exit(token));
	else if (!ft_strcmp(token->data, "echo"))
	{
		process->m_env->err_code = ft_echo(token);
		return (0);
	}
	return (99);
}
