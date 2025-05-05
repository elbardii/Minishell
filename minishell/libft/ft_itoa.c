/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 21:41:20 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/15 21:25:37 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nb_len(int n)
{
	int	i;

	i = 0;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*ft_strrev(char *str)
{
	char	tmp;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str) - 1;
	while (i < len)
	{
		tmp = str[i];
		str[i] = str[len];
		str[len] = tmp;
		i++;
		len--;
	}
	return (str);
}

static void	check_negga(int *n, unsigned int *nb, size_t *nbl)
{
	if (*n <= 0)
	{
		*nb = -(*n);
		(*nbl)++;
	}
}

char	*ft_itoa(int n)
{
	char			*num;
	size_t			i;
	size_t			nbl;
	unsigned int	nb;

	i = 0;
	nbl = 0;
	nb = n;
	check_negga(&n, &nb, &nbl);
	nbl += nb_len(n);
	num = malloc(sizeof(char) * (nbl + 1));
	if (!num)
		return (NULL);
	if (n == 0)
		num[i++] = '0';
	while (nb)
	{
		num[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	if (n < 0)
		num[i++] = '-';
	num[i] = 0;
	return (ft_strrev(num));
}
