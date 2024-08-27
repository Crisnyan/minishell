/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:41:46 by cristian          #+#    #+#             */
/*   Updated: 2024/08/27 04:18:31 by cristian         ###   ########.fr       */
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
	
	printf("entra?\n");
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

static char	*null_ignore_join(char *s1, char *s2)
{
	printf("entra null_ignore_join\n");
	if (s1 == NULL && s2 == NULL)
	{
		printf("entra 1\n");
		return (NULL);
	}
	if (s1 == NULL)
	{
		printf("entra 2\n");
		return (s2);
	}
	if (s2 == NULL)
	{
		printf("entra 3\n");
		return (s1);
	}
	printf("entra 4\n");
	return (ft_strjoin(s1, s2));
}

static void join(t_token *tok, t_token *prev)
{
	char	*temp;

	printf("entra join\n");
	temp = tok->data;
	printf("prev->data:%s\n", tok->data);
	prev->data = null_ignore_join(prev->data, tok->data);
	printf("prev->data:%s\n", tok->data);
	prev->next = tok->next;
	free(temp);
	free(tok);
}

static void tokjoin(t_token *tok)
{
	t_token *prev;

	prev = NULL;
	printf("entra tokjoin\n");
	while (tok)
	{
		while (tok->flags == FOLLOW_QUOTE || tok->flags == FOLLOW_DQUOTE)
			join(tok, prev);
		if (!prev)
			prev = tok;
		tok = tok->next;
	}
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
		else if (tok->flags >= DOLLAR_QUOTE && tok->flags <= FOLLOW_DQUOTE)
			expand_string(tok, m_env);
		else if (tok->flags == DOLLAR_QUOTE 
		|| tok->flags == DOLLAR_DQUOTE)
			rearrange(tok, tok->next);
		tok = tok->next;
	}
	return (print_token_list(head), tokjoin(head), print_token_list(head), head);
}
