/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:43:25 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/02 21:12:43 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_and_append(int indx, char *value, t_dict *d, int export)
{
	if (d->entries[indx].value)
		d->entries[indx].value = ft_strappend(&d->entries[indx].value, value);
	else
	{
		d->entries[indx].value = ft_strdup(value);
		if (d->entries[indx].value)
			d->entries[indx].is_export = export;
	}
}

void	append_word(char *key, char *value, t_dict *d, int export)
{
	int	indx;

	indx = hash(key) % d->cap;
	if (0 < d->current)
	{
		while (d->entries[indx].is_tombstone
			|| (d->entries[indx].key && ft_strcmp(d->entries[indx].key, key)))
			indx = (indx + 1) % d->cap;
		if (!(d->entries[indx].key))
		{
			d->entries[indx] = create_entry(key, value, export);
			d->current++;
			if (d->current == (d->cap * 3 / 4))
				expand_dict(d);
		}
		else if (value)
			check_and_append(indx, value, d, export);
	}
	else
	{
		d->entries[indx] = create_entry(key, value, export);
		d->current++;
	}
}
