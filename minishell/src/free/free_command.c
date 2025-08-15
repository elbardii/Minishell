#include "parser.h"

static void	free_redirs(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		free(current->file);
		free(current);
		current = next;
	}
}

static void	free_simple_cmd(t_simple_cmd *cmd)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free_redirs(cmd->redirs);
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	
	if (cmd->type == CMD_SIMPLE)
		free_simple_cmd(&cmd->data.simple);
	else if (cmd->type == CMD_PIPE)
	{
		free_command(cmd->data.pipe_cmd.left);
		free_command(cmd->data.pipe_cmd.right);
	}
	
	free(cmd);
}

