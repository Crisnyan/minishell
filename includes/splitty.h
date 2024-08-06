/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitty.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:25:00 by cristian          #+#    #+#             */
/*   Updated: 2024/08/06 16:59:44 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLITTY_H
# define SPLITTY_H

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
