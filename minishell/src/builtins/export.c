#include "builtins.h"

void	set_env_value(t_env **env, char *key, char *value){
	t_env	*current;
	t_env	*new_node;

	current = *env;
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0 &&
		    ft_strlen(current->key) == ft_strlen(key))
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	
    // Create new node and append at the end to avoid
    // relying on external head pointer updates
    new_node = malloc(sizeof(t_env));
    if (!new_node)
        return ;
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;

    // Append to tail (assumes *env is not NULL as environment is initialized)
    if (*env)
    {
        t_env *tail = *env;
        while (tail->next)
            tail = tail->next;
        tail->next = new_node;
    }
    else
    {
        // Fallback for unexpected empty env
        *env = new_node;
    }
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	
	// First character must be letter or underscore
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	
	// Rest can be letters, digits, or underscores
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	
	return (1);
}

int	builtin_export(char **args, t_env *env)
{
	char	*equal_pos;
	char	*key;
	char	*value;
	int	i;
	int	exit_status;

	if (!args[1])
	{
		// Print all environment variables in export format
		t_env *current = env;
		while (current)
		{
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
			current = current->next;
		}
		return (0);
	}
	
	exit_status = 0;
	i = 1;
	while (args[i])
	{
		equal_pos = ft_strchr(args[i], '=');
		if (equal_pos)
		{
			key = ft_substr(args[i], 0, equal_pos - args[i]);
			value = ft_strdup(equal_pos + 1);
			
			if (!is_valid_identifier(key))
			{
				fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
				exit_status = 1;
			}
			else if (key && value)
			{
				set_env_value(&env, key, value);
			}
			free(key);
			free(value);
		}
		else
		{
			// Variable without value - check if valid identifier
			if (!is_valid_identifier(args[i]))
			{
				fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
				exit_status = 1;
			}
			else
			{
				set_env_value(&env, args[i], "");
			}
		}
		i++;
	}
	
	return (exit_status);
}
