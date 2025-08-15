#include "lexer.h"

static t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->quote_type = 0;
	token->next = NULL;
	return (token);
}

static void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static char	*extract_quoted_content(char *line, int *i, char quote_char, int *quoted)
{
	char	*content;
	int	start;
	int	len;

	(*i)++; // Skip opening quote
	start = *i;
	*quoted = 1;
	
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	
	if (line[*i] != quote_char)
	{
		// Unclosed quote - return NULL to indicate error
		return (NULL);
	}
	
	len = *i - start;
	content = malloc(len + 1);
	if (!content)
		return (NULL);
	ft_strlcpy(content, line + start, len + 1);
	(*i)++; // Skip closing quote
	return (content);
}

static char	*extract_word_with_quotes(char *line, int *i, int *quote_type)
{
	char	*result;
	char	*temp;
	char	*quoted_content;
	int	start;
	int	len;
	int	is_quoted;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	
	*quote_type = 0; // 0=no quotes, 1=single, 2=double, 3=mixed
	
	while (line[*i] && !is_whitespace(line[*i]) && !is_metachar(line[*i]))
	{
		is_quoted = 0;
		if (line[*i] == '\'' || line[*i] == '"')
		{
			char quote_char = line[*i];
			if (*quote_type == 0)
				*quote_type = (quote_char == '\'') ? 1 : 2;
			else if (*quote_type != ((quote_char == '\'') ? 1 : 2))
				*quote_type = 3; // Mixed quotes
			
			quoted_content = extract_quoted_content(line, i, quote_char, &is_quoted);
			if (!quoted_content)
			{
				free(result);
				return (NULL); // Unclosed quote
			}
			temp = ft_strjoin(result, quoted_content);
			free(result);
			free(quoted_content);
			result = temp;
		}
		else
		{
			// Regular character
			start = *i;
			while (line[*i] && line[*i] != '\'' && line[*i] != '"' 
				&& !is_whitespace(line[*i]) && !is_metachar(line[*i]))
				(*i)++;
			len = *i - start;
			if (len > 0)
			{
				char *unquoted_part = malloc(len + 1);
				if (!unquoted_part)
				{
					free(result);
					return (NULL);
				}
				ft_strlcpy(unquoted_part, line + start, len + 1);
				temp = ft_strjoin(result, unquoted_part);
				free(result);
				free(unquoted_part);
				result = temp;
			}
		}
		if (!result)
			return (NULL);
	}
	
	return (result);
}

static t_token_type	get_redirect_type(char *line, int *i)
{
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			(*i) += 2;
			return (TOKEN_HEREDOC);
		}
		(*i)++;
		return (TOKEN_REDIRECT_IN);
	}
	else if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			(*i) += 2;
			return (TOKEN_APPEND);
		}
		(*i)++;
		return (TOKEN_REDIRECT_OUT);
	}
	return (TOKEN_WORD);
}

t_token	*lexer(char *line)
{
	t_token	*tokens;
	t_token	*new_token;
	char	*word;
	int	i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while (is_whitespace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '|')
		{
			new_token = create_token("|", TOKEN_PIPE);
			i++;
		}
		else if (is_metachar(line[i]))
		{
			t_token_type type = get_redirect_type(line, &i);
			if (type == TOKEN_HEREDOC)
				new_token = create_token("<<", type);
			else if (type == TOKEN_APPEND)
				new_token = create_token(">>", type);
			else if (type == TOKEN_REDIRECT_IN)
				new_token = create_token("<", type);
			else
				new_token = create_token(">", type);
		}
		else
		{
			int quote_type;
			word = extract_word_with_quotes(line, &i, &quote_type);
			if (!word)
				return (NULL); // Error (e.g., unclosed quotes)
			
			new_token = create_token(word, TOKEN_WORD);
			if (new_token)
				new_token->quote_type = quote_type;
			free(word);
		}
		if (!new_token)
			return (NULL);
		add_token(&tokens, new_token);
	}
	return (tokens);
}

