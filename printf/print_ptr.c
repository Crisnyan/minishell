/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vperez-f <vperez-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:49:12 by vperez-f          #+#    #+#             */
/*   Updated: 2024/07/27 04:50:12 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_ptr(unsigned long ptr, int *printres, int fd)
{
	unsigned long	nb;
	int				base;
	char			*hexa_base;

	hexa_base = "0123456789abcdef";
	base = 16;
	nb = ptr;
	if (nb <= 15)
		prot_write(&hexa_base[(nb % base)], 1, printres, fd);
	else
	{
		print_ptr(nb / base, printres, fd);
		print_ptr(nb % base, printres, fd);
	}
}
