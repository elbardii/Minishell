/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:29:15 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/17 02:45:32 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	i;
	size_t	ii;
	char	*rs;

	i = 0;
	ii = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	rs = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!rs)
		return (NULL);
	while (s1[i])
		rs[ii++] = s1[i++];
	i = 0;
	while (s2[i])
		rs[ii++] = s2[i++];
	rs[ii] = '\0';
	return (rs);
}
