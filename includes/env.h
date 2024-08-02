/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:09 by vperez-f          #+#    #+#             */
/*   Updated: 2024/08/02 17:06:43 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

#define PREFIX					ANSI_COLOR_MAGENTA_B
#define SUFFIX					"\x1b[0m$ "
#define EXPORT_PREFIX			"declare -x "
#define EXPORT_ERROR			"minishell: export: '%s': not a valid identifier\n"
#define PWD_ERROR				"Unable to determine current working directory\n"
#define CD_ERROR				"cd: no such file or directory: %s\n"

typedef struct s_entry
{
	char	*key;
    char    *value;
	int		is_export;
}			t_entry;

typedef struct s_dict
{
	t_entry	*entries;
	int		current;
	int		cap;
}			t_dict;

int		init_env(char **envp, t_dict *m_env);

int		ft_export(char *line, t_dict *dict);
int		ft_unset(char *line, t_dict *dict);
int     ft_cd(char *path);
int     print_pwd();

char	*ft_getenv(char *key, t_dict *dict);
char	**fetch_env(t_dict *dict);
char	**split_env(char *line);

void    print_env(t_dict *dict);
void	print_export(t_dict *dict);
void	free_env(char **env);
void	free_entry(t_entry *entry);
void	add_word(char *key, char *value, t_dict *dict, int export);

void	free_env(char **env);
void	free_entries(t_entry *entries, int n);

int     search_index(char *key, t_dict *dict);
int     ft_strcmp(const char *s1, const char *s2);

unsigned int	hash(char *word);

#endif