/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:52:52 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:52:54 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builtins.h"

int	builtin_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	
	return (0);
}

