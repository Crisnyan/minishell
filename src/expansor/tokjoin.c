/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokjoin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 01:57:46 by cristian          #+#    #+#             */
/*   Updated: 2024/09/16 00:00:52 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand(t_token *tok, t_dict *m_env)
{
	char	*expanded;

	expanded = ft_strtrim(tok->data, "$");
	free(tok->data);
	tok->data = ft_getenv(expanded, m_env);
	free(expanded);
}

void	rearrange(t_token *dollar, t_token *string)
{
	free(dollar->data);
	dollar->data = string->data;
	dollar->next = string->next;
	dollar->flags = string->flags;
	free(string);
}

static void	join(t_token *tok)
{
	t_token	*temp;

	temp = NULL;
	while (tok && tok->next && (tok->next->flags == FOLLOW_STRING
			||tok->next->flags == FOLLOW_QUOTE
			|| tok->next->flags == FOLLOW_DQUOTE))
	{
		temp = tok->next;
		tok->data = ft_strappend(&tok->data, temp->data);
		tok->next = temp->next;
		free(temp->data);
		free(temp);
	}
}

void	tokjoin(t_token *tok)
{
	while (tok)
	{
		if (tok->next && (tok->next->flags == FOLLOW_QUOTE
				|| tok->next->flags == FOLLOW_DQUOTE
				|| tok->next->flags == FOLLOW_STRING))
		{
			join(tok);
		}
		tok = tok->next;
	}
}

t_token	*expansor(t_token *tok, t_dict *m_env)
{
	t_token	*head;

	head = tok;
	while (tok)
	{
		if (tok->flags == DQUOTE || tok->flags == FOLLOW_DQUOTE
			|| tok->flags == STRING || tok->flags == FOLLOW_STRING)
			{
				expand_string(tok, m_env);
				if (tok->flags == STRING || tok->flags == FOLLOW_STRING)
				{
					space_expansor_retoken(tok);
				}
			}
		tok = tok->next;
	}
	return (tokjoin(head), head);
}
