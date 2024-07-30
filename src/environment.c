/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:37:01 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/30 20:10:39 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    print_dict(t_dict *dict, int col_num)
{
	int i;
	int j;

	i = 0;
	j = 0;
	printf("|====================[-+-]======================|\n");
	printf("     Entry cap --> %i | %i <-- Current words\n", dict->cap, dict->current);
	printf("         Final number of collisions: %03i\n", col_num);
	printf("|-----------------------------------------------|\n");
	while(i < dict->current)
	{
		if (dict->entries[j].key)
		{
			printf("        (%04i) | %-12s -----> %2s\n", j, dict->entries[j].key, dict->entries[j].value);
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

t_entry create_entry(char *key, char *value)
{
	t_entry	entry;

	entry.key = ft_strdup(key);
	entry.value = ft_strdup(value);
	return (entry);
}

void	re_addword(t_entry *temp, char *key, char *value, int cap, int *col_num)
{
	int	hash_index;

	hash_index = hash(key) % cap;
	//printf("hash: %i --- hash_index: %i\n", hash(key), hash_index);
	if ((temp[hash_index].key) && ft_strcmp((temp[hash_index].key), key))
	{
		//printf(" !! Collisao on hash_index: %i\n", hash_index);
		*col_num = *col_num + 1;
		while ((temp[hash_index].key) && ft_strcmp((temp[hash_index].key), key))
			hash_index = (hash_index + 1) % cap;
		//printf(" !! New index after collisao: %i\n", hash_index);
	}
	if (!(temp[hash_index].key))
	{
		temp[hash_index] = create_entry(key, value);
	}
}

void	expand_dict(t_dict *dict, int *col_num)
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
			re_addword(temp, dict->entries[j].key, dict->entries[j].value, dict->cap, col_num);
			i++;
			printf("Word from dict: %s --> %s\n", dict->entries[j].key, dict->entries[j].value);
		}
		j++;
	}
	free_entries(dict->entries, dict->current);
	dict->entries = temp;
	//free(temp);
}

void	add_word(char *key, char *value, t_dict *dict, int *col_num)
{
	int	hash_index;

	hash_index = hash(key) % dict->cap;
	printf("KEY: %s || hash: %i --- hash_index: %i\n", key, hash(key), hash_index);
	if ((dict->current) > 0)
	{
		if ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
		{
			printf(" !! Collisao on hash_index: %i\n", hash_index);
			*col_num = *col_num + 1;
			while ((dict->entries[hash_index].key) && ft_strcmp((dict->entries[hash_index].key), key))
			{
				printf("Not yet...\n");
				hash_index = (hash_index + 1) % dict->cap;
			}
			printf(" !! New index after collisao: %i\n", hash_index);
			
		}
		if (!(dict->entries[hash_index].key))
		{
			dict->entries[hash_index] = create_entry(key, value);
			dict->current++;
			if (dict->current == (dict->cap * 3 / 4))
			{
				printf("holasdad EXP\n");
				expand_dict(dict, col_num);
			}
		}
		else
			printf("KEK\n");
	}
	else
	{
		dict->entries[hash_index] = create_entry(key, value);
		dict->current++;
	}
	//printf("---------------------------------------------------\n");
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

int	init_env(char **envp)
{
	int		i;
	int		*col_num;
	t_dict  *dict;
	char	**test;

	i = 0;
	dict = (t_dict *)calloc(1, sizeof(t_dict));
	dict->cap = 4;
	dict->entries = (t_entry *)calloc(dict->cap, sizeof(t_entry));
	dict->current = 0;
	col_num = (int *)malloc(sizeof(int));
	*col_num = 0;
	while (envp && envp[i])
	{
		test = split_env(envp[i]);
		add_word(test[0], test[1], dict, col_num);
		free(test[0]);
		free(test[1]);
		free(test);
		i++;
	}
	print_dict(dict, *col_num);
	return (0);
}

