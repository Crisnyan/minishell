/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:41:46 by cristian          #+#    #+#             */
/*   Updated: 2024/08/03 13:35:54 by vperez-f         ###   ########.fr       */
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
		if (tok->data[i] == '$' && tok->data[i + 1] && (tok->data[i + 1] != '\'' && tok->data[i + 1] != '\"' && tok->data[i + 1] != ' '))
		{
			aux = ft_substr(tok->data, j, i - j);
			if (!res)
				res = ft_strdup(aux);
			else
				res = ft_strappend(&res, aux);
			free(aux);
			j = i + 1;
			while (tok->data[j] && tok->data[j] != '$' && tok->data[j] != '?' && tok->data[j] != ' ' && 
				tok->data[j] != '\t' && tok->data[j] != '\'' && tok->data[j] != '\"')
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
	printf("EXPANSOR RESULT: %s\n", tok->data);
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

t_token *expansor(t_token *tok, t_dict *m_env)
{
	t_token *head;

	head = tok;
	printf("INTO EXPANSOR\n");
	printf("++++++++++++++++++++++\n");
	print_token_list(head);
	head = tok;
	while (tok)
	{
		if (tok->flags == DOLLAR && !is_space(tok->data[1]))
			expand(tok, m_env);
		else if (tok->flags == DQUOTE && !is_space(tok->data[1]))
			expand_string(tok, m_env);
		else if (tok->flags == DOLLAR_QUOTE 
		|| tok->flags == DOLLAR_DQUOTE)
			rearrange(tok, tok->next);
		tok = tok->next;
	}
	return (head);
}
