/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 04:59:22 by cristian          #+#    #+#             */
/*   Updated: 2024/09/15 23:58:36 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_exit(char *data)
{
	if (data == NULL)
		ft_printf(2, "minishell: exit: : numeric argument required\n");
	else
	{
		ft_printf(2, "minishell: exit: %s: numeric argument required\n", data);
	}
	return (exit(2), 2);
}

static int	check_null(t_token *token, t_process *process)
{
	if (token->data == NULL)
		return (exit(0), 0);
	if (token->data[0] == '\0')
		return (print_exit(token->data), 0);
	if (token->next)
	{
		process->m_env->err_code = 1;
		return (1);
	}
	return (0);
}

int	ft_exit(t_token *token, t_process *process)
{
	int	i;
	int	neg;

	i = 0;
	neg = 0;
	if (isatty(STDIN_FILENO))
		ft_printf(STDOUT_FILENO, "exit\n");
	if (!token->next)
		return (exit(0), 0);
	token = token->next;
	if (check_null(token, process))
		return (ft_printf(STDERR_FILENO, EXIT_ERR_ARGS), 1);
	if (token->data[i] == '-' && (i++ || 1))
		neg = 1;
	while (ft_isdigit(token->data[i]) && i - neg < 20)
		i++;
	if (token->data[i])
		return (print_exit(token->data));
	if (neg == 1 && i - neg == 19)
		if (ft_strncmp(token->data, "-9223372036854775808", 20) > 0)
			return (print_exit(token->data));
	if (neg == 0 && i == 19)
		if (ft_strncmp(token->data, "9223372036854775807", 19) > 0)
			return (print_exit(token->data));
	return (exit((unsigned char)ft_atoi(token->data)), 0);
}
