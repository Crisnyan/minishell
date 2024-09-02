/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:41:03 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/02 18:34:02 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static	int	cd_no_path(char *path, char *old_pwd, t_dict *m_env)
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
		free(path);
		return (1);
	}
	add_word("PWD", path, m_env, 0);
	add_word("OLDPWD", old_pwd, m_env, 0);
	free(path);
	return (0);
}

static	int	cd_path(char *path, char *old_pwd, t_dict *m_env)
{
	if (chdir(path))
	{
		if (access(path, F_OK))
			ft_printf(2, CD_ERROR_DIR, path);
		else if (access(path, X_OK))
			ft_printf(2, CD_ERROR_PERM, path);
		else
			perror("cd");
		return (1);
	}
	path = getcwd(NULL, 0);
	add_word("PWD", path, m_env, 0);
	add_word("OLDPWD", old_pwd, m_env, 0);
	free(path);
	return (0);
}

int	ft_cd(char *path, t_dict *m_env)
{
	int		err;
	char	*old_pwd;

	err = 0;
	old_pwd = getcwd(NULL, 0);
	if (!path)
	{
		err = cd_no_path(path, old_pwd, m_env);
		free(old_pwd);
		return (err);
	}
	else
	{
		err = cd_path(path, old_pwd, m_env);
		free(old_pwd);
		return (err);
	}
}
