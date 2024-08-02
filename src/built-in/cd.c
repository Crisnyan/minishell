/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:41:03 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 17:59:39 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(char *path)
{
	int	i;

	i = 0;
	if (!path)
		return (0);
	while (*path == ' ')
		path++;
	if (chdir(path))
	{
		ft_printf(2, CD_ERROR, path);
		return (1);
	}
	return (0);
}
