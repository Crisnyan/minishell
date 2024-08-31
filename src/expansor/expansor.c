/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:41:46 by cristian          #+#    #+#             */
/*   Updated: 2024/08/29 18:08:58 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	expand_string(t_token *tok, t_dict *m_env)
{
	int		i;
	int		j;
	char	*res;
	char	*aux;
	char	*expanded;
	char	*to_expand;

	i = 0;
	j = 0;
	res = NULL;
	while (tok->data[i])
	{	
		if (tok->data[i] == '$' && tok->data[i + 1] &&
			(!is_quote(tok->data[i + 1]) && !is_space(tok->data[i + 1])))
		{
			aux = ft_substr(tok->data, j, i - j);
			res = ft_strappend(&res, aux);
			free(aux);
			j = i + 1;
			while (tok->data[j] && tok->data[j] != '$' && tok->data[j] != '?' &&
				!is_space(tok->data[j]) && !is_quote(tok->data[j]))
			{
				j++;
			}
			if (tok->data[j] == '$' || tok->data[j] == '?')
				j++;
			to_expand = ft_substr(tok->data, i + 1, j - i - 1);
			expanded = ft_getenv(to_expand, m_env);
			free(to_expand);
			res = ft_strappend(&res, expanded);
			free(expanded);
			i = j - 1;
		}
		if (tok->next && (tok->next->flags == FOLLOW_QUOTE || tok->next->flags == FOLLOW_DQUOTE) &&
			(tok->flags == STRING || tok->flags == FOLLOW_STRING) && tok->data[i] == '$' && !tok->data[i + 1])
		{
			aux = ft_substr(tok->data, j, i - j);
			res = ft_strappend(&res, aux);
			free(aux);
			j = i + 1;
			res = ft_strappend(&res, NULL);
		}
		i++;
	}
	if (tok->data[j])
	{
		aux = ft_substr(tok->data, j, i);
		res = ft_strappend(&res, aux);
		free(aux);
	}
	if (res)
	{
		free(tok->data);
		tok->data = ft_strdup(res);
		free(res);
	}
}

void	expand(t_token *tok, t_dict *m_env)
{
	char *expanded;

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

static void join(t_token *tok)
{
	t_token	*temp;

	temp = NULL;
	while (tok && tok->next && (tok->next->flags == FOLLOW_STRING ||
		tok->next->flags == FOLLOW_QUOTE || tok->next->flags == FOLLOW_DQUOTE))
	{
		temp = tok->next;
		tok->data = ft_strappend(&tok->data, temp->data);
		tok->next = temp->next;
		free(temp->data);
		free(temp);
	}
}

static void tokjoin(t_token *tok)
{
	while (tok)
	{
		if (tok->next && (tok->next->flags == FOLLOW_QUOTE ||
			tok->next->flags == FOLLOW_DQUOTE))
		{
			join(tok);
		}
		tok = tok->next;
	}
}

t_token *expansor(t_token *tok, t_dict *m_env)
{
	t_token *head;

	head = tok;
	while (tok)
	{
		if (tok->flags == DQUOTE || tok->flags == FOLLOW_DQUOTE || tok->flags == STRING || tok->flags == FOLLOW_STRING)
			expand_string(tok, m_env);
		tok = tok->next;
	}
	return (tokjoin(head), head);
}
