/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 19:19:28 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/24 21:03:58 by vperez-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	int		i;
	char	*user;
	char	*test;

	i = 0;
	argv = NULL;
	if (argc != 1)
		return (0);
	while (strncmp(envp[i], "USER", 4))
		i++;
	test = strdup(envp[i] + 5);
	user = ft_strjoin(test, " -- minishell: ");
	free(test);
	i = 0;
	while (i < 10)
	{
		test = readline(user);
		ft_printf(STDOUT_FILENO, "  %s\n", test);
		free(test);
		i++;
	}
	free(user);
}