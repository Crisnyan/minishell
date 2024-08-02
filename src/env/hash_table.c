/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:50:39 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 17:59:43 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	return res;
}

t_entry create_entry(char *key, char *value, int export)
{
	t_entry	entry;

	entry.key = ft_strdup(key);
	if (value)
		entry.value = ft_strdup(value);
	else
		entry.value = NULL;
	entry.is_export = export;
	return (entry);
}

void	re_addword(t_entry *temp, t_entry *old, int cap)
{
	int	hash_index;

	hash_index = hash(old->key) % cap;
	if ((temp[hash_index].key) && ft_strcmp((temp[hash_index].key), old->key))
	{
		while ((temp[hash_index].key) && ft_strcmp((temp[hash_index].key), old->key))
			hash_index = (hash_index + 1) % cap;
	}
	if (!(temp[hash_index].key))
	{
		temp[hash_index] = create_entry(old->key, old->value, old->is_export);
	}
}

void	expand_dict(t_dict *dict)
{
	t_entry	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dict->cap = dict->cap * 2;
	temp =  (t_entry *)calloc(dict->cap, sizeof(t_entry));
	while (i < dict->current)
	{
		if (dict->entries[j].key)
		{
			re_addword(temp, &dict->entries[j], dict->cap);
			i++;
		}
		j++;
	}
	free_entries(dict->entries, dict->current);
	dict->entries = temp;
}

void	add_word(char *key, char *value, t_dict *dict, int export)
{
	int	hash_index;

	hash_index = hash(key) % dict->cap;
	if ((dict->current) > 0)
	{
		if ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
		{
			while ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
				hash_index = (hash_index + 1) % dict->cap;
		}
		if (!(dict->entries[hash_index].key))
		{
			dict->entries[hash_index] = create_entry(key, value, export);
			dict->current++;
			if (dict->current == (dict->cap * 3 / 4))
				expand_dict(dict);
		}
		else if (value)
		{
			free_entry(&dict->entries[hash_index]);
			dict->entries[hash_index] = create_entry(key, value, export);
		}
	}
	else
	{
		dict->entries[hash_index] = create_entry(key, value, export);
		dict->current++;
	}
}
