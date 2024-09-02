/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishplit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 00:22:43 by cristian          #+#    #+#             */
/*   Updated: 2024/09/03 00:33:59 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*minishplit(char *line)
{
	char	quote;
	int		i;
	t_token	*tok[3];

	i = 0;
	tok[HEAD] = NULL;
	tok[PREV] = NULL;
	while (line[i])
	{
		quote = '\0';
		while (is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (is_quote(line[i]))
			quote = line[i];
		tok[PTR] = get_token(&line[i], quote, line, i);
		i += tok[PTR]->adv;
		set_tokens(tok);
		tok[PREV] = tok[PTR];
		tok[PTR] = tok[PTR]->next;
	}
	return (tok[HEAD]);
}
