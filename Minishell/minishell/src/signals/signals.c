/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:56:40 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:56:43 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	handle_sigint(int sig)
{
	(void)sig;
	/* Move to new line and clear current readline buffer */
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	/* In interactive prompt: handle SIGINT to refresh prompt, ignore SIGQUIT */
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

