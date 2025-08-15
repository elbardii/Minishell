#include "builtins.h"

static int	is_valid_number(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int	exit_code;

	printf("exit\n");
	
	if (!args[1])
	{
		exit(0);
	}
	
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	
	if (!is_valid_number(args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
		exit(2);
	}
	
	exit_code = ft_atoi(args[1]);
	exit(exit_code);
}

