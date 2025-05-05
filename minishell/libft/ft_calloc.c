/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:00:42 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/15 14:26:20 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			i;
	unsigned char	*b;

	i = 0;
	b = (unsigned char *)malloc(nmemb * size);
	if (!b)
		return (NULL);
	while (i < (nmemb * size))
	{
		b[i] = 0;
		i++;
	}
	return ((void *)b);
}
