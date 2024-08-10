/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:52:40 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 20:01:58 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	search_index(char *key, t_dict *dict)
{
	int	hash_index;

	if (!key || *key == '\0')
		return (-1);
	hash_index = hash(key) % dict->cap;
	if (0 < dict->current)
	{
		if ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
		{
			while ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
				hash_index = (hash_index + 1) % dict->cap;
		}
		if (dict->entries[hash_index].key)
			return (hash_index);
		else
			return (-1);
	}
	return (-1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*ts1;
	unsigned char	*ts2;

	i = 0;
	ts1 = (unsigned char *)s1;
	ts2 = (unsigned char *)s2;
	while ((ts1[i] != '\0') && (ts2[i] != '\0'))
	{
		if (ts1[i] != ts2[i])
		{
			return (ts1[i] - ts2[i]);
		}
		i++;
	}
	return (ts1[i] - ts2[i]);
}

void	free_entries(t_entry *entries, int n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < n)
	{
		if (entries[j].key)
		{
			free(entries[j].key);
			i++;
		}
		if (entries[j].value)
			free(entries[j].value);
		j++;
	}
	free(entries);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

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
