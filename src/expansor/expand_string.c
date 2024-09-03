/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 01:57:00 by cristian          #+#    #+#             */
/*   Updated: 2024/09/03 01:57:12 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	if1(t_token *tok, t_dict *m_env, char **str, int *n)
{
	if (tok->data[n[I]] == '$' && tok->data[n[I] + 1]
		&& (!is_quote(tok->data[n[I] + 1]) && !is_space(tok->data[n[I] + 1])))
	{
		str[AUX] = ft_substr(tok->data, n[J], n[I] - n[J]);
		str[RES] = ft_strappend(&str[RES], str[AUX]);
		free(str[AUX]);
		n[J] = n[I] + 1;
		while (tok->data[n[J]] && tok->data[n[J]] != '$'
			&& tok->data[n[J]] != '?' && !is_space(tok->data[n[J]])
			&& !is_quote(tok->data[n[J]]))
		{
			n[J]++;
		}
		if (tok->data[n[J]] == '$' || tok->data[n[J]] == '?')
			n[J]++;
		str[TO_EXPAND] = ft_substr(tok->data, n[I] + 1, n[J] - n[I] - 1);
		str[EXPANDED] = ft_getenv(str[TO_EXPAND], m_env);
		free(str[TO_EXPAND]);
		str[RES] = ft_strappend(&str[RES], str[EXPANDED]);
		free(str[EXPANDED]);
		n[I] = n[J] - 1;
	}
}

static void	if2(t_token *tok, char **str, int *nums)
{
	str[AUX] = ft_substr(tok->data, nums[J], nums[I] - nums[J]);
	str[RES] = ft_strappend(&str[RES], str[AUX]);
	free(str[AUX]);
	nums[J] = nums[I] + 1;
	str[RES] = ft_strappend(&str[RES], NULL);
}

static void	if3(t_token *tok, char **str, int *nums)
{
	str[AUX] = ft_substr(tok->data, nums[J], nums[I]);
	str[RES] = ft_strappend(&str[RES], str[AUX]);
	free(str[AUX]);
}

static void	if4(t_token *tok, char **str)
{
	free(tok->data);
	tok->data = ft_strdup(str[RES]);
	free(str[RES]);
}

void	expand_string(t_token *tok, t_dict *m_env)
{
	int		nums[2];
	char	*str[4];

	nums[I] = 0;
	nums[J] = 0;
	str[RES] = NULL;
	while (tok->data[nums[I]])
	{
		if (tok->data[nums[I]] == '$' && tok->data[nums[I] + 1]
			&& (!is_quote(tok->data[nums[I] + 1])
				&& !is_space(tok->data[nums[I] + 1])))
			if1(tok, m_env, str, nums);
		if (tok->next && (tok->next->flags == FOLLOW_QUOTE
				|| tok->next->flags == FOLLOW_DQUOTE)
			&& (tok->flags == STRING || tok->flags == FOLLOW_STRING)
			&& tok->data[nums[I]] == '$' && !tok->data[nums[I] + 1])
			if2(tok, str, nums);
		nums[I]++;
	}
	if (tok->data[nums[J]])
		if3(tok, str, nums);
	if (str[RES])
		if4(tok, str);
}
