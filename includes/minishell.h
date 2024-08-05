/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:53:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 18:59:50 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include	<stdio.h>
# include	<stdlib.h>
# include 	<signal.h>
# include 	<unistd.h>
# include	<../readline/readline.h>
# include	<../readline/history.h>
# include	"../libft/libft.h"
# include	"../printf/ft_printf.h"

# include	"splitty.h"
# include	"executor.h"
# include	"expansor.h"
# include 	"env.h"

#define ANSI_COLOR_RED_B		"\x1b[31;1m"
#define ANSI_COLOR_GREEN_B		"\x1b[32;1m"
#define ANSI_COLOR_YELLOW_B		"\x1b[33;1m"
#define ANSI_COLOR_BLUE_B		"\x1b[34;1m"
#define ANSI_COLOR_MAGENTA_B	"\x1b[35;1m"
#define ANSI_COLOR_CYAN_B		"\x1b[36;1m"
#define ANSI_COLOR_RESET		"\x1b[0m"

#endif
