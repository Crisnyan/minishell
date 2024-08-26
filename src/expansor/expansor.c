/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:41:46 by cristian          #+#    #+#             */
/*   Updated: 2024/08/27 00:49:09 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct s_dollar
{
	int			pos;
	struct s_dollar	*next;
}	t_dollar;

static void	expand(t_token *tok, t_dict *m_env)
{
	char *expanded;
	
	expanded = ft_strtrim(tok->data, "$");
	if (!expanded)
		return ;
	free(tok->data);
	tok->data = ft_getenv(expanded, m_env);
	free(expanded);
}

static void	rearrange(t_token *dollar, t_token *string)
{
	free(dollar->data);
	dollar->data = string->data;
	dollar->next = string->next;
	dollar->flags = string->flags;
	free(string);
}

static char	*malloc_join(t_token *tok, int counter)
{
	int 	i;
	char	*joined;

	ft_printf(2, "entra malloc join\n");
	i = 0;
	while (counter--)
	{
		i += ft_strlen(tok->data);
		ft_printf(2, "STRLEN: %d\n", ft_strlen(tok->data));
		tok = tok->next;
	}
	ft_printf(2, "TOTAL: %d\n", i);
	joined = malloc(sizeof(char) * i + 1);
	if (!joined)
		return (NULL);
	ft_printf(2, "i: %d\n", i);
	joined[i] = '\0';
	ft_printf(2, "pasa malloc join\n");
	return (joined);
}

static void	tokjoin(t_token *tok, int counter)
{
	int		i;
	int		j;
	char	*joined;
	t_token	*temp;
	t_token	*head;


	i = 0;
	j = 0;
	joined = malloc_join(tok, counter + 1);
	if (!joined)
		return ;
	ft_printf(2, "pasa malloc comprobacion\n");
	head = tok;
	while (tok->data[i])
		joined[j++] = tok->data[i++];
	ft_printf(2, "pasa asignacion\n");
	tok = tok->next;
	ft_printf(2, "pasa tok->next\n");
	ft_printf(2, "counter: %d\n", counter);
	while (counter--)
	{
		ft_printf(2, "counter: %d\n", counter);
		i = 0;
		ft_printf(2, "j: %d\n", j);
		ft_printf(2, "tok->data:%s\n", tok->data);
		temp = tok;
		ft_printf(2, "llega 1\n");
		while (tok->data[i])
			joined[j++] = tok->data[i++];
		ft_printf(2, "tok->data[%d]:%s\n", i, tok->data[i]);
		ft_printf(2, "llega 2\n");
		write(2, joined, j);
		tok = tok->next;
		free(temp->data);
		free(temp);
	}
	free(head->data);
	head->data = joined;
	head->next = tok;
}

static t_dollar	*create_dollar_list(int i)
{
	t_dollar	*dollar;

	dollar = (t_dollar *)malloc(sizeof(t_dollar));
	if (!dollar)
		return (NULL);
	dollar->pos = i;
	dollar->next = NULL;
	return (dollar);
}

static int	is_expansion_dollar(char *data, int i, int restart)
{
	static int	was_dollar = 0;
	static int	has_been_dollar = 0;

	printf("entra en exp dollar\n");
	if (restart == 1)
	{
		was_dollar = 0;
		has_been_dollar = 0;
		return (0);
	}
	if ((has_been_dollar && was_dollar)
	|| (data[i - 1] && data[i - 1] != '$')
	|| i == 0)
	{
		was_dollar = 0;
		has_been_dollar = 0;
	}
	if (was_dollar)
	{
		has_been_dollar = 1;
		return (0);
	}
	if (data[i] == '$' && data[i + 1])
	{
		if (i > 0 && data[i - 1] == '$') 
			was_dollar = 1;
	}
	if (!was_dollar && data[i + 1])
		return (1);
	printf("sale de exp dollar\n");
	return (0);
}

static t_dollar	*strdlr(char *data)
{
	int			i;
	t_dollar	*dollar_list;
	t_dollar	*prev;
	t_dollar	*head;

	i = 0;
	printf("entra en strdlr\n");
	head = NULL;
	prev = NULL;
	is_expansion_dollar("", i, 1);
	while (data[i])
	{
		printf("entra en strdlr loop\n");
		if ((data[i] == '$' && is_expansion_dollar(data, i, 0)) || i == 0)
		{
			dollar_list = create_dollar_list(i);
			if (prev)
				prev->next = dollar_list;
			if (!head)
				head = dollar_list;
			prev = dollar_list;
			dollar_list = dollar_list->next;
		}
		printf("pasa strdlr loop\n");
		i++;
	}
	printf("pasa strdlr loop\n");
	if (!prev)
		return (NULL);
	dollar_list = create_dollar_list(i);
	prev->next = dollar_list;
	printf("pasa strdlr\n");
	return (head);
}

static void	free_dollar_list(t_dollar *list)
{
	t_dollar *head;

	while (list)
	{
		head = list;
		list = list->next;
		free(head);
	}
}

static void pid_attach(t_token *tok, char *str, char *data)
{
	char	*temp;
	int		i;

	printf("entra a expand pid\n");
	while (data[i] == '$')
		i++;
	tok->data = str;
	if (data[i])
	{
		temp = ft_substr(data, i, strlen(data));
		tok->data = ft_strjoin(str, temp);
		free(temp);
		free(str);
	}
		ft_printf(2, "%s\n", tok->data);
}

static void expand_attach(t_token *tok, char *data, t_dict *m_env, int expand)
{
	char	*str;
	char	*temp;
	int		i;

	printf("entra a expand attach\n");
	while (data[i] && !is_space(data[i]))
		printf("%d\n", i++);
	temp = ft_substr(data, 0, i);
	ft_printf(2, "string: %s\n", temp);
	if (expand)
	{
		str = ft_getenv(&temp[1], m_env);
		free(temp);
	}
	else
		str = temp;
	tok->data = str;
	if (data[i])
	{
		temp = ft_substr(data, i, strlen(data));
		tok->data = ft_strjoin(str, temp);
		free(temp);
	}
	ft_printf(2, "%s\n", tok->data);
}

static t_token	*create_token(char *data, t_dict *m_env)
{
	t_token		*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (data[1] == '$')
		pid_attach(tok, ft_itoa((int)getpid()), data);
	else if (data [0] != '$')
		expand_attach(tok, data, m_env, 0);
	else
		expand_attach(tok, data, m_env, 1);
	tok->next = NULL;
	tok->flags = -1;
	tok->adv = -1;
	return (tok);
}

static void print_dollar_list(t_dollar *list)
{
		while (list)
		{
			ft_printf(2, "self: %p\n", list);
			ft_printf(2, "pos: %d\n", list->pos);
			ft_printf(2, "next: %p\n", list->next);
			list = list->next;
		}
}

static char	*expand_join(char *data, t_dollar *list, t_dict *m_env)
{
	int		counter;
	char	*str;
	t_token	*tok;
	t_token	*head;
	t_token	*prev;

	head = NULL;
	prev = NULL;
	counter = 0;
	printf("entra a expand join\n");
	print_dollar_list(list);
	while (list)
	{
		ft_printf(2, "pasa\n");
		if (!list->next)
			break;
		if (prev)
			prev->next = tok;
		str = ft_substr(data, list->pos, list->next->pos);
		ft_printf(2, "entra a crear token\n");
		tok = create_token(str, m_env);
		if (!head)
			head = tok;
		prev = tok;
		tok = tok->next;
		list = list->next;
		counter++;
		//free(str);
	}
	return (tokjoin(head, counter - 1), head->data);
}

static void	expand_in_quote(t_token *tok, t_dict *m_env)
{
	t_dollar	*dollar_list;

	dollar_list = NULL;
	printf("entra en expand in quote\n");
		 dollar_list = strdlr(tok->data);
	if (dollar_list)
	{
		printf("entra al if\n");
		tok->data = expand_join(tok->data, dollar_list, m_env);
		free_dollar_list(dollar_list);
	}
}

static void	final_join(t_token *tok)
{
	int		counter;
	t_token	*temp;

		counter = 0;
		while ((tok->next && tok->next->flags == FOLLOW_DQUOTE)
		|| (tok->next && tok->next->flags == FOLLOW_QUOTE))
		{
			temp = temp->next;
			counter++;
		}
		if (counter)
			tokjoin(tok, counter);
}

t_token *expansor(t_token *tok, t_dict *m_env)
{
	t_token *head;

	head = tok;
	print_token_list(head);
	while (tok)
	{
		ft_printf(2, "flags: %d\n", tok->flags);
		if (tok->flags >= DOLLAR_QUOTE && tok->flags <= FOLLOW_DQUOTE)
		{
			ft_printf(2, "llega a expand in quote\n");
			expand_in_quote(tok, m_env); 
		}
		//print_token_list(head);
		if (tok->flags == DOLLAR && !is_space(tok->data[1]))
			expand(tok, m_env);
		else if (tok->flags == DOLLAR_QUOTE 
		|| tok->flags == DOLLAR_DQUOTE)
			rearrange(tok, tok->next);
		tok = tok->next;
	}
	final_join(head);
	return (head);
}
