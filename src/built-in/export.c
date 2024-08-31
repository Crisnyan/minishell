/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:43:25 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/29 17:16:03 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	check_args_export(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[0])
		return(-1);
	if (!ft_isalpha(arg[i]))
		return (-1);
	i++;
	while (arg[i] && arg[i] != '+' && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]))
			return (-1);
		i++;
	}
	if (arg[i] == '+')
	{
		i++;
		if (arg[i] && arg[i] == '=')
			return (1);
		else
			return (-1);
	}
	return (0);
}

void	append_word(char *key, char *value, t_dict *dict, int export)
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
			if (dict->entries[hash_index].value )
				dict->entries[hash_index].value = ft_strappend(&dict->entries[hash_index].value, value);
			else
				dict->entries[hash_index].value = ft_strdup(value);
		}
	}
	else
	{
		dict->entries[hash_index] = create_entry(key, value, export);
		dict->current++;
	}
}

char	**split_env_append(char *line)
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
		res[0] = ft_substr(line, 0, i - 1);
		res[1] = ft_substr(line, (i + 1), ft_strlen(line) - i);
	}
	return (res);
}

void	update_env(char **variable, t_dict *dict, int export, int append)
{
	if (append == 1)
		append_word(variable[0], variable[1], dict, export);
	else
		add_word(variable[0], variable[1], dict, export);
}

int	ft_export(char *line, t_dict *dict, int mode)
{
	int		export;
	int		check_args;
	char	**variable;

	variable = NULL;
	export = 0;
	if (mode == 1)
		return (print_export(dict), 0);
	check_args = check_args_export(line);
	if (check_args < 0)
		return (ft_printf(2, EXPORT_ERROR, line), 1);
	else if (check_args == 1)
		variable = split_env_append(line);	
	else
		variable = split_env(line);
	if (!variable[1])
		export = 1;
	update_env(variable, dict, export, check_args);
	free(variable[0]);
	free(variable[1]);
	free(variable);
	return (0);
}
