#include "builtins.h"
#include <fcntl.h>

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

static int	save_and_redirect(t_redir *redirs, int *saved_stdin, int *saved_stdout)
{
	t_redir	*current;
	int	fd;

	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	
	current = redirs;
	while (current)
	{
		if (current->type == REDIR_OUT)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(current->file);
				return (1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
            close(fd);
        }
		else if (current->type == REDIR_APPEND)
		{
			fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(current->file);
				return (1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
            close(fd);
        }
		else if (current->type == REDIR_IN)
		{
			fd = open(current->file, O_RDONLY);
			if (fd == -1)
			{
				perror(current->file);
				return (1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
            close(fd);
        }
		current = current->next;
	}
	return (0);
}

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

int	execute_builtin(t_simple_cmd *cmd, t_env *env)
{
    int	saved_stdin;
    int	saved_stdout;
    int	result;

    if (!cmd->args || !cmd->args[0])
        return (0);
    
    // Handle redirections for built-ins
    if (cmd->redirs)
    {
        if (save_and_redirect(cmd->redirs, &saved_stdin, &saved_stdout) != 0)
        {
            // On redirection failure, restore any changed fds before returning
            if (saved_stdin >= 0)
                dup2(saved_stdin, STDIN_FILENO), close(saved_stdin);
            if (saved_stdout >= 0)
                dup2(saved_stdout, STDOUT_FILENO), close(saved_stdout);
            return (1);
        }
    }
	
    // Execute the built-in command
    if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
        result = builtin_echo(cmd->args);
    else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
        result = builtin_cd(cmd->args, env);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		result = builtin_pwd();
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		result = builtin_export(cmd->args, env);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		result = builtin_unset(cmd->args, env);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		result = builtin_env(env);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		result = builtin_exit(cmd->args);
	else
		result = 0;
	
	// Restore original file descriptors
	if (cmd->redirs)
		restore_fds(saved_stdin, saved_stdout);
	
	return (result);
}
