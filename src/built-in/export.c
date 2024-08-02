/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:43:25 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 19:22:47 by vperez-f         ###   ########.fr       */
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
		return(1);
	if (!ft_isalpha(arg[i]))
		return (2);
	i++;
	while (arg && arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]))
			return (2);
		i++;
	}
	return (0);
}

int	ft_export(char *line, t_dict *dict, int mode)
{
	int		export;
	char	**variable;

	variable = NULL;
	export = 0;
	if (mode == 1)
	{
		print_export(dict);
		return (0);
	}
	if (check_args_export(line))
	{
		ft_printf(2, EXPORT_ERROR, line);
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
