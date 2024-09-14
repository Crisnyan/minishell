/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:00:40 by vpf               #+#    #+#             */
/*   Updated: 2024/09/14 18:50:14 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_redirs(t_process *prcs, t_cmd *cmd, t_token *temp, t_token *cmd_list)
{
	if (temp->flags == HEREDOC)
		heredoc_redirection(prcs);
	else if (temp->flags == I_REDIRECT)
		input_redirection(prcs, cmd, cmd_list);
	else if (temp->flags == O_REDIRECT)
		output_redirection(prcs, cmd, cmd_list);
	else if (temp->flags == APPEND)
		append_redirection(prcs, cmd, cmd_list);
	if (prcs->m_env->err_code)
		return (1);
	else
		return (0);
}

void	check_previous(t_token **redir_list, t_token **cmd_list)
{
	if (redir_list[PREV_EXE] != NULL)
		redir_list[PREV_EXE]->next = (*cmd_list);
	else
		redir_list[REDIR_CMD] = (*cmd_list);
}

void	advance_redir(t_token **redir_list, t_token **cmd_list)
{
	redir_list[AUX_EXE] = (*cmd_list)->next;
	(*cmd_list)->next = NULL;
	(*cmd_list) = redir_list[AUX_EXE];
}

int	is_redir(int flag)
{
	if (flag == I_REDIRECT || flag == O_REDIRECT
		|| flag == HEREDOC || flag == APPEND)
		return (1);
	else
		return (0);
}
