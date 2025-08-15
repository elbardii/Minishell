#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"
# include "lexer.h"
# include "env.h"

// Expansion functions
char	*expand_string(char *str, t_env *env, int exit_status, int quote_type);
void	expand_tokens(t_token *tokens, t_env *env, int exit_status);
// Remove tokens that became empty strings after expansion
void	compact_empty_tokens(t_token **tokens);

#endif
