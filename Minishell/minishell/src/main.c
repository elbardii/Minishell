#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "env.h"
#include "expander.h"
#include "executor.h"
#include "signals.h"

// Global variable for signal handling
volatile sig_atomic_t g_sig = 0;


int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	
	char *line;
	t_token *tokens;
	t_command *cmd;
	t_env *env;
	int exit_status = 0;

	env = init_env(envp);
	setup_signals();
	
    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            printf("exit\n");
            break ;
        }
        tokens = lexer(line);
        if (tokens)
        {
            // Expand variables in tokens
            expand_tokens(tokens, env, exit_status);
            // Drop empty tokens produced by expansion (no word splitting)
            compact_empty_tokens(&tokens);
        }

        // If the line is empty after expansion, do not add to history and no-op
        if (!tokens)
        {
            free(line);
            // Keep exit_status unchanged (bash returns 0 for no-op)
            continue;
        }

        // Add to history only if something remains after expansion
        if (line[0] != '\0')
            add_history(line);

        cmd = parse(tokens);

        // Execute the command
        if (cmd)
        {
            exit_status = execute_command(cmd, env);
        }
        
        free_command(cmd);
        free_tokens(tokens);
        free(line);
    }
	
	free_env(env);
	return (0);
}
