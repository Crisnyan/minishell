/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:19:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/13 20:30:21 by vperez-f         ###   ########.fr       */
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
		ft_printf(STDOUT_FILENO, "\n");
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
	if (cmd_list)
	{
		while (i < (n + 1))
		{
			if (cmd_list[i])
				free_list(cmd_list[i]);
			i++;
		}
		free(cmd_list);
	}
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

void	init_process(t_process *process, t_dict *m_env)
{
	process->stat = 0;
	process->heredoc_count = 0;
	process->m_env = m_env;
	process->cmd_list = NULL;
	process->og_fd[0] = dup(0);
	process->og_fd[1] = dup(1);	
}

int	parse(t_token *token, t_process *process)
{
	int		heredoc_count;
	t_token *temp;

	heredoc_count = 0;
	temp = token;
	while (temp)
	{
		if ((temp->flags == PIPE || temp->flags == I_REDIRECT || temp->flags == O_REDIRECT || temp->flags == HEREDOC || temp->flags == O_APPEND)
			&& (!temp->next || temp->next->flags == PIPE || temp->next->flags == I_REDIRECT || temp->next->flags == O_REDIRECT || temp->next->flags == HEREDOC || temp->next->flags == O_APPEND))
			{
				ft_printf(2, "minishell: syntax error near unexpected token '%s'\n", temp->data);
				process->stat = 2;
				return(2);
			}
		else if (temp->flags == HEREDOC)
			heredoc_count++;
		temp = temp->next;
	}
	if (heredoc_count > 15)
	{
		ft_printf(2, "minishell: maximum here-document count exceeded\n");
		process->stat = 2;
		return (2);
	}
	return (0);
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
	init_process(&process, &m_env);
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
		if (parse(tok, &process))
		{
			free_list(tok);
			free(line);
			free(prompt);
			continue;
		}
		process.n_pipes = count_pipes(tok);
		process.cmd_list = split_cmd(tok, process.n_pipes);
		ft_executor(&process);
		free_cmd_list(process.cmd_list, process.n_pipes);
		printf("------%i\n", process.stat);
		free(line);
		free(prompt);
	}
}
