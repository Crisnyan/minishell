/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_funcs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 00:04:49 by cristian          #+#    #+#             */
/*   Updated: 2024/09/03 00:27:11 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_quote(char c)
{
	return (c == '\"' || c == '\'');
}

int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|' || c == '$');
}

int	is_space(char c)
{
	return ((c >= '\a' && c <= '\r') || c == ' ');
}

int	is_special_no_dolar(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

void	quote_flags(t_token *tok, char quote, char *line, int n)
{
	if (n && !is_space(line[n - 1]) && !is_special_no_dolar(line[n - 1]))
	{
		if (quote == '\'')
			tok->flags = FOLLOW_QUOTE;
		else if (quote == 's')
			tok->flags = FOLLOW_STRING;
		else
			tok->flags = FOLLOW_DQUOTE;
	}
	else
	{
		if (quote == '\'')
			tok->flags = QUOTE;
		else if (quote == 's')
			tok->flags = STRING;
		else
			tok->flags = DQUOTE;
	}
}
