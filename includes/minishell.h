/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:53:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/14 20:23:34 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <limits.h>
# include <../readline/readline.h>
# include <../readline/history.h>
# include "../libft/libft.h"
# include "../printf/ft_printf.h"

# include "structs.h"

# include "env.h"
# include "splitty.h"
# include "executor.h"
# include "expansor.h"

# define PROMPT					0
# define LINE					1
# define ANSI_COLOR_RED_B		"\x1b[31;1m"
# define ANSI_COLOR_GREEN_B		"\x1b[32;1m"
# define ANSI_COLOR_YELLOW_B	"\x1b[33;1m"
# define ANSI_COLOR_BLUE_B		"\x1b[34;1m"
# define ANSI_COLOR_MAGENTA_B	"\x1b[35;1m"
# define ANSI_COLOR_CYAN_B		"\x1b[36;1m"
# define ANSI_COLOR_RESET		"\x1b[0m"

# define ERR_QUOTES		"minishell: syntax error involving quotes\n"
# define ERR_SYNTAX		"minishell: syntax error near unexpected token '%s'\n"

extern int	g_global_signal;

t_token		**split_cmd(t_token *tokens, int pipes);
t_token		*null_var(char **str);
void		handle_c(int signal);
void		handle_c_heredoc(int signal);
void		handle_c_heredoc(int signal);
void		handle_c(int signal);
void		free_split(char **tokens);
void		free_cmd_list(t_token **cmd_list, int n);
void		sig_and_line(char **str, t_dict m_env);
void		init_process(t_process *process, t_dict *m_env);
void		quote_error(char **str, t_process *process);
void		process_pipes(char **str, t_token *tok, t_process *process);
char		*get_user(char *env);
char		*format_prompt(t_dict *m_env);
int			parse_error(char **str, t_token *tok);
int			count_pipes(t_token *list);
int			parse(t_token *token, t_process *process);
int			count_quotes(char *line);

#endif
