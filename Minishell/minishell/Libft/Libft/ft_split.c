/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yatanagh <yatanagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:17:58 by yatanagh          #+#    #+#             */
/*   Updated: 2024/11/17 18:52:06 by yatanagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_memory(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

static int	count_word(char const *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			i++;
			continue ;
		}
		count++;
		while (str[i] != c && str[i])
			i++;
	}
	return (count);
}

static char	**words_to_array(char **arr, char const *str, char c)
{
	int	index;
	int	i;
	int	start;

	index = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			i++;
			continue ;
		}
		start = i;
		while (str[i] != c && str[i])
			i++;
		arr[index] = malloc(i - start + 1);
		if (!arr[index])
			return (NULL);
		ft_strlcpy(arr[index++], str + start, i - start + 1);
	}
	arr[index] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		words_count;

	if (!s)
		return (NULL);
	words_count = count_word(s, c);
	array = malloc((words_count + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	if (!words_to_array(array, s, c))
	{
		free_memory(array);
		return (NULL);
	}
	return (array);
}
