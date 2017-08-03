/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnstr_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alngo <alngo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/20 13:18:03 by alngo             #+#    #+#             */
/*   Updated: 2017/06/20 13:18:40 by alngo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_putnstr_fd(char *s, size_t n, int fd)
{
	while (n)
	{
		write(1, &(*s), fd);
		s++;
		n--;
	}
}
