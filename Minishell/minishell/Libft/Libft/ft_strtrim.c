/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 21:58:06 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/15 21:29:45 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_set(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	i;
	int	slen;

	if (!s1)
		return (NULL);
	i = 0;
	slen = ft_strlen(s1) - 1;
	while (s1[i] && in_set(s1[i], set))
		i++;
	while (s1[slen] && in_set(s1[slen], set))
		slen--;
	if (slen == -1)
		return (ft_substr(s1, i, 0));
	return (ft_substr(s1, i, (slen - i + 1)));
}
