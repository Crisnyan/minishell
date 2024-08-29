/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:41:03 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/29 17:16:02 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(char *path, t_dict *m_env)
{
	if (!path)
	{
		path = ft_getenv("HOME", m_env);
		if (!path)
			return (ft_printf(2, CD_ERROR_SET, "HOME"), 1);
		if (chdir(path))
		{
			free(path);
			return (perror("cd"), 1);
		}
		free (path);
		return (0);
	}
	if (chdir(path))
	{
		ft_printf(2, CD_ERROR_DIR, path);
		return (1);
	}
	return (0);
}
