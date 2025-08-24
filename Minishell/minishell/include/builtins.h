/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:50:05 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:50:10 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include "env.h"
# include "parser.h"

int	is_builtin(char *cmd);
int	execute_builtin(t_simple_cmd *cmd, t_env *env);

// Built-in functions
int	builtin_echo(char **args);
int	builtin_cd(char **args, t_env *env);
int	builtin_pwd(void);
int	builtin_export(char **args, t_env *env);
int	builtin_unset(char **args, t_env *env);
int	builtin_env(t_env *env);
int	builtin_exit(char **args);

#endif