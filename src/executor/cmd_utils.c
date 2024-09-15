/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:54:37 by vpf               #+#    #+#             */
/*   Updated: 2024/09/15 20:19:49 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		args[i] = ft_strdup(token_args->data);
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
	if (!tokens->data[0])
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
