/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:09 by vperez-f          #+#    #+#             */
/*   Updated: 2024/09/16 15:53:59 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# define PREFIX					ANSI_COLOR_MAGENTA_B
# define SUFFIX					"\x1b[0m$ "
# define EXPORT_PREFIX			"declare -x "

# define EXPORT_ERROR	"minishell: export: '%s': not a valid identifier\n"
# define PWD_ERROR	"minishell: Unable to determine current working directory\n"
# define CD_ERROR_DIR	"minishell: cd: no such file or directory: %s\n"
# define CD_ERROR_ARGS	"minishell: cd: too many arguments\n"
# define CD_ERROR_SET	"minishell: cd: %s not set\n"
# define CD_ERROR_PERM	"minishell: cd: %s: Permission denied\n"
# define ENV_ERROR		"env: \'%s\': No such file or directory\n"
# define ENV_ERROR_OPT	"env: unrecognized option '%s'(options not allowed)\n"
# define EXIT_ERR_ARGS	"minishell: exit: too many arguments\n"

int				ft_exit(t_token *token, t_process *process);
int				ft_strcmp(const char *s1, const char *s2);
int				init_env(char **envp, t_dict *m_env);
int				ft_export(char *line, t_dict *dict, int mode);
int				ft_unset(char *line, t_dict *dict);
int				ft_cd(char *path, t_dict *dict);
int				print_pwd(void);
int				ft_echo(t_token *token);
int				search_index(char *key, t_dict *dict);
int				check_built_in(t_token *token, t_process *process);
char			*ft_getenv(char *key, t_dict *dict);
char			**fetch_env(t_dict *dict);
char			**split_env(char *line);
void			print_env(t_dict *dict);
void			print_export(t_dict *dict);
void			add_word(char *key, char *value, t_dict *dict, int export);
void			append_word(char *key, char *value, t_dict *d, int export);

void			expand_dict(t_dict *dict);
void			free_env(char **env);
void			free_entry(t_entry *entry);
void			free_entries(t_entry *entries, int n);

t_entry			create_entry(char *key, char *value, int export);

unsigned int	hash(char *word);

#endif
