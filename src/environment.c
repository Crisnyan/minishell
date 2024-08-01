/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:37:01 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/01 20:48:14 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_export(t_dict *dict)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!dict->current)
		return ;
	while(i < dict->current)
	{
		if (dict->entries[j].key && dict->entries[j].value)
		{
			printf("%s%s=\"%s\"\n", EXPORT_PREFIX, dict->entries[j].key, dict->entries[j].value);
			i++;
		}
		else if (dict->entries[j].key && !dict->entries[j].value)
		{
			printf("%s%s\n", EXPORT_PREFIX, dict->entries[j].key);
			i++;
		}
		j++;
	}
}

void	print_env(t_dict *dict)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!dict->current)
		return ;
	while(i < dict->current)
	{
		if (dict->entries[j].key)
		{
			if (!dict->entries[j].is_export)
				printf("%s=%s\n", dict->entries[j].key, dict->entries[j].value);
			i++;
		}
		j++;
	}
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
void	free_entry(t_entry *entry)
{
	if (entry->key)
		free(entry->key);
	if (entry->value)
		free(entry->value);
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

char	**split_env(char *line)
{
	size_t	i;
	char	**res;

	i = 0;
	res = (char **)malloc(sizeof(char *) * 2);
	if (!res || !line)
		return (NULL);
	while (line[i] && line[i] != '=')
		i++;
	if (i == strlen(line))
	{
		res[0] = ft_strdup(line);
		res[1] = NULL;
	}
	else
	{
		res[0] = ft_substr(line, 0, i);
		res[1] = ft_substr(line, (i + 1), strlen(line) - i);
	}
	return (res);
}

int		search_index(char *key, t_dict *dict)
{
	int	hash_index;

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

void	export_error(char *line)
{
	ft_printf(2, EXPORT_ERROR, line);
}

int	check_args_export(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return(1);
	if (!ft_isalpha(arg[i]))
		return (2);
	i++;
	while (arg && arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '=')
			return (2);
		i++;
	}
	return (0);
}

int	ft_export(char *line, t_dict *dict)
{
	int		export;
	char	**variable;

	variable = NULL;
	export = 0;
	if (line && line[0] == '\0')
	{
		print_export(dict);
		return (0);
	}
	while (*line == ' ')
		line++;
	if (check_args_export(line))
	{
		export_error(line);
		return (1);
	}
	variable = split_env(line);
	if (!variable[1])
		export = 1;
	add_word(variable[0], variable[1], dict, export);
	free(variable[0]);
	free(variable[1]);
	free(variable);
	return (0);
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

char	**fetch_env(t_dict *dict)
{
	int		i;
	int		j;
	char	*aux;
	char	**res;

	i = 0;
	j = 0;
	aux = NULL;
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

void	print_pwd(t_dict *m_env)
{
	char	*pwd;

	pwd = ft_getenv("PWD", m_env);
	if (!pwd)
		return ;
	ft_printf(1, "%s\n", pwd);
	free(pwd);
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
