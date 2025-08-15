/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:51:48 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/17 03:08:18 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	k;

	j = ft_strlen(src);
	if (!dst && size == 0)
		return (j);
	i = ft_strlen(dst);
	k = 0;
	if (size <= i)
		return (size + j);
	while (k < j && k < size - 1 - i)
	{
		dst[i + k] = src[k];
		k++;
	}
	dst[i + k] = 0;
	return (i + j);
}
