/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:25:00 by cristian          #+#    #+#             */
/*   Updated: 2024/07/31 05:59:01 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSOR_H
# define EXPANSOR_H

void	expand(t_token *tok);
void	rearrange(t_token *dollar, t_token *string);
t_token *expansor(t_token *tok);
#endif
