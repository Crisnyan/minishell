/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:57:01 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/14 18:08:01 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

enum e_flags
{
	STRING,
	PIPE,
	I_REDIRECT,
	O_REDIRECT,
	HEREDOC,
	APPEND,
	QUOTE,
	DQUOTE,
	FOLLOW_STRING,
	FOLLOW_QUOTE,
	FOLLOW_DQUOTE
};

typedef struct s_token
{
	int				flags;
	int				adv;
	char			*data;
	struct s_token	*next;
}	t_token;

typedef struct s_entry
{
	char	*key;
	char	*value;
	int		is_export;
	int		is_tombstone;
}			t_entry;

typedef struct s_dict
{
	t_entry	*entries;
	int		current;
	int		cap;
	int		err_code;
}			t_dict;

typedef struct s_cmd
{
	int		in_file;
	int		out_file;
	char	*name;
	char	*path;
	char	**args;
	char	**envp;
}			t_cmd;

typedef struct s_process
{
	int		n_pipes;
	int		og_fd[2];
	int		pipe[2];
	int		current_heredoc;
	int		heredoc_count;
	t_dict	*m_env;
	t_token	**cmd_list;
}			t_process;

#endif
