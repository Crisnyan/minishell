/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 14:16:50 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/14 20:34:45 by vperez-f         ###   ########.fr       */
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
	else if(err == ERR_HDMAX)
	{
		ft_printf(STDERR_FILENO, "minishell: maximum here-document count exceeded\n");
		return (2);
	}
	else
		return (exec_err_aux(err, msg));
}

void	close_pipes(int *pipefd)
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
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
		arr = NULL;
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
				return (1);
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
			exec_err(ERR_FAILEDEXE, cmd);
			return (NULL);
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
	while (token_args && token_args->data && i < n_args)
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
	{
		res = (char **)calloc(1, sizeof(char *));
		if (!res)
		{
			exec_err(ERR_MEM, " calloc:");
			return (NULL);
		}
	}
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
	cmd = NULL;
}

void	clean_here_docs(t_process *process)
{
	int		i;
	char	*filename;

	i = 0;
	process->heredoc_count = 0;
	while (i < 16)
	{
		filename = ft_itoa(i + 1);
		filename = ft_strattach("temp/here-doc", &filename);
		if (!access(filename, F_OK))
			unlink(filename);
		free(filename);
		i++;
	}
}

void	heredoc_prompt(t_process *process, char *delim, int fd)
{
	char	*line;
	int		og_stdin;

	og_stdin = dup(process->og_fd[0]);
	if (!isatty(STDIN_FILENO))
	{
		dup2(og_stdin, STDIN_FILENO);
		close(og_stdin);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			return ;
		if (!ft_strcmp(line, delim))
		{
			free(line);
			return ;
		}
		line = ft_strappend(&line, "\n");
		ft_printf(fd, line);
		free(line);
	}
}

void	create_heredocs(t_process *process, t_token *cmd_list)
{

	int		fd;
	char	*filename;
	t_token	*temp;

	fd = 0;
	filename = NULL;
	temp = cmd_list;
	while (temp)
	{
		if (temp->flags == HEREDOC)
		{
			filename = ft_itoa(process->heredoc_count + 1);
			filename = ft_strattach("temp/here-doc", &filename);
			fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (fd < 0)
			{
				if (access(filename, F_OK))
					process->stat = (exec_err(ERR_NOFILE, filename));
				else if (access(filename, R_OK))
					process->stat = (exec_err(ERR_PERM, filename));
				else
					process->stat = (exec_err(ERR_STD, NULL));
				free(filename);
				return ;
			}
			heredoc_prompt(process, temp->next->data, fd);
			close(fd);
			free(filename);
			process->heredoc_count++;
		}
		temp = temp->next;		
	}
}

void	heredoc_redirection(t_process *process)
{
	char	*filename;
	int		fd;

	fd = 0;
	filename = ft_itoa(process->heredoc_count + 1);
	filename = ft_strattach("temp/here-doc", &filename);
	fd = open(filename, O_RDONLY);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		process->stat = exec_err(ERR_STD, NULL);
		close(fd);
		free(filename);
		return ;
	}
	process->heredoc_count++;
	close(fd);
	free(filename);
}

void	input_redirection(t_process *process, t_cmd *cmd, t_token *target)
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
		return ;
	}
	if (dup2(cmd->in_file, STDIN_FILENO) < 0)
	{
		process->stat = exec_err(ERR_STD, NULL);
		close(cmd->in_file);
		return ;
	}
	close(cmd->in_file);	
}

void	append_redirection(t_process *process, t_cmd *cmd, t_token *target)
{
	cmd->out_file = open(target->data, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (cmd->out_file < 0)
	{
		if (access(target->data, R_OK))
			process->stat = (exec_err(ERR_PERM, target->data));
		else
			process->stat = (exec_err(ERR_STD, NULL));
		return ;
	}
	if (dup2(cmd->out_file, STDOUT_FILENO) < 0)
	{
		process->stat = exec_err(ERR_STD, NULL);
		close(cmd->out_file);
		return ;
	}
	close(cmd->out_file);
}

void	output_redirection(t_process *process, t_cmd *cmd, t_token *target)
{
	cmd->out_file = open(target->data, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (cmd->out_file < 0)
	{
		if (access(target->data, R_OK))
			process->stat = (exec_err(ERR_PERM, target->data));
		else
			process->stat = (exec_err(ERR_STD, NULL));
		return ;
	}
	if (dup2(cmd->out_file, STDOUT_FILENO) < 0)
	{
		process->stat = exec_err(ERR_STD, NULL);
		close(cmd->out_file);
		return ;
	}
	close(cmd->out_file);
}

t_token	*ft_redirect(t_process *process, t_cmd *cmd, t_token *cmd_list)
{
	t_token	*aux;
	t_token	*temp;
	t_token	*previous;
	t_token	*redir_cmd;

	redir_cmd = cmd_list;
	previous = NULL;
	process->heredoc_count = 0;
	while (cmd_list)
	{
		if (cmd_list->flags == I_REDIRECT || cmd_list->flags == O_REDIRECT
			|| cmd_list->flags == HEREDOC || cmd_list->flags == APPEND)
		{
			temp = cmd_list;
			cmd_list = cmd_list->next;
			if (cmd_list)
			{
				if (temp->flags == HEREDOC)
					heredoc_redirection(process);
				else if (temp->flags == I_REDIRECT)
					input_redirection(process, cmd, cmd_list);
				else if (temp->flags == O_REDIRECT)
					output_redirection(process, cmd, cmd_list);
				else if (temp->flags == APPEND)
					append_redirection(process, cmd, cmd_list);
				if (process->stat)
					return (redir_cmd);
				aux = cmd_list->next;
				cmd_list->next = NULL;
				cmd_list = aux;
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

int	wait_child_processes(pid_t *childs, int amount)
{
	pid_t	pid;
	int		i;
	int		stat_loc;
	int		status;

	i = 0;
	status = 1;
	while (i < amount)
	{
		pid = waitpid(-1, &stat_loc, 0);
		if (pid < 0)
			exec_err(ERR_STD, "pid:");
		if (pid == childs[amount - 1] && WIFEXITED(stat_loc))
			status = WEXITSTATUS(stat_loc);
		i++;
	}
	return (status);
}

int	exec_no_pipes(t_process *process)
{
	t_cmd	cmd;
	pid_t	child;
	t_token	*cmd_tokens;

	ft_bzero(&cmd, sizeof(t_cmd));
	create_heredocs(process, process->cmd_list[0]);
	cmd_tokens = ft_redirect(process, &cmd, process->cmd_list[0]);
	process->cmd_list[0] = cmd_tokens;
	if (process->stat)
		return (process->stat);
	if (check_built_in(cmd_tokens, process) == 99)
	{
		child = fork();
		if (child < 0)
		{
			process->stat = exec_err(ERR_MEM, " fork:");
			return (process->stat);
		}
		else if (child == 0)
		{
			if (format_cmd(&cmd, process->m_env, cmd_tokens, &process->stat))
			{
				free_cmd(&cmd);
				exit (process->stat);
			}
			execve(cmd.path, cmd.args, cmd.envp);
			free_cmd(&cmd);
			process->stat = exec_err(ERR_STD, NULL);
			exit (process->stat);
		}
		process->stat = wait_child_processes(&child, 1);
	}
	return (process->stat);
}

int	exec_pipes(t_process *process)
{
	int		i;
	t_cmd	cmd;
	pid_t	*child;
	t_token	*cmd_tokens;

	i = -1;
	ft_bzero(&cmd, sizeof(t_cmd));
	child = (pid_t *)malloc((process->n_pipes + 1) * sizeof(pid_t));
	if (!child)
	{
		process->stat = exec_err(ERR_MEM, " malloc:");
		return (process->stat);
	}
	while (++i < (process->n_pipes + 1))
	{
		if (pipe(process->pipe))
		{
			process->stat = exec_err(ERR_STD, NULL);
			break ;
		}
		create_heredocs(process, process->cmd_list[i]);
		child[i] = fork();
		if (child[i] < 0)
		{
			process->stat = exec_err(ERR_MEM, " fork:");
			break ;
		}
		else if (child[i] == 0)
		{
			if (i != process->n_pipes)
			{
				if (dup2(process->pipe[1], STDOUT_FILENO) < 0)
					exit(exec_err(ERR_STD, NULL));
			}
			close_pipes(process->pipe);
			cmd_tokens = ft_redirect(process, &cmd, process->cmd_list[i]);
			if (process->stat)
				exit (process->stat);
			process->cmd_list[i] = cmd_tokens;
			if (check_built_in(cmd_tokens, process) != 99)
				exit (process->stat);
			if (format_cmd(&cmd, process->m_env, cmd_tokens, &process->stat))
			{
				free_cmd(&cmd);
				exit (process->stat);
			}
			execve(cmd.path, cmd.args, cmd.envp);
			free_cmd(&cmd);
			process->stat = exec_err(ERR_STD, NULL);
			exit (process->stat);
		}
		if (dup2(process->pipe[0], STDIN_FILENO) < 0)
			exit(exec_err(ERR_STD, NULL));
		close_pipes(process->pipe);
	}
	process->stat = wait_child_processes(child, process->n_pipes + 1);
	free(child);
	return (process->stat);
}

int	ft_executor(t_process *process)
{
	if (!process->cmd_list)
		return (-1);
	process->stat = 0;
	if (!process->n_pipes)
		exec_no_pipes(process);
	else
		exec_pipes(process);
	clean_here_docs(process);
	dup2(process->og_fd[0], STDIN_FILENO);
	dup2(process->og_fd[1], STDOUT_FILENO);
	return (process->stat);
}
