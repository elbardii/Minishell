#include "expander.h"

static char	*expand_variable(char *str, t_env *env, int exit_status)
{
	char	*result;
	char	*var_name;
	char	*var_value;
	int	i;
	int	start;
	char	*temp_result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			if (str[i] == '?')
			{
				char *exit_str = ft_itoa(exit_status);
				temp_result = ft_strjoin(result, exit_str);
				free(result);
				free(exit_str);
				result = temp_result;
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				start = i;
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
				var_name = ft_substr(str, start, i - start);
				var_value = get_env_value(env, var_name);
				if (var_value)
				{
					temp_result = ft_strjoin(result, var_value);
					free(result);
					result = temp_result;
				}
				free(var_name);
			}
			else if (str[i] == '\0')
			{
				// '$' at end of string
				temp_result = ft_strjoin(result, "$");
				free(result);
				result = temp_result;
			}
			else
			{
				// '$' followed by invalid character
				char dollar_and_char[3];
				dollar_and_char[0] = '$';
				dollar_and_char[1] = str[i];
				dollar_and_char[2] = '\0';
				temp_result = ft_strjoin(result, dollar_and_char);
				free(result);
				result = temp_result;
				i++;
			}
		}
		else
		{
			char c_str[2];
			c_str[0] = str[i];
			c_str[1] = '\0';
			temp_result = ft_strjoin(result, c_str);
			free(result);
			result = temp_result;
			i++;
		}
	}
	return (result);
}

char	*expand_string(char *str, t_env *env, int exit_status, int quote_type)
{
	char	*result;

	if (!str)
		return (NULL);
	
	// Handle expansion based on quote type
	if (quote_type == 1)
	{
		// Single quotes: no expansion at all
		result = ft_strdup(str);
	}
	else if (quote_type == 2 || quote_type == 0)
	{
		// Double quotes or no quotes: expand variables
		result = expand_variable(str, env, exit_status);
	}
	else
	{
		// Mixed quotes: treat as no quotes (expand variables)
		result = expand_variable(str, env, exit_status);
	}
	
	return (result);
}

void	expand_tokens(t_token *tokens, t_env *env, int exit_status)
{
	t_token	*current;
	char	*expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD)
		{
			expanded = expand_string(current->value, env, exit_status, current->quote_type);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}

void	compact_empty_tokens(t_token **tokens)
{
	t_token *curr;
	t_token *prev;

	if (!tokens || !*tokens)
		return ;
	prev = NULL;
	curr = *tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD && curr->value && curr->value[0] == '\0')
		{
			// remove this token
			t_token *to_free = curr;
			if (prev)
				prev->next = curr->next;
			else
				*tokens = curr->next;
			curr = curr->next;
			free(to_free->value);
			free(to_free);
			continue;
		}
		prev = curr;
		curr = curr->next;
	}
}
