#include "builtins.h"

static void	print_with_escape(char *str, int interpret_escapes)
{
    int	i;

    i = 0;
    while (str[i])
    {
        if (interpret_escapes && str[i] == '\\' && str[i + 1])
        {
            i++;
            if (str[i] == 'n')
                write(STDOUT_FILENO, "\n", 1);
            else if (str[i] == 't')
                write(STDOUT_FILENO, "\t", 1);
            else if (str[i] == 'r')
                write(STDOUT_FILENO, "\r", 1);
            else if (str[i] == 'b')
                write(STDOUT_FILENO, "\b", 1);
            else if (str[i] == 'a')
                write(STDOUT_FILENO, "\a", 1);
            else if (str[i] == 'v')
                write(STDOUT_FILENO, "\v", 1);
            else if (str[i] == 'f')
                write(STDOUT_FILENO, "\f", 1);
            else if (str[i] == '\\')
                write(STDOUT_FILENO, "\\", 1);
            else
            {
                char buf[2] = {'\\', 0};
                write(STDOUT_FILENO, buf, 1);
                buf[0] = str[i];
                write(STDOUT_FILENO, buf, 1);
            }
        }
        else
        {
            char c = str[i];
            write(STDOUT_FILENO, &c, 1);
        }
        i++;
    }
}

// Helper: treat -n, -nn, -nnn ... as the -n flag (match bash behavior)
static int is_n_flag(char *s)
{
    int j;

    if (!s || s[0] != '-')
        return (0);
    j = 1;
    if (s[j] != 'n')
        return (0);
    while (s[j])
    {
        if (s[j] != 'n')
            return (0);
        j++;
    }
    return (1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;
	int	first_arg;
	int	interpret_escapes;


	newline = 1;
	interpret_escapes = 0;
	i = 1;
	first_arg = 1;
	
    // Check for flags. Accept -n, -nnn... as -n (suppress newline)
    while (args[i] && args[i][0] == '-')
    {
        if (is_n_flag(args[i]))
        {
            newline = 0;
            i++;
        }
        else if (ft_strncmp(args[i], "-e", 2) == 0 && !args[i][2])
        {
            interpret_escapes = 1;
            i++;
        }
        else
            break;
    }
	
    // Print arguments with spaces between them
    while (args[i])
    {
        if (!first_arg)
            write(STDOUT_FILENO, " ", 1);
        print_with_escape(args[i], interpret_escapes);
        first_arg = 0;
        i++;
    }
	
    // Always print newline unless -n flag is present
    if (newline)
        write(STDOUT_FILENO, "\n", 1);

    return (0);
}
