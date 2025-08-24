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

