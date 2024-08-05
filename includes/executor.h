/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:17:15 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/05 19:45:13 by vperez-f         ###   ########.fr       */
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

typedef struct s_pip
{
	int		in_file;
	int		out_file;
	int		pipefd[2];
	pid_t	pid_child_1;
	pid_t	pid_child_2;
	char	*cmd_path;
	char	**cmd_args;
	char	**argv;
	char	**envp;
	char	**env_paths;
}			t_pip;

typedef struct s_cmd
{
	int		in_file;
	int		out_file;
	int		pipe[2];
	char	*name;
	char	*path;
	char	**args;
	char	**envp;
}			t_cmd;

typedef struct s_process
{
	int		n_pipes;
	int		stat;
	int		og_fd[2];
	int		pipe[2];
	t_dict	*m_env;
	t_token	**cmd_list;
}			t_process;

int	ft_executor(t_process *process);

#endif