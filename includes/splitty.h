/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitty.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:25:00 by cristian          #+#    #+#             */
/*   Updated: 2024/09/14 13:00:13 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLITTY_H
# define SPLITTY_H

# define PREV 0
# define HEAD 1
# define PTR 2

int		is_quote(char c);
int		is_special(char c);
int		is_space(char c);
int		is_special_no_dolar(char c);
t_token	*get_token(char *line, char quote, char *original, int n);
void	print_token(t_token *token);
void	print_token_list(t_token *head);
void	free_list(t_token *head);
void	set_tokens(t_token **tok);
void	quote_flags(t_token *tok, char quote, char *line, int n);
void	print_token(t_token *token);
void	print_token_list(t_token *head);
void	free_list(t_token *head);
t_token	*create_normal_token(char *line, char *original, int n);
t_token	*create_str_token(char *line, char quote, char *original, int n);
int		redir_flags(char *line);
int		i_or_o_flags(char *line);
t_token	*create_special_token(char *line);
t_token	*minishplit(char *line);

#endif
