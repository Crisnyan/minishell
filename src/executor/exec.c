/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 14:16:50 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/03 14:17:34 by vperez-f         ###   ########.fr       */
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
				exit(exec_err(ERR_FAILEDEXE, cmd));
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

int	ft_executor(t_process *process)
{
	int		i;
	t_cmd	cmd;
	pid_t	child;

	i = -1;
	if (!process->cmd_list)
		return (-1);
	while (++i < (process->n_pipes + 1))		
	{
		ft_printf(2, "exec\n");
		if (check_built_in(process->cmd_list[i], process->m_env, &process->stat) != 99)
		{
			ft_printf(2, "cont 1\n");
			continue;
		}
		if (format_cmd(&cmd, process->m_env, process->cmd_list[i], &process->stat))
		{
			ft_printf(2, "cont 2\n");
			free_cmd(&cmd);
			continue;
		}
		child = fork();
		if (child < 0)
			process->stat = exec_err(ERR_MEM, " fork:");
		else if (child == 0)
		{
			execve(cmd.path, cmd.args, cmd.envp);
			exec_err(ERR_STD, NULL);
			return (0);
		}
		waitpid(child, &process->stat, 0);
		free_cmd(&cmd);
	}
	return (0);
}
