/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:50:18 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:50:41 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

t_env	*init_env(char **envp);
char	*get_env_value(t_env *env, char *key);
void	set_env_value(t_env **env, char *key, char *value);
void	remove_env_var(t_env **env, char *key);
void	free_env(t_env *env);

#endif


