/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:19:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/01 20:48:16 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_user(char *env)
{
	char 	*user;

	if (!env)
		env = ft_strjoin(ANSI_COLOR_CYAN_B, "Guest");
	else
		env = ft_strjoin(ANSI_COLOR_CYAN_B, env);
	user = ft_strjoin(env, "@minishell\x1b[0m:");
	free(env);
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
	//sigaction(SIGQUIT, &cntl_bar, NULL);
}

char	*format_prompt(t_dict *m_env)
{
	char	*prompt;
	char	*user;
	char	*aux;
	
	prompt = NULL;
	user = NULL;
	aux = ft_getenv("USER", m_env);
	user = get_user(aux);
	free(aux);
	prompt = getcwd(prompt, 0);
	if (!prompt)
		return(user);
	prompt = ft_strattach(PREFIX, &prompt);
	prompt = ft_strappend(&prompt, SUFFIX);
	prompt = ft_strattach(user,&prompt);
	free(user);
	return (prompt);
}

void	free_split(char **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i])
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
	char	*line;
	char	*prompt;
	t_dict	m_env;

	prompt = NULL;
	line = NULL;
	if (argc != 1 && argv[0][2] == 'm' && envp)
		return (0);
	cntl_signals();
	if (init_env(envp, &m_env))
		return (1);
	while (1)
	{
		prompt = format_prompt(&m_env);
		line = readline(prompt);
		if (!line)
			return(exit(0), 0);
		if (*line != '\0')
			add_history(line);
		if (!ft_strncmp(line, "env", 3))
			print_env(&m_env);
		/*Go to export when token data == export || then while (token.nex.flags ==0) --> are the variables to export */
		if (!ft_strncmp(line, "export", 6))
			ft_export(line + 6, &m_env);
		if (!ft_strncmp(line, "unset", 5))
			ft_unset(line + 6, &m_env);
		if (!ft_strncmp(line, "pwd", 3))
			print_pwd(&m_env);
		//token_loop(ft_split(line, ' '));
		free(line);
		free(prompt);
	}
}
