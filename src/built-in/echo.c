/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 23:31:48 by cristian          #+#    #+#             */
/*   Updated: 2024/09/02 21:10:25 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	echo_check_flags(t_token *tok)
{
	if (tok->flags == I_REDIRECT || tok->flags == O_REDIRECT
		|| tok->flags == HEREDOC || tok->flags == APPEND || tok->flags == PIPE)
	{
		return (1);
	}
	else
		return (0);
}

static void	print_tokens(t_token *token, int newline)
{
	while (token)
	{
		if (echo_check_flags(token))
			break ;
		printf("%s", token->data);
		token = token->next;
		if (token)
			printf(" ");
	}
	if (newline)
		printf("\n");
}

static int	check_n(t_token *token)
{
	int	i;

	i = 0;
	if (!token)
		return (0);
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

int	ft_echo(t_token *token)
{
	int	newline;

	newline = 1;
	if (!token->next)
	{
		printf("\n");
		return (0);
	}
	token = token->next;
	while (check_n(token))
	{
		newline = 0;
		token = token->next;
	}
	print_tokens(token, newline);
	return (0);
}
