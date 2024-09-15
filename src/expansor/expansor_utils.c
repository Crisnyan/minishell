/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 00:00:06 by vpf               #+#    #+#             */
/*   Updated: 2024/09/16 00:04:05 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_spaces(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (is_space(str[i]))
			return (1);
		i++;
	}
	return (0);
}

static t_token	*create_expansor_token(char *line)
{
	t_token		*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		exit (exec_err(ERR_MEM, " malloc:"));
	tok->data = ft_strdup(line);
	tok->flags = STRING;
	tok->next = NULL;
	tok->adv = 0;
	return (tok);
}

void	space_expansor_retoken(t_token *tok)
{
	int		i;
	char	**splt;
	t_token	*og_next;

	i = 0;
	og_next = tok->next;
	if (!check_spaces(tok->data))
		return ;
	splt = ft_split(tok->data, ' ');
	while (splt && splt[i] && tok)
	{
		if (i == 0)
		{
			free(tok->data);
			tok->data = ft_strdup(splt[i]);
			i++;
			continue ;
		}
		tok->next = create_expansor_token(splt[i]);
		tok = tok->next;
		i++;
	}
	tok->next = og_next;
	free_arr(splt);
}
