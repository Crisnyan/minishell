/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitty.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:25:00 by cristian          #+#    #+#             */
/*   Updated: 2024/07/31 01:25:47 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLITTY_H
# define SPLITTY_H

enum flags
{
	STRING,
	PIPE,
	REDIRECT,
	DOLLAR,
	DOLLAR_QUOTE,
	DOLLAR_DQUOTE,
	QUOTE,
	DQUOTE
};

typedef struct s_token
{
	int			flags;
	int			adv;
	char		*data;
	struct s_token	*next;
}	t_token;

int	is_quote(char c);
int	is_special(char c);
int	is_space(char c);
void	print_token(t_token *token);
void	print_token_list(t_token *head);
void	free_list(t_token *head);
t_token	*create_normal_token(char *line);
t_token	*create_str_token(char *line, char quote);
t_token	*create_special_token(char *line);
t_token *get_token(char *line, char quote);
t_token *minishplit(char *line);
#endif
