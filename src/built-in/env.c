/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 17:10:18 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 17:59:37 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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