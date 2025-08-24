/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:51:26 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:51:31 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Standard library includes
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>

// Readline includes
# include <readline/readline.h>
# include <readline/history.h>

// Libft includes
# include "../Libft/Libft/libft.h"

// Environment variable structure
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// Global variable for signal handling
extern volatile sig_atomic_t g_sig;

// Function prototypes
void	minishell_loop(void);

#endif

