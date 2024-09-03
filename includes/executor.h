/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:17:15 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/03 19:39:18 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <string.h>
# include <sys/wait.h>
# include <fcntl.h>

# include "env.h"

# define AUX_EXE			0
# define TEMP_EXE			1
# define PREV_EXE			2
# define REDIR_CMD			3 

# define ERR_STD			1 //perror

# define ERR_MEM			2 //1
# define ERR_MEM_MSG		"minishell:%s cannot allocate memory\n"

# define ERR_ARGS  			3 //102
# define ERR_ARGS_MSG 		"minishell: invalid number of arguments\n"

# define ERR_OUTF			4 //73
# define ERR_OUTF_MSG		"minishell: error creating outfile: %s\n"

# define ERR_PERM			5 //1
# define ERR_PERM_MSG		"minishell: permission denied: %s\n"

# define ERR_NOFILE			6 //1
# define ERR_NOFILE_MSG		"minishell: no such file or dir: %s\n"

# define ERR_COMMANDNF		7 //127
# define ERR_COMMANDNF_MSG	"minishell: command not found: %s\n"

# define ERR_FAILEDEXE		8 //126
# define ERR_FAILEDEXE_MSG	"minishell: command execution failed: %s\n"

# define ERR_HDMAX			9 
# define ERR_HDMAX_MSG		"minishell: maximum here-document count exceeded\n"

int	    ft_executor(t_process *process);
int	    wait_child_processes(pid_t *childs, int amount);

void	clean_here_docs(t_process *process);

#endif