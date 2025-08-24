/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:53:42 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:53:46 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "builtins.h"
#include "signals.h"
#include <fcntl.h>
#include <signal.h>

static void	print_minishell_error(const char *name, const char *msg)
{
    if (!name || !msg)
        return ;
    fprintf(stderr, "minishell: %s: %s\n", name, msg);
}

static int	precheck_and_map_path(const char *path, int *mapped_exit, const char **msg)
{
    struct stat st;

    if (stat(path, &st) == -1)
    {
        if (errno == ENOENT)
        {
            if (msg)
                *msg = "No such file or directory";
            if (mapped_exit)
                *mapped_exit = 127;
            return (0);
        }
        return (-1);
    }
    if (S_ISDIR(st.st_mode))
    {
        if (msg)
            *msg = "Is a directory";
        if (mapped_exit)
            *mapped_exit = 126;
        return (0);
    }
    if (access(path, X_OK) == -1)
    {
        if (msg)
            *msg = "Permission denied";
        if (mapped_exit)
            *mapped_exit = 126;
        return (0);
    }
    return (-1);
}

static int	map_exec_errno(const char *cmd, int had_slash, int err, const char **msg)
{
    (void)cmd;
    if (err == EACCES)
    {
        if (msg)
            *msg = "Permission denied";
        return (126);
    }
    if (err == EISDIR)
    {
        if (msg)
            *msg = "Is a directory";
        return (126);
    }
    if (err == ENOENT)
    {
        if (msg)
            *msg = had_slash ? "No such file or directory" : "command not found";
        return (127);
    }
    if (msg)
        *msg = strerror(err);
    return (126);
}

static char	**env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	char	*temp;
	int	count;
	int	i;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	
	current = env;
	i = 0;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		if (temp)
		{
			env_array[i] = ft_strjoin(temp, current->value);
			free(temp);
		}
		else
			env_array[i] = NULL;
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

static void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

static int	search_path_for_cmd(char *cmd, t_env *env, char **out_path)
{
    char	*path_env;
    char	**paths;
    int		i;
    int		seen_eacces;

    if (out_path)
        *out_path = NULL;

    path_env = get_env_value(env, "PATH");
    if (!path_env || path_env[0] == '\0')
        return (127); // PATH unset/empty => command not found

    paths = ft_split(path_env, ':');
    if (!paths)
        return (127);

    seen_eacces = 0;
    i = 0;
    while (paths[i])
    {
        char *dir = paths[i][0] ? paths[i] : ".";
        char *tmp = ft_strjoin(dir, "/");
        char *full_path = NULL;
        if (tmp)
        {
            full_path = ft_strjoin(tmp, cmd);
            free(tmp);
        }
        if (full_path)
        {
            struct stat st;
            if (stat(full_path, &st) == 0 && !S_ISDIR(st.st_mode))
            {
                if (access(full_path, X_OK) == 0)
                {
                    // success
                    int j = 0;
                    while (paths[j])
                        free(paths[j++]);
                    free(paths);
                    if (out_path)
                        *out_path = full_path;
                    else
                        free(full_path);
                    return (0);
                }
                else
                {
                    seen_eacces = 1;
                }
            }
            free(full_path);
        }
        i++;
    }

    i = 0;
    while (paths[i])
        free(paths[i++]);
    free(paths);
    return (seen_eacces ? 126 : 127);
}

static int	handle_redirections(t_redir *redirs)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == REDIR_IN)
		{
			fd = open(redirs->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redirs->file);
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
		else if (redirs->type == REDIR_OUT)
		{
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redirs->file);
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
		else if (redirs->type == REDIR_APPEND)
		{
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(redirs->file);
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
		else if (redirs->type == REDIR_HEREDOC)
		{
			// Heredoc should be handled in parser/pre-processing
			// For now, create a temporary file
			fd = handle_heredoc(redirs->file);
			if (fd == -1)
				return (1);
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				return (1);
			}
			close(fd);
		}
		redirs = redirs->next;
	}
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int	pipefd[2];
	char	*line;
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}

	if (pid == 0)
	{
		// Child process - read heredoc content
		/* Reset signals so heredoc child exits on Ctrl-C and behaves like a normal process */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pipefd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line || (ft_strlen(line) == ft_strlen(delimiter) && 
			ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0))
			{
				free(line);
				break ;
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		// Parent process - ignore SIGINT while waiting for heredoc child
		close(pipefd[1]);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, NULL, 0);
		// Restore parent handlers
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		return (pipefd[0]);
	}
}

static int	execute_simple_command(t_simple_cmd *cmd, t_env *env)
{
    char	*executable;
    char	**env_array;
    pid_t	pid;
    int	status;
    int	mapped_exit;
    const char *msg;

    if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
        return (0);
    
    // Handle built-in commands (don't fork for built-ins)
    if (is_builtin(cmd->args[0]))
        return (execute_builtin(cmd, env));

    executable = NULL;

    // When the command contains '/', treat as a path
    if (ft_strchr(cmd->args[0], '/'))
    {
        msg = NULL;
        if (precheck_and_map_path(cmd->args[0], &mapped_exit, &msg) == 0)
        {
            print_minishell_error(cmd->args[0], msg);
            return (mapped_exit);
        }
        // Use the given path directly
        executable = cmd->args[0];
    }
    else
    {
        int res = search_path_for_cmd(cmd->args[0], env, &executable);
        if (res == 126)
        {
            print_minishell_error(cmd->args[0], "Permission denied");
            return (126);
        }
        if (res == 127)
        {
            print_minishell_error(cmd->args[0], "command not found");
            return (127);
        }
        // res == 0 => executable set (allocated)
    }
    
    env_array = env_to_array(env);
    
	pid = fork();
	if (pid == 0)
	{
		/* Child process - restore default signal handling so it
		   reacts normally to SIGINT/SIGQUIT (like /bin/cat) */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);

		/* Child process - handle redirections here */
		if (handle_redirections(cmd->redirs) != 0)
			exit(1);

		execve(executable, cmd->args, env_array);
        // On execve failure, map errno to message/exit code
        msg = NULL;
        mapped_exit = map_exec_errno(cmd->args[0], ft_strchr(cmd->args[0], '/') != NULL, errno, &msg);
        print_minishell_error(cmd->args[0], msg);
        exit(mapped_exit);
    }
    else if (pid > 0)
    {
		// Parent process - ignore SIGINT/SIGQUIT while waiting for child
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		// Restore parent signal handlers: keep SIGQUIT ignored and
		// SIGINT handled by readline handler
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
        // Free only if it was allocated (PATH search)
        if (executable != cmd->args[0])
            free(executable);
        free_env_array(env_array);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
		/* If child terminated by signal, return 128 + signum (common bash behaviour) */
		if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
		return (1);
    }
    else
    {
        perror("fork");
        if (executable && executable != cmd->args[0])
            free(executable);
        free_env_array(env_array);
        return (1);
    }
}

static int	execute_pipe_command(t_command *cmd, t_env *env)
{
	int	pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int	status1;
	int	status2;
	int	final_status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}

	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}

	if (pid1 == 0)
	{
		/* First child process (left side of pipe) */
		/* Reset signals to defaults in child */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);

		close(pipefd[0]); // Close read end
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(pipefd[1]); // Close write end after dup2
		exit(execute_command(cmd->data.pipe_cmd.left, env));
	}

	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}

	if (pid2 == 0)
	{
		/* Second child process (right side of pipe) */
		/* Reset signals to defaults in child */
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);

		close(pipefd[1]); // Close write end
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(pipefd[0]); // Close read end after dup2
		exit(execute_command(cmd->data.pipe_cmd.right, env));
	}

	// Parent process - close both ends of pipe
	close(pipefd[0]);
	close(pipefd[1]);

	// Parent should ignore SIGINT/SIGQUIT while waiting for pipeline children
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	// Wait for both children
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);

	// Restore parent handlers
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);

	// Return exit status of the last command in the pipeline
	if (WIFEXITED(status2))
		final_status = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		final_status = 128 + WTERMSIG(status2);
	else
		final_status = 1;

	return (final_status);
}

int	execute_command(t_command *cmd, t_env *env)
{
	if (!cmd)
		return (0);
	
	if (cmd->type == CMD_SIMPLE)
		return (execute_simple_command(&cmd->data.simple, env));
	else if (cmd->type == CMD_PIPE)
	{
		return (execute_pipe_command(cmd, env));
	}
	
	return (0);
}
