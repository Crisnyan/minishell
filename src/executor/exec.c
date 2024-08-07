/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 14:16:50 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/07 19:41:53 by vperez-f         ###   ########.fr       */
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
	t_token	*previous;
	t_token	*redir_cmd;
	int		mode;

	(void)process;
	(void)cmd;
	mode = 0;
	redir_cmd = cmd_list;
	previous = NULL;
	while (cmd_list)
	{
		if (cmd_list->flags == I_REDIRECT || cmd_list->flags == O_REDIRECT)
		{
			temp = cmd_list;
			cmd_list = cmd_list->next;
			if (cmd_list)
			{
				if (temp->flags == O_REDIRECT)
					mode = O_REDIRECT;
				else if (temp->flags == I_REDIRECT)
					mode = I_REDIRECT;
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
			if (previous != NULL)
				previous->next = cmd_list;
			else
				redir_cmd = cmd_list;
			free_list(temp);
			continue;
		}
		previous = cmd_list;
		cmd_list = cmd_list->next;
	}
	return (redir_cmd);
}

int	ghetto_builtin(t_token *cmd_tokens)
{
	int	res;

	res = 0;
	while (cmd_tokens)
	{
		if (!ft_strcmp(cmd_tokens->data, "pwd")
			|| !ft_strcmp(cmd_tokens->data, "cd")
			|| !ft_strcmp(cmd_tokens->data, "env")
			|| !ft_strcmp(cmd_tokens->data, "export")
			|| !ft_strcmp(cmd_tokens->data, "unset"))
		{
			res = 1;
		}
		cmd_tokens = cmd_tokens->next;
	}
	return (res);
}

int	exec_no_pipes(t_process *process)
{
	t_cmd	cmd;
	pid_t	child;
	t_token	*cmd_tokens;
	
	if (ghetto_builtin(process->cmd_list[0]))
	{
		ft_printf(2, "BUILTIN PARENT\n");
		cmd_tokens = ft_redirect(process, &cmd, process->cmd_list[0]);
		check_built_in(cmd_tokens, process);
		close(cmd.in_file);
		close(cmd.out_file);
		dup2(process->og_fd[0], STDIN_FILENO);
		dup2(process->og_fd[1], STDOUT_FILENO);
		return (0);
	}
	else
	{
		child = fork();
		if (child < 0)
			return (-1);
		else if (child == 0)
		{
			cmd_tokens = ft_redirect(process, &cmd, process->cmd_list[0]);
			print_token_list(cmd_tokens);
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
	}
	return (0);
}

int	wait_child_processes(pid_t *childs, int amount)
{
	pid_t	pid;
	int		i;
	int		tmp;
	int		status;

	i = 0;
	status = 1;
	while (i < amount)
	{
		pid = waitpid(-1, &tmp, 0);
		printf("CHILD: %i\n", pid);
		if (pid < 0)
			printf("ERROR\n");
		if (pid == childs[amount - 1] && WIFEXITED(tmp))
		{
			status = WEXITSTATUS(tmp);
			printf("HUEHUE\n");
		}
		i++;
	}
	return (status);
}

int	exec_pipes(t_process *process)
{
	int		i;
	t_cmd	cmd;
	pid_t	*child;
	t_token	*cmd_tokens;

	i = -1;
	if (pipe(process->pipe))
		process->stat = exec_err(ERR_STD, NULL);
	child = (pid_t *)malloc((process->n_pipes + 1) * sizeof(pid_t));
	while (++i < (process->n_pipes + 1))
	{
		ft_printf(2, "exec %i\n", i);
		child[i] = fork();
		if (child[i] < 0)
			process->stat = exec_err(ERR_MEM, " fork:");
		else if (child[i] == 0)
		{
			if (i != 0)
			{
				if (dup2(process->pipe[0], STDIN_FILENO) < 0)
				{
					free_cmd(&cmd);
					exit(exec_err(ERR_STD, NULL));
				}
			}
			if (i != process->n_pipes)
			{
				if (dup2(process->pipe[1], STDOUT_FILENO) < 0)
				{
					free_cmd(&cmd);
					exit(exec_err(ERR_STD, NULL));
				}
			}
			cmd_tokens = ft_redirect(process, &cmd, process->cmd_list[i]);
			if (check_built_in(cmd_tokens, process) != 99)
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
			close_pipes(process->pipe);
			execve(cmd.path, cmd.args, cmd.envp);
			free_cmd(&cmd);
			process->stat = exec_err(ERR_STD, NULL);
			exit (process->stat);
		}
		printf("P|| Child %i: ID: %i\n", i, child[i]);
		waitpid(child[i], &process->stat, 0);
	}
	//process->stat = wait_child_processes(child, process->n_pipes + 1);
	free(child);
	return (0);
}

int	ft_executor(t_process *process)
{
	if (!process->cmd_list)
		return (-1);
	if (!process->n_pipes)
	{
		exec_no_pipes(process);
		return (0);
	}
	else
	{
		exec_pipes(process);
		return (0);
	}
	return (0);
}
