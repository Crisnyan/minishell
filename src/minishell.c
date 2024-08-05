/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:19:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/05 20:05:05 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	
	user = NULL;
	aux = ft_getenv("USER", m_env);
	user = get_user(aux);
	free(aux);
	prompt = getcwd(NULL, 0);
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

int	count_pipes(t_token *list)
{
	int	pipes;
	t_token	*temp;

	pipes = 0;
	temp = list;
	while (temp)
	{
		if (temp->flags == PIPE)
			pipes++;
		temp = temp->next;
	}
	return (pipes);
}

void	free_cmd_list(t_token **cmd_list, int n)
{
	int	i;

	i = 0;
	while (i < (n + 1))
	{
		if (cmd_list[i])
			free_list(cmd_list[i]);
		i++;
	}
	free(cmd_list);
}

t_token	**split_cmd(t_token *tokens, int pipes)
{
	int	i;
	t_token	*temp;
	t_token	**cmd_list;

	i = 0;
	cmd_list = (t_token **)calloc(pipes + 1, sizeof(t_token *));
	if (!cmd_list)
		return (NULL);
	while (i < (pipes + 1) && tokens)
	{
		cmd_list[i] = tokens;
		while (tokens)
		{
			if (tokens->next && tokens->next->flags == PIPE)
			{
				temp = tokens->next;
				tokens->next = NULL;
				tokens = temp->next;
				temp->next = NULL;
				free_list(temp);
				break;
			}
			tokens = tokens->next;
		}
		i++;
	}
	return (cmd_list);
}

void	print_list(t_token **list, int n)
{
	int	i;

	i = 0;
	while (i < n + 1)
	{
		if (list && list[i])
			print_token_list(list[i]);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	char		*prompt;
	t_token		*tok;
	t_dict		m_env;
	t_process	process;

	prompt = NULL;
	line = NULL;
	if (argc != 1 && argv[0][2] == 'm' && envp)
		return (0);
	cntl_signals();
	if (init_env(envp, &m_env))
		return (1);
	process.m_env = &m_env;
	process.og_fd[0] = dup(0);
	process.og_fd[1] = dup(1);
	while (1)
	{
		prompt = format_prompt(&m_env);
		line = readline(prompt);
		if (!line)
			return(exit(0), 0);
		if (*line != '\0')
			add_history(line);
		if (*line == '\0')
		{
			free(line);
			free(prompt);
			continue;
		}
		tok = expansor(minishplit(line), &m_env);
		process.n_pipes = count_pipes(tok);
		process.cmd_list = split_cmd(tok, process.n_pipes);
		//print_list(process.cmd_list, process.n_pipes);
		ft_executor(&process);
		printf("------%i\n", process.stat);
		//print_token_list(tok);
		free_cmd_list(process.cmd_list, process.n_pipes);
		free(line);
		free(prompt);
	}
}
