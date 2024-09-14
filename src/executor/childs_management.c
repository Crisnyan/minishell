/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   childs_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:07:51 by vpf               #+#    #+#             */
/*   Updated: 2024/09/14 20:48:24 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_status(int status)
{
	if (status == SIGINT)
		status = 130;
	else if (status == SIGQUIT)
	{
		status = 131;
		ft_printf(STDOUT_FILENO, "Quit (core dumped)\n");
	}
	return (status);
}

int	wait_child_processes(pid_t *childs, int amount)
{
	pid_t	pid;
	int		i;
	int		stat_loc;
	int		status;

	i = 0;
	status = 1;
	while (i < amount)
	{
		pid = waitpid(-1, &stat_loc, 0);
		if (pid < 0)
			exec_err(ERR_STD, "pid:");
		if (pid == childs[amount - 1] && WIFEXITED(stat_loc))
			status = WEXITSTATUS(stat_loc);
		else if (pid == childs[amount - 1] && WIFSIGNALED(stat_loc))
		{
			status = WTERMSIG(stat_loc);
			status = check_status(status);
		}
		i++;
	}
	if (amount > 1)
		free(childs);
	return (status);
}
