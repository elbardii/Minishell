#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

// Token types
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_EOF
} t_token_type;

// Token structure
typedef struct s_token
{
	char		*value;
	t_token_type	type;
	int		quote_type;  // 0=no quotes, 1=single, 2=double, 3=mixed
	struct s_token	*next;
} t_token;

// Lexer function prototypes
t_token	*lexer(char *line);
void	free_tokens(t_token *tokens);

#endif


