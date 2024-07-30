/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:53:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/30 22:42:26 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include	<stdio.h>
# include	<stdlib.h>
# include	<signal.h>
# include	<unistd.h>
# include	<../readline/readline.h>
# include	<../readline/history.h>
# include	"../libft/libft.h"
# include	"../printf/ft_printf.h"

#define ANSI_COLOR_RED_B		"\x1b[31;1m"
#define ANSI_COLOR_GREEN_B		"\x1b[32;1m"
#define ANSI_COLOR_YELLOW_B		"\x1b[33;1m"
#define ANSI_COLOR_BLUE_B		"\x1b[34;1m"
#define ANSI_COLOR_MAGENTA_B	"\x1b[35;1m"
#define ANSI_COLOR_CYAN_B		"\x1b[36;1m"
#define ANSI_COLOR_RESET		"\x1b[0m"

#define PREFIX					ANSI_COLOR_MAGENTA_B
#define SUFFIX					"\x1b[0m$ "

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
