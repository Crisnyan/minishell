/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:19:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/27 19:12:53 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

const char	*get_user(char *env)
{
	char 	*user;

	user = ft_strjoin(env, " -- minishell: ");
	return (user);
}

void	handle_c(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	cntl_signals(void)
{
	struct	sigaction cntl_c;
	struct	sigaction cntl_bar;

	cntl_c.sa_handler = &handle_c;
	sigemptyset(&cntl_c.sa_mask);
	sigaction(SIGINT, &cntl_c, NULL);
	cntl_bar.sa_handler = SIG_IGN;
	sigemptyset(&cntl_bar.sa_mask);
	sigaction(SIGQUIT, &cntl_bar, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	int			i;
	const char	*user = get_user(getenv("USER"));
	char		*test;

	cntl_signals();
	if (argc != 1 && argv[0][2] == 'm' && envp)
		return (0);
	i = 0;
	while (i < 10)
	{
		test = readline(user);
		if (!test)
			return(exit(0), 0);
		//ft_printf(STDOUT_FILENO, "  %s\n", test);
		free(test);
		i++;
	}
}
