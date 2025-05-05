/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:14:19 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/14 23:43:20 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*fx;
	int		slen;
	int		i;

	if (!s || !f)
		return (NULL);
	i = 0;
	slen = ft_strlen(s);
	fx = malloc(slen + 1);
	if (!fx)
		return (NULL);
	while (s[i])
	{
		fx[i] = f(i, s[i]);
		i++;
	}
	fx[i] = '\0';
	return (fx);
}
