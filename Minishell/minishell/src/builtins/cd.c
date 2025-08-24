/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:52:37 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:52:38 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builtins.h"
#include <unistd.h>

int	builtin_cd(char **args, t_env *env)
{
	char	*path;
	char	*home;
	char	*oldpwd;
	char	*current_pwd;

	// Check for too many arguments
	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (1);
	}
	
	// Get current directory for OLDPWD
	current_pwd = getcwd(NULL, 0);
	
	if (!args[1])
	{
		home = get_env_value(env, "HOME");
		if (!home)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			free(current_pwd);
			return (1);
		}
		path = home;
	}
	else if (ft_strncmp(args[1], "-", 2) == 0 && !args[1][1])
	{
		oldpwd = get_env_value(env, "OLDPWD");
		if (!oldpwd)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			free(current_pwd);
			return (1);
		}
		path = oldpwd;
		printf("%s\n", path); // Print the directory we're going to
	}
	else
		path = args[1];
	
	if (chdir(path) != 0)
	{
		perror("cd");
		free(current_pwd);
		return (1);
	}
	
	// Update OLDPWD and PWD
	if (current_pwd)
	{
		set_env_value(&env, "OLDPWD", current_pwd);
		free(current_pwd);
	}
	
	current_pwd = getcwd(NULL, 0);
	if (current_pwd)
	{
		set_env_value(&env, "PWD", current_pwd);
		free(current_pwd);
	}
	
	return (0);
}

