/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:41:03 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/02 18:15:39 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(char *path, t_dict *m_env)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!path)
	{
		path = ft_getenv("HOME", m_env);
		if (!path)
			return (ft_printf(2, CD_ERROR_SET, "HOME"), 1);
		if (chdir(path))
		{
			if (access(path, F_OK))
				ft_printf(2, CD_ERROR_DIR, path);
			else if (access(path, X_OK))
				ft_printf(2, CD_ERROR_PERM, path);
			else
				perror("cd");
			free(old_pwd);
			free(path);
			return (1);
		}
		add_word("PWD", path, m_env, 0);
		add_word("OLDPWD", old_pwd, m_env, 0);
		free(path);
		free(old_pwd);
		return (0);
	}
	if (chdir(path))
	{
		if (access(path, F_OK))
			ft_printf(2, CD_ERROR_DIR, path);
		else if (access(path, X_OK))
			ft_printf(2, CD_ERROR_PERM, path);
		else
			perror("cd");
		free(old_pwd);
		return (1);
	}
	path = getcwd(NULL, 0);
	add_word("PWD", path, m_env, 0);
	add_word("OLDPWD", old_pwd, m_env, 0);
	free(path);
	free(old_pwd);
	return (0);
}
