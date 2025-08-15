#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"
# include "parser.h"
# include "env.h"

// Execution functions
int	execute_command(t_command *cmd, t_env *env);
int	handle_heredoc(char *delimiter);

#endif


