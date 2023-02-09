/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeseo <jeseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 15:08:36 by jeseo             #+#    #+#             */
/*   Updated: 2023/02/08 15:46:19 by jeseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philosopher.h"

void	*ft_memset(void *bytes, int value, size_t len)
{
	unsigned char	*temp;

	temp = (unsigned char *)bytes;
	while (len > 0)
	{
		*temp = value;
		temp++;
		len--;
	}
	return (bytes);
}

void	*ft_calloc(size_t n, size_t size)
{
	void	*p;

	p = (void *)malloc(n * size);
	if (p == NULL)
		return (NULL);
	ft_memset(p, 0, n * size);
	return (p);
}
