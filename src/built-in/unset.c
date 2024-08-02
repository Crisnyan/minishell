/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:48:07 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 18:03:27 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_entry(t_entry *entry)
{
	if (entry->key)
	{
		free(entry->key);
		entry->key = NULL;
	}
	if (entry->value)
	{
		free(entry->value);
		entry->value = NULL;
	}
}

int	ft_unset(char *line, t_dict *dict)
{
	int	env_index;

	while (*line == ' ')
		line++;
	env_index = search_index(line, dict);
	if (env_index == -1)
		return (0);
	free_entry(&dict->entries[env_index]);
	dict->current--;
	return (0);
}