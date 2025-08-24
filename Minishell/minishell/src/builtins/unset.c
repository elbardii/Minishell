/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:53:21 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:53:23 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builtins.h"

void	remove_env_var(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0 &&
		    ft_strlen(current->key) == ft_strlen(key))
		{
			// If removing a non-head node, unlink normally
			if (prev)
			{
				prev->next = current->next;
				free(current->key);
				free(current->value);
				free(current);
				return ;
			}

			// Removing head: avoid changing external head pointer by
			// promoting next node into current and freeing the next node struct.
			if (current->next)
			{
				t_env *next = current->next;
				free(current->key);
				free(current->value);
				current->key = next->key;
				current->value = next->value;
				current->next = next->next;
				free(next);
				return ;
			}

			// Single-node list fallback: clear contents
			free(current->key);
			free(current->value);
			current->key = ft_strdup("");
			current->value = ft_strdup("");
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_env *env)
{
	int	i;

	if (!args[1])
		return (0);
	
	i = 1;
	while (args[i])
	{
		remove_env_var(&env, args[i]);
		i++;
	}
	
	return (0);
}
