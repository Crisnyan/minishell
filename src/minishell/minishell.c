/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 12:43:05 by cristian          #+#    #+#             */
/*   Updated: 2024/09/16 15:19:07 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	g_global_signal;

void	quote_error(char **str, t_process *process)
{
	ft_printf(STDERR_FILENO, ERR_QUOTES);
	process->m_env->err_code = 2;
	g_global_signal = 0;
	free(str[LINE]);
	free(str[PROMPT]);
}

int	parse_error(char **str, t_token *tok)
{
	g_global_signal = 0;
	free_list(tok);
	free(str[LINE]);
	free(str[PROMPT]);
	return (1);
}

void	process_pipes(char **str, t_token *tok, t_process *process)
{
	process->n_pipes = count_pipes(tok);
	process->cmd_list = split_cmd(tok, process->n_pipes);
	ft_executor(process);
	free_cmd_list(process->cmd_list, process->n_pipes);
	g_global_signal = 0;
	free(str[LINE]);
	free(str[PROMPT]);
}

static int	continue_loop(char **str, t_process *proc)
{
	if (!str[LINE])
	{
		if (isatty(STDIN_FILENO))
			ft_printf(STDERR_FILENO, "exit\n");
		exit(proc->m_env->err_code);
	}
	if (*str[LINE] != '\0')
		add_history(str[LINE]);
	if (*str[LINE] == '\0')
	{
		free(str[LINE]);
		free(str[PROMPT]);
		return (1);
	}
	if (count_quotes(str[LINE]) % 2)
	{
		quote_error(str, proc);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char		*str[2];
	t_token		*tok;
	t_dict		m_env;
	t_process	process;

	tok = null_var(str);
	if (argc != 1 && envp)
		return (0);
	if (init_env(envp, &m_env))
		return (1);
	init_process(&process, &m_env);
	while (1)
	{
		sig_and_line(str, m_env);
		if (g_global_signal)
			process.m_env->err_code = g_global_signal;
		if (continue_loop(str, &process))
			continue ;
		tok = expansor(minishplit(str[LINE]), &m_env);
		if (parse(tok, &process) && parse_error(str, tok))
			continue ;
		process_pipes(str, tok, &process);
	}
	free_entries(m_env.entries, m_env.current);
	return ((void)argv, process.m_env->err_code);
}
