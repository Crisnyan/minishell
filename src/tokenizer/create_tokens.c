/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 00:06:49 by cristian          #+#    #+#             */
/*   Updated: 2024/09/03 00:26:25 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*create_normal_token(char *line, char *original, int n)
{
	int			i;
	t_token		*tok;

	i = 0;
	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	while (!is_space(line[i]) && line[i]
		&& !is_special_no_dolar(line[i]) && !is_quote(line[i]))
		i++;
	tok->data = ft_substr(line, 0, i);
	quote_flags(tok, 's', original, n);
	tok->next = NULL;
	tok->adv = i;
	return (tok);
}

t_token	*create_str_token(char *line, char quote, char *original, int n)
{
	int			i;
	t_token		*tok;

	i = 1;
	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	while (line[i] && line[i] != quote)
		i++;
	if (i > 1)
		tok->data = ft_substr(line, 1, i - 1);
	else
		tok->data = ft_strdup("");
	quote_flags(tok, quote, original, n);
	tok->next = NULL;
	if ((i == 1 && !line[i]) || !line[i])
		i--;
	tok->adv = i + 1;
	return (tok);
}

int	redir_flags(char *line)
{
	if ((line[0] == '<' && line[1] == '<'))
		return (HEREDOC);
	else
		return (APPEND);
}

int	i_or_o_flags(char *line)
{
	if (line[0] == '<')
		return (I_REDIRECT);
	else
		return (O_REDIRECT);
}

t_token	*create_special_token(char *line)
{
	t_token		*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	tok->next = NULL;
	tok->adv = 1;
	if (!tok)
		return (NULL);
	if ((line[0] == '<' && line[1] == '<')
		|| (line[0] == '>' && line[1] == '>'))
	{
		tok->flags = redir_flags(line);
		tok->adv = 2;
		tok->data = ft_substr(line, 0, 2);
	}
	else if (line[0] == '|')
	{
		tok->flags = PIPE;
		tok->data = ft_substr(line, 0, 1);
	}
	else if (line[0] == '<' || line[0] == '>')
	{
		i_or_o_flags(line);
		tok->data = ft_substr(line, 0, 1);
	}
	return (tok);
}
