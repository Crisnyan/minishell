/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:19:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/28 00:24:09 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

const char	*get_user(char *env)
{
	char 	*user;

	if (!env)
		return ((void)write(2, "user not found\n", 15), exit(0), NULL);
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

void	free_split(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}

void	token_loop(char **tokens)
{
	int		i;

	i = 0;
	while (tokens[i])
		printf("%s\n", tokens[i++]);
	free_split(tokens);
}

int	main(int argc, char **argv, char **envp)
{
	char *line;
	const char	*user = get_user(getenv("USER"));

	if (argc != 1 && argv[0][2] == 'm' && envp)
		return (0);
	cntl_signals();
	while (1)
	{
		line = readline(user);
		if (!line)
			return(exit(0), 0);
		token_loop(ft_split(line, ' '));
	}
}
