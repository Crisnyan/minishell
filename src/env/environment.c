/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:37:01 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 20:07:43 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_getenv(char *key, t_dict *dict)
{
	int		index;
	char	*res;

	if (!key)
		return (NULL);
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
	while(i < dict->current)
	{
		if (dict->entries[j].key && dict->entries[j].value && !dict->entries->is_export)
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
