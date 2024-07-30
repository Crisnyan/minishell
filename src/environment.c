/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:37:01 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/30 21:00:51 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_export(t_dict *dict)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!dict->init)
		return ;
	printf("|====================[-+-]======================|\n");
	printf("     Entry cap --> %i | %i <-- Current words\n", dict->cap, dict->current);
	printf("           		   EXPORT\n");	
	printf("|-----------------------------------------------|\n");
	while(i < dict->current)
	{
		if (dict->entries[j].key)
		{
			if (dict->entries[j].is_export == 1)
				printf("%s%s%s\n", EXPORT, dict->entries[j].key, dict->entries[j].value);
			i++;
		}
		j++;
	}
	printf("|====================[---]======================|\n");
}

void	print_env(t_dict *dict)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!dict->init)
		return ;
	printf("|====================[-+-]======================|\n");
	printf("     Entry cap --> %i | %i <-- Current words\n", dict->cap, dict->current);
	printf("        		ENVIRONMENT\n");
	printf("|-----------------------------------------------|\n");
	while(i < dict->current)
	{
		if (dict->entries[j].key)
		{
			if (dict->entries[j].is_export == 0)			
				printf("%s%s\n", dict->entries[j].key, dict->entries[j].value);
			i++;
		}
		j++;
	}
	printf("|====================[---]======================|\n");
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
	entry.value = ft_strdup(value);
	entry.is_export = export;
	return (entry);
}

void	re_addword(t_entry *temp, t_entry *old, int cap)
{
	int	hash_index;

	hash_index = hash(old->key) % cap;
	//printf("hash: %i --- hash_index: %i\n", hash(key), hash_index);
	if ((temp[hash_index].key) && ft_strcmp((temp[hash_index].key), old->key))
	{
		//printf(" !! Collisao on hash_index: %i\n", hash_index);

		while ((temp[hash_index].key) && ft_strcmp((temp[hash_index].key), old->key))
			hash_index = (hash_index + 1) % cap;
		//printf(" !! New index after collisao: %i\n", hash_index);
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
			//printf("Word from dict: %s --> %s\n", dict->entries[j].key, dict->entries[j].value);
		}
		j++;
	}
	free_entries(dict->entries, dict->current);
	dict->entries = temp;
	//free(temp);
}

void	add_word(char *key, char *value, t_dict *dict, int export)
{
	int	hash_index;

	hash_index = hash(key) % dict->cap;
	//printf("KEY: %s || hash: %i --- hash_index: %i\n", key, hash(key), hash_index);
	if ((dict->current) > 0)
	{
		if ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
		{
			//printf(" !! Collisao on hash_index: %i\n", hash_index);
			while ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
			{
				//printf("Not yet...\n");
				hash_index = (hash_index + 1) % dict->cap;
			}
			//printf(" !! New index after collisao: %i\n", hash_index);
			
		}
		if (!(dict->entries[hash_index].key))
		{
			dict->entries[hash_index] = create_entry(key, value, export);
			dict->current++;
			if (dict->current == (dict->cap * 3 / 4))
			{
				//printf("holasdad EXP\n");
				expand_dict(dict);
			}
		}
	}
	else
	{
		dict->entries[hash_index] = create_entry(key, value, export);
		dict->current++;
	}
}

char	**split_env(char *line)
{
	int		i;
	char	**res;

	i = 0;
	res = (char **)malloc(sizeof(char *) * 2);
	if (!line)
		return (NULL);
	while (line[i] && line[i] != '=')
		i++;
	res[0] = ft_substr(line, 0, i);
	res[1] = ft_substr(line, i, strlen(line) - i);
	return (res);
}

int	init_env(char **envp, t_dict *dict)
{
	int		i;
	int		export;
	char	**test;

	i = 0;
	export = 0;
	dict->cap = 4;
	dict->entries = (t_entry *)calloc(dict->cap, sizeof(t_entry));
	dict->current = 0;
	while (envp && envp[i])
	{
		test = split_env(envp[i]);
		add_word(test[0], test[1], dict, export);
		export = (export + 1) % 2;
		free(test[0]);
		free(test[1]);
		free(test);
		i++;
	}
	dict->init = 1;
	return (0);
}
