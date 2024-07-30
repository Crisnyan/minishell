/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitty.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 01:08:41 by cristian          #+#    #+#             */
/*   Updated: 2024/07/30 07:27:25 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/ft_substr.c"
#include "../libft/ft_strlen.c"
#include "../libft/ft_strdup.c"
#include "../includes/minishell.h"

//enum flags
//{
//	STRING,
//	PIPE,
//	REDIRECT
//};
//
//typedef struct s_token
//{
//	int			flags;
//	int			adv;
//	char		*data;
//	struct s_token	*next;
//}	t_token;

int	is_quote(char c)
{
	return (c == '\"' || c == '\'');
}

int	is_special(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_space(char c)
{
	return ((c >= '\a' && c <= '\r') || c == ' ');
}

t_token	*create_normal_token(char *line)
{
	int			i;
	t_token		*tok;

	i = 0;
	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	while (!is_space(line[i]) && line[i] 
	&& !is_special(line[i]) && !is_quote(line[i]))
		i++;
	tok->data = ft_substr(line, 0, i);
	//printf("%s\n", line);
	//printf("%s\n", tok->data);
	tok->flags = STRING;
	tok->next = NULL;
	tok->adv = i;
	return (tok);
}

t_token	*create_str_token(char *line, char quote)
{
	int			i;
	t_token		*tok;

	i = 1;
	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	while (line[i] != quote && line[i])
		i++;
	tok->data = ft_substr(line, 1, i - 1);
	tok->flags = STRING;
	tok->next = NULL;
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
		tok->flags = REDIRECT;
		tok->adv = 2;
		tok->data = ft_substr(line, 0, 2);
	}
	else if (line[0] == '|')
	{
		tok->flags = PIPE;
		tok->data = ft_substr(line, 0, 1);
	}
	else if (line[0] == '<' || '>')
	{
		tok->flags = REDIRECT;
		tok->data = ft_substr(line, 0, 1);
	}
	return (tok);
}
	
t_token *get_token(char *line, char quote)
{
	t_token *tok;

	if (is_quote(*line))
	{
		//printf("entra 3\n");
		tok = create_str_token(line, quote);
	}
	else if (is_special(*line))
	{
		//printf("entra 2\n");
		tok = create_special_token(line);
	}
	else 
	{
		//printf("entra 1\n");
		tok = create_normal_token(line);
	}
	return (tok);
}

void	print_token(t_token *token)
{
	printf("self: %p\n", token);
	printf("data: %s\n", token->data);
	printf("flags: %d\n", token->flags);
	printf("adv: %d\n", token->adv);
	printf("next: %p\n", token->next);
}

void	print_token_list(t_token *head)
{
	while (head)
	{
		print_token(head);
		head = head->next;
	}
}

void	free_list(t_token *head)
{
	t_token	*ptr;

	ptr = head;
	while (ptr)
	{
		ptr = ptr->next;
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
		ptr = get_token(&line[i], quote);
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