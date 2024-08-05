/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 14:16:50 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/05 20:33:41 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_err_aux(int err, char *m)
{
	if (err == ERR_ARGS)
	{
		ft_printf(STDERR_FILENO, "minishell: invalid number of arguments\n");
		return (102);
	}
	else if (err == ERR_OUTF)
	{
		ft_printf(STDERR_FILENO, "minishell: error creating outfile: %s\n", m);
		return (73);
	}
	else if (err == ERR_COMMANDNF)
	{
		ft_printf(STDERR_FILENO, "minishell: command not found: %s\n", m);
		return (127);
	}
	else if (err == ERR_FAILEDEXE)
	{
		ft_printf(STDERR_FILENO, "minishell: command execution failed: %s\n", m);
		return (126);
	}
	else
	{
		perror("minishell");
		return (1);
	}
}

int	exec_err(int err, char *msg)
{
	if (err == ERR_MEM || err == ERR_PERM || err == ERR_NOFILE)
	{
		if (err == ERR_MEM)
			ft_printf(STDERR_FILENO, "minishell:%s cannot allocate memory\n", msg);
		else if (err == ERR_PERM)
			ft_printf(STDERR_FILENO, "minishell: permission denied: %s\n", msg);
		else if (err == ERR_NOFILE)
			ft_printf(STDERR_FILENO, "minishell: no such file or dir: %s\n", msg);
		return (1);
	}
	else
		return (exec_err_aux(err, msg));
}

void	close_pipes(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (arr)
	{
		while (arr[i])
		{
			free(arr[i]);
			arr[i] = NULL;
			i++;
		}
		free(arr);
	}
}

int	check_path(char *path)
{
	int	i;

	i = 0;
	if (path)
	{
		while (path[i])
		{
			if (path[i] == '/')
			{
				return (1);
			}
			i++;
		}
		return (0);
	}
	return (0);
}

char	*get_path(char *cmd_name, char **all_paths)
{
	int		i;
	char	*temp;
	char	*cmd;

	i = 0;
	temp = ft_strjoin("/", cmd_name);
	while (all_paths && all_paths[i] && temp)
	{
		cmd = ft_strjoin(all_paths[i], temp);
		if ((!access(cmd, F_OK)))
		{
			if (!access(cmd, X_OK))
			{
				free(temp);
				return (cmd);
			}
			else
			{
				exec_err(ERR_FAILEDEXE, cmd);
				return (NULL);
			}
		}
		free(cmd);
		i++;
	}
	if (temp)
		free(temp);
	return (NULL);
}

int	count_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens)
	{
		tokens = tokens->next;
		i++;
	}
	return (i);
}

char	**get_args(t_token *token_args)
{
	int		i;
	int		n_args;
	char	**args;

	i = 0;
	args = NULL;	
	n_args = count_tokens(token_args);
	args = (char **)calloc(n_args + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (token_args && i < n_args)
	{
		args[i] =  ft_strdup(token_args->data);
		token_args = token_args->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

char	**get_all_paths(t_dict *m_env)
 {
	char	*env_path;
	char	**res;

	env_path = ft_getenv("PATH", m_env);
	if (env_path)
	{
		res = ft_split(env_path, ':');
		free(env_path);
	}
	else
		res = (char **)calloc(1, sizeof(char *));
	return (res);
 }

int	format_cmd(t_cmd *cmd, t_dict *m_env, t_token *tokens, int *stat)
{
	char	**all_paths;

	if (!tokens || !tokens->data)
		return (1);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->name = ft_strdup(tokens->data);
	cmd->envp = fetch_env(m_env);
	all_paths = get_all_paths(m_env);
	if (check_path(cmd->name))
		cmd->path = ft_strdup(cmd->name);
	else
		cmd->path = get_path(cmd->name, all_paths);
	if (!cmd->path)
	{
		free_arr(all_paths);
		*stat = exec_err(ERR_COMMANDNF, cmd->name);
		return (1);
	}
	cmd->args = get_args(tokens);
	free_arr(all_paths);
	return (0);
}
void	free_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->path)
		{
			free(cmd->path);
			cmd->path = NULL;
		}
		if (cmd->name)
		{
			free(cmd->name);
			cmd->name = NULL;
		}
		free_arr(cmd->args);
		free_arr(cmd->envp);
	}	
}

void	do_redirection(t_process *process, t_cmd *cmd, t_token *target, int mode)
{

	if (pipe(cmd->pipe))
		process->stat = exec_err(ERR_STD, NULL);
	if (mode == I_REDIRECT)
	{
		cmd->in_file = open(target->data, O_RDONLY);
		if (cmd->in_file < 0)
		{
			if (access(target->data, F_OK))
				process->stat = (exec_err(ERR_NOFILE, target->data));
			else if (access(target->data, R_OK))
				process->stat = (exec_err(ERR_PERM, target->data));
			else
				process->stat = (exec_err(ERR_STD, NULL));
		}
		printf("INPUTTT\n");
		if (dup2(cmd->in_file, STDIN_FILENO))
			process->stat = exec_err(ERR_STD, NULL);
	}
	else if (mode == O_REDIRECT)
	{
		cmd->out_file = open(target->data, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
		if (cmd->out_file < 0)
			process->stat = (exec_err(ERR_OUTF, target->data));
		printf("OUTTPUTTT\n");
		if (dup2(cmd->out_file, STDOUT_FILENO))
			process->stat = exec_err(ERR_STD, NULL);
	}
}
/*Maybe just save the files and do th redirs in-child || to avoid going back to stdin/stdout etc...
		Fix multiple redirssssssssss*/
t_token	*ft_redirect(t_process *process, t_cmd *cmd, t_token *cmd_list)
{
	t_token	*aux;
	t_token	*temp;
	t_token	*redir_cmd;
	int		mode;

	(void)process;
	(void)cmd;
	mode = 0;
	redir_cmd = cmd_list;
	while (cmd_list)
	{
		printf("WHILE: %s\n", cmd_list->data);
		if (cmd_list->next && (cmd_list->next->flags == I_REDIRECT || cmd_list->next->flags == O_REDIRECT))
		{
			temp = cmd_list;
			cmd_list = cmd_list->next;
			if (cmd_list->next)
			{
				if (cmd_list->flags == O_REDIRECT)
					mode = O_REDIRECT;
				else if (cmd_list->flags == I_REDIRECT)
					mode = I_REDIRECT;
				cmd_list = cmd_list->next;
				printf("do redir %s\n", cmd_list->data);
				do_redirection(process, cmd, cmd_list, mode);
				aux = cmd_list->next;
				cmd_list->next = NULL;
				cmd_list = aux;
			}
			else
			{
				ft_printf(2, "minishell: syntax error near unexpected token\n");
				break;
			}
			free_list(temp->next);
			temp->next = cmd_list;
		}
		cmd_list = cmd_list->next;
	}
	return (redir_cmd);
}
/*First built do not do it in-child // otherwise cd etc doesnt work*/
int	ft_executor(t_process *process)
{
	int		i;
	t_cmd	cmd;
	pid_t	child;
	t_token	*cmd_tokens;

	i = -1;
	if (!process->cmd_list)
		return (-1);
	while (++i < (process->n_pipes + 1))
	{
		ft_printf(2, "exec\n");
		cmd_tokens = ft_redirect(process, &cmd, process->cmd_list[i]);
		close_pipes(cmd.pipe);
		print_token_list(cmd_tokens);
		child = fork();
		if (child < 0)
			process->stat = exec_err(ERR_MEM, " fork:");
		else if (child == 0)
		{
			if (check_built_in(cmd_tokens, process->m_env, &process->stat) != 99)
			{
				ft_printf(2, "cont 1\n");
				exit (process->stat);
			}
			if (format_cmd(&cmd, process->m_env, cmd_tokens, &process->stat))
			{
				ft_printf(2, "cont 2 %i\n");
				free_cmd(&cmd);
				exit (process->stat);
			}			
			execve(cmd.path, cmd.args, cmd.envp);
			free_cmd(&cmd);
			process->stat = exec_err(ERR_STD, NULL);
			exit (process->stat);
		}
		waitpid(child, &process->stat, 0);
		dup2(process->og_fd[0], STDIN_FILENO);
		dup2(process->og_fd[1], STDOUT_FILENO);
	}
	return (0);
}
