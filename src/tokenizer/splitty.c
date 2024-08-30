/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitty.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 01:08:41 by cristian          #+#    #+#             */
/*   Updated: 2024/08/29 18:09:39 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_quote(char c)
{
	return (c == '\"' || c == '\'');
}

int	is_special(char c) {
	return (c == '<' || c == '>' || c == '|' || c == '$');
}

int	is_space(char c)
{
	return ((c >= '\a' && c <= '\r') || c == ' ');
}

int	is_special_no_dolar(char c) {
	return (c == '<' || c == '>' || c == '|');
}
/*
int	dollar_flags(char *line)
{
	if (line[0] != '$')
		return (STRING);
	else if (line[0] == '$' && line[1] == '\'')
		return (DOLLAR_QUOTE);
	else if (line[0] == '$' && line[1] == '\"')
		return (DOLLAR_DQUOTE);
	return (DOLLAR);
}*/
static void quote_flags(t_token *tok, char quote, char *line, int n)
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

static t_token	*create_normal_token(char *line, char *original, int n)
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

static t_token	*create_str_token(char *line, char quote, char *original, int n)
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
	if ((i == 1  && !line[i])|| !line[i])
		i--;
	tok->adv = i + 1;
	return (tok);
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
		if ((line[0] == '<' && line[1] == '<'))
			tok->flags = HEREDOC;
		else if ((line[0] == '>' && line[1] == '>'))
			tok->flags = APPEND;
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
		if (line[0] == '<')
			tok->flags = I_REDIRECT;
		else if (line[0] == '>')
			tok->flags = O_REDIRECT;
		tok->data = ft_substr(line, 0, 1);
	}
	return (tok);
}
	
static t_token *get_token(char *line, char quote, char *original, int n)
{
	t_token *tok;

	if (is_quote(*line))
	{
		//ft_printf(2, "entra 1\n");
		tok = create_str_token(line, quote, original, n);
	}
	else if (is_special(*line) && *line != '$')
	{
		//ft_printf(2, "entra 2\n");
		tok = create_special_token(line);
	}
	else 
	{
		//ft_printf(2, "entra 3\n");
		tok = create_normal_token(line, original, n);
	}
	return (tok);
}

void	print_token(t_token *token)
{
	//printf("self: %p\n", token);
	printf("data: %s\n", token->data);
	printf("flags: %d\n", token->flags);
	printf("adv: %d\n", token->adv);
	//printf("next: %p\n", token->next);
}

void	print_token_list(t_token *head)
{
	t_token *temp;

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

t_token *minishplit(char *line)
{
	char	quote;
	int	i;
	t_token	*ptr;
	t_token	*prev;
	t_token *head;

	i = 0;
	quote = '\0';
	head = NULL;
	prev = NULL;
	while (line[i])
	{
		while (is_space(line[i]))
			i++;
		if (!line[i])
			break;
		if (is_quote(line[i]))
			quote = line[i];
		ptr = get_token(&line[i], quote, line, i);
		i += ptr->adv;
		if (!head)
			head = ptr;
		if (prev)
			prev->next = ptr;
		prev = ptr;
		ptr = ptr->next;
		quote = '\0';
	}
	return (head);
}
//
//int	main(void)
//{
//	char	*str;
//	t_token	*head;
//
//	str = "hola que tal hermano";
//	head = minishplit(str);
//	while (head != NULL)
//	{
//		print_token(head);
//		head = head->next;
//	}
//}
