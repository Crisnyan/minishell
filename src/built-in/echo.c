/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 23:31:48 by cristian          #+#    #+#             */
/*   Updated: 2024/08/04 01:04:27 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_tokens(t_token *token, int newline)
{
	while (token)
	{
		printf("%s", token->data);
		token = token->next;
		if (token)
		{
			if (token->flags == REDIRECT || token->flags == PIPE)
				break;
		printf(" ");
		}
	}
	if (newline)
		printf("\n");
}

static int	check_n(t_token *token)
{	
	int	i;

	i = 0;
	if (token->data[0] == '-')
	{
		i++;
		while (token->data[i] && token->data[i] == 'n')
		{
			i++;
		}
		if (token->data[i])
			return (0);
		return (1);
	}
	return (0);
}

void	ft_echo(t_token *token)
{
	int	newline;

	newline = 1;
	if (!token->next)
		return;
	token = token->next;
	while (check_n(token))
	{
		newline = 0;
		token = token->next;
	}
	print_tokens(token, newline);
}
