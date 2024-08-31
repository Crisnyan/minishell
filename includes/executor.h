/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:17:15 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/13 18:50:59 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>

# include "env.h"

# define DEF_PATH "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

# define ERR_STD  1 //perror
# define ERR_MEM  2 //1
# define ERR_ARGS  3 //102
# define ERR_OUTF  4 //73
# define ERR_PERM  5 //1
# define ERR_NOFILE 6 //1
# define ERR_COMMANDNF 7 //127
# define ERR_FAILEDEXE 8 //126
# define ERR_HDMAX 9 //126

int	ft_executor(t_process *process);
int	wait_child_processes(pid_t *childs, int amount);

void	clean_here_docs(t_process *process);

#endif