/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:52:40 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/03 15:33:14 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	search_index(char *key, t_dict *d)
{
	int	indx;

	if (!key || *key == '\0')
		return (-1);
	indx = hash(key) % d->cap;
	if (0 < d->current)
	{
		while (d->entries[indx].is_tombstone
			|| (d->entries[indx].key && ft_strcmp(d->entries[indx].key, key)))
			indx = (indx + 1) % d->cap;
		if (d->entries[indx].key && !ft_strcmp((d->entries[indx].key), key))
			return (indx);
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

unsigned int	hash(char *word)
{
	unsigned int	res;
	int				i;
	char			c;

	res = 0;
	i = 0;
	while (word[i])
	{
		c = word[i];
		res += res * 13 + c;
		i++;
	}
	return (res);
}
