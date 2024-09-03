/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:37:01 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/03 15:33:43 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**split_env(char *line)
{
	size_t	i;
	char	**res;

	i = 0;
	if (!line)
		return (NULL);
	res = (char **)malloc(sizeof(char *) * 2);
	if (!res)
		return (NULL);
	while (line[i] && line[i] != '=')
		i++;
	if (i == ft_strlen(line))
	{
		res[0] = ft_strdup(line);
		res[1] = NULL;
	}
	else
	{
		res[0] = ft_substr(line, 0, i);
		res[1] = ft_substr(line, (i + 1), ft_strlen(line) - i);
	}
	return (res);
}

char	*ft_getenv(char *key, t_dict *dict)
{
	int		index;
	char	*res;

	if (!key)
		return (NULL);
	if (!ft_strcmp(key, "$"))
	{
		res = ft_itoa((int)getpid());
		return (res);
	}
	if (!ft_strcmp(key, "?"))
	{
		res = ft_itoa((int)dict->err_code);
		return (res);
	}
	index = search_index(key, dict);
	if (index == -1)
		return (NULL);
	res = ft_strdup(dict->entries[index].value);
	if (!res)
		return (NULL);
	return (res);
}

char	**fetch_env(t_dict *dict)
{
	int		i;
	int		j;
	char	**res;

	i = 0;
	j = 0;
	res = (char **)calloc(dict->current + 1, sizeof(char *));
	if (!res)
		return (NULL);
	while (i < dict->current && j < dict->cap)
	{
		if (dict->entries[j].key && !dict->entries[j].is_export)
		{
			res[i] = ft_strdup(dict->entries[j].key);
			res[i] = ft_strappend(&res[i], "=");
			res[i] = ft_strappend(&res[i], dict->entries[j].value);
			i++;
		}
		j++;
	}
	res[i] = NULL;
	return (res);
}

int	init_env(char **envp, t_dict *dict)
{
	int		i;
	char	**variable;

	i = 0;
	dict->cap = 4;
	dict->current = 0;
	dict->err_code = 0;
	dict->entries = (t_entry *)calloc(dict->cap, sizeof(t_entry));
	if (!dict->entries)
		return (-1);
	while (envp && envp[i])
	{
		variable = split_env(envp[i]);
		if (!variable)
			return (-1);
		add_word(variable[0], variable[1], dict, 0);
		free(variable[0]);
		free(variable[1]);
		free(variable);
		i++;
	}
	return (0);
}
