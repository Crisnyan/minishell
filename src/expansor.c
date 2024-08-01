/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:41:46 by cristian          #+#    #+#             */
/*   Updated: 2024/07/31 06:00:28 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand(t_token *tok)
{
	char *expanded;
	
	expanded = getenv((const char *)&tok->data[1]);
	free(tok->data);
	tok->data = expanded;
}

void	rearrange(t_token *dollar, t_token *string)
{
	free(dollar->data);
	dollar->data = string->data;
	dollar->next = string->next;
	dollar->flags = string->flags;
	free(string);
}

t_token *expansor(t_token *tok)
{
	t_token *head;

	head = tok;
	while (tok)
	{
		if (tok->flags == DOLLAR && !is_space(tok->data[1]))
			expand(tok);
		else if (tok->flags == DOLLAR_QUOTE 
		|| tok->flags == DOLLAR_DQUOTE)
			rearrange(tok, tok->next);
		tok = tok->next;
	}
	return (head);
}
