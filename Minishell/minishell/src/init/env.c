/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:56:03 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:56:06 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*current;

	if (!*env)
	{
		*env = new_node;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	char	*equal_pos;
	char	*key;
	char	*value;
	int	i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			key = ft_substr(envp[i], 0, equal_pos - envp[i]);
			value = ft_strdup(equal_pos + 1);
			if (key && value)
			{
				new_node = create_env_node(key, value);
				if (new_node)
					add_env_node(&env, new_node);
			}
			free(key);
			free(value);
		}
		i++;
	}
	return (env);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0 &&
		    ft_strlen(current->key) == ft_strlen(key))
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}


