/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpf <vpf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:25:00 by cristian          #+#    #+#             */
/*   Updated: 2024/09/16 00:01:35 by vpf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSOR_H
# define EXPANSOR_H

# define I 0
# define J 1

# define RES 0
# define AUX 1
# define EXPANDED 2
# define TO_EXPAND 3

# include "env.h"

void	expand(t_token *tok, t_dict *m_env);
void	expand_string(t_token *tok, t_dict *m_env);
void	rearrange(t_token *dollar, t_token *string);
void	tokjoin(t_token *tok);
void	expand_string(t_token *tok, t_dict *m_env);
void	space_expansor_retoken(t_token *tok);
t_token	*expansor(t_token *tok, t_dict *m_env);

#endif
