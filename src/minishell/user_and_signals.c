/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_and_signals.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:38:06 by cristian          #+#    #+#             */
/*   Updated: 2024/09/14 14:00:33 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_global_signal;

char	*get_user(char *env)
{
	char	*user;

	if (!env)
		env = ft_strjoin(ANSI_COLOR_CYAN_B, "Guest");
	else
		env = ft_strjoin(ANSI_COLOR_CYAN_B, env);
	user = ft_strjoin(env, "@minishell\x1b[0m:");
	free(env);
	return (user);
}

void	handle_c_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		close(STDIN_FILENO);
		ft_printf(STDERR_FILENO, "\n");
		g_global_signal = 130;
	}
}

void	handle_c(int signal)
{
	if (signal == SIGINT)
	{
		ft_printf(STDOUT_FILENO, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_global_signal = 130;
	}
}

char	*format_prompt(t_dict *m_env)
{
	char	*prompt;
	char	*user;
	char	*aux;

	user = NULL;
	aux = ft_getenv("USER", m_env);
	user = get_user(aux);
	free(aux);
	prompt = getcwd(NULL, 0);
	if (!prompt)
		return (user);
	prompt = ft_strattach(PREFIX, &prompt);
	prompt = ft_strappend(&prompt, SUFFIX);
	prompt = ft_strattach(user, &prompt);
	free(user);
	return (prompt);
}

void	free_split(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
}
