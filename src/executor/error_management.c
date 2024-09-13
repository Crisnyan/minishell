/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 14:16:50 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/13 21:18:22 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_err_aux(int err, char *msg)
{
	if (err == ERR_ARGS)
	{
		ft_printf(STDERR_FILENO, ERR_ARGS_MSG);
		return (102);
	}
	else if (err == ERR_OUTF)
	{
		ft_printf(STDERR_FILENO, ERR_OUTF_MSG, msg);
		return (73);
	}
	else if (err == ERR_COMMANDNF)
	{
		ft_printf(STDERR_FILENO, ERR_COMMANDNF_MSG, msg);
		return (127);
	}
	else if (err == ERR_FAILEDEXE)
	{
		ft_printf(STDERR_FILENO, ERR_FAILEDEXE_MSG, msg);
		return (126);
	}
	else
	{
		perror("minishell");
		return (1);
	}
}

int	exec_err(int err, char *msg)
{
	if (err == ERR_MEM || err == ERR_PERM || err == ERR_NOFILE)
	{
		if (err == ERR_MEM)
			ft_printf(STDERR_FILENO, ERR_MEM_MSG, msg);
		else if (err == ERR_PERM)
			ft_printf(STDERR_FILENO, ERR_PERM_MSG, msg);
		else if (err == ERR_NOFILE)
			ft_printf(STDERR_FILENO, ERR_NOFILE_MSG, msg);
		return (1);
	}
	else if (err == ERR_HDMAX)
	{
		ft_printf(STDERR_FILENO, ERR_HDMAX_MSG);
		return (2);
	}
	else
		return (exec_err_aux(err, msg));
}
