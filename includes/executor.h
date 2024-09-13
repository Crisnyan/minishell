/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:17:15 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/13 21:12:31 by vpf              ###   ########.fr       */
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

# define ERR_STD			1

# define ERR_MEM			2
# define ERR_MEM_MSG		"minishell:%s cannot allocate memory\n"

# define ERR_ARGS  			3
# define ERR_ARGS_MSG 		"minishell: invalid number of arguments\n"

# define ERR_OUTF			4
# define ERR_OUTF_MSG		"minishell: error creating outfile: %s\n"

# define ERR_PERM			5
# define ERR_PERM_MSG		"minishell: permission denied: %s\n"

# define ERR_NOFILE			6
# define ERR_NOFILE_MSG		"minishell: no such file or dir: %s\n"

# define ERR_COMMANDNF		7
# define ERR_COMMANDNF_MSG	"minishell: command not found: %s\n"

# define ERR_FAILEDEXE		8
# define ERR_FAILEDEXE_MSG	"minishell: command execution failed: %s\n"

# define ERR_HDMAX			9
# define ERR_HDMAX_MSG		"minishell: maximum here-document count exceeded\n"

int		ft_executor(t_process *process);

int		check_status(int status);
int		exec_err(int err, char *msg);
int		wait_child_processes(pid_t *childs, int amount);
int		format_cmd(t_cmd *cmd, t_dict *m_env, t_token *tokens, int *stat);

int		count_tokens(t_token *tokens);
void	child_exec_pipes(t_process *process, t_cmd *cmd, int i);
int		pipe_and_heredocs(t_process *process, pid_t *child, int i);
int		init_exec_pipes(t_cmd *cmd, pid_t **child, t_process *process);
void	child_exec_no_pipes(t_process *process, t_cmd *cmd, t_token *cmd_tokens);

void	free_cmd(t_cmd *cmd);
void	free_arr(char **arr);
void	close_pipes(int *pipefd);
void	clean_here_docs(t_process *process);
void	create_heredocs(t_process *process, t_token *cmd_list);

int	    is_redir(int flag);
void	check_previous(t_token **redir_list, t_token **cmd_list);
void	advance_redir(t_token **redir_list, t_token **cmd_list);
int	    check_redirs(t_process *prcs, t_cmd *cmd, t_token *temp, t_token *cmd_list);

void	heredoc_redirection(t_process *process);
void	input_redirection(t_process *process, t_cmd *cmd, t_token *target);
void	append_redirection(t_process *process, t_cmd *cmd, t_token *target);
void	output_redirection(t_process *process, t_cmd *cmd, t_token *target);
t_token	*ft_redirect(t_process *process, t_cmd *cmd, t_token *cmd_list);

#endif