/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 00:20:45 by cristian          #+#    #+#             */
/*   Updated: 2024/09/03 00:28:37 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*get_token(char *line, char quote, char *original, int n)
{
	t_token	*tok;

	if (is_quote(*line))
	{
		tok = create_str_token(line, quote, original, n);
	}
	else if (is_special(*line) && *line != '$')
	{
		tok = create_special_token(line);
	}
	else 
	{
		tok = create_normal_token(line, original, n);
	}
	return (tok);
}

void	print_token(t_token *token)
{
	printf("data: %s\n", token->data);
	printf("flags: %d\n", token->flags);
	printf("adv: %d\n", token->adv);
}

void	print_token_list(t_token *head)
{
	t_token	*temp;

	temp = head;
	while (temp)
	{
		print_token(temp);
		temp = temp->next;
	}
}

void	free_list(t_token *head)
{
	t_token	*ptr;

	ptr = head;
	while (ptr)
	{
		ptr = ptr->next;
		if (head->data)
			free(head->data);
		free(head);
		head = ptr;
	}
}

void	set_tokens(t_token **tok)
{
	if (!tok[HEAD])
		tok[HEAD] = tok[PTR];
	if (tok[PREV])
		tok[PREV]->next = tok[PTR];
}
