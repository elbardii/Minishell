/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:56:29 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:56:31 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_redir	*create_redir(t_redir_type type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

static void	add_redir(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*current;

	if (!*redirs)
	{
		*redirs = new_redir;
		return ;
	}
	current = *redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

static t_simple_cmd	parse_simple_cmd(t_token **tokens)
{
    t_simple_cmd	cmd;
    t_redir		*redir;
    t_redir_type	redir_type;

    // Temporary arg linked list
    typedef struct s_arg { char *v; struct s_arg *next; } t_arg;
    t_arg	*args_head = NULL;
    t_arg	*args_tail = NULL;
    int		argc = 0;

    cmd.args = NULL;
    cmd.redirs = NULL;

    while (*tokens && (*tokens)->type != TOKEN_PIPE)
    {
        if ((*tokens)->type == TOKEN_WORD)
        {
            t_arg *node = malloc(sizeof(t_arg));
            if (!node)
                break ;
            node->v = ft_strdup((*tokens)->value);
            if (!node->v)
            {
                free(node);
                break ;
            }
            node->next = NULL;
            if (!args_head)
                args_head = node;
            else
                args_tail->next = node;
            args_tail = node;
            argc++;
            *tokens = (*tokens)->next;
        }
        else if ((*tokens)->type == TOKEN_REDIRECT_IN ||
                 (*tokens)->type == TOKEN_REDIRECT_OUT ||
                 (*tokens)->type == TOKEN_HEREDOC ||
                 (*tokens)->type == TOKEN_APPEND)
        {
            if ((*tokens)->type == TOKEN_REDIRECT_IN)
                redir_type = REDIR_IN;
            else if ((*tokens)->type == TOKEN_REDIRECT_OUT)
                redir_type = REDIR_OUT;
            else if ((*tokens)->type == TOKEN_HEREDOC)
                redir_type = REDIR_HEREDOC;
            else
                redir_type = REDIR_APPEND;

            *tokens = (*tokens)->next;
            if (*tokens && (*tokens)->type == TOKEN_WORD)
            {
                redir = create_redir(redir_type, (*tokens)->value);
                if (redir)
                    add_redir(&cmd.redirs, redir);
                *tokens = (*tokens)->next;
            }
        }
        else
        {
            // Unknown token type for a simple command; stop
            break ;
        }
    }

    if (argc > 0)
    {
        cmd.args = malloc(sizeof(char *) * (argc + 1));
        if (cmd.args)
        {
            int i = 0;
            t_arg *it = args_head;
            while (it)
            {
                cmd.args[i++] = it->v;
                t_arg *next = it->next;
                free(it);
                it = next;
            }
            cmd.args[i] = NULL;
        }
    }
    // If no args collected, leave cmd.args == NULL

    // Free any remaining arg nodes on allocation failure
    if (!cmd.args && args_head)
    {
        t_arg *it = args_head;
        while (it)
        {
            t_arg *next = it->next;
            free(it->v);
            free(it);
            it = next;
        }
    }

    return (cmd);
}

static t_command	*create_command(t_command_type type)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = type;
	return (cmd);
}

t_command	*parse(t_token *tokens)
{
	t_command	*cmd;
	t_command	*left;
	t_command	*right;

	if (!tokens)
		return (NULL);
	
	cmd = create_command(CMD_SIMPLE);
	if (!cmd)
		return (NULL);
	
	cmd->data.simple = parse_simple_cmd(&tokens);
	
	if (tokens && tokens->type == TOKEN_PIPE)
	{
		left = cmd;
		tokens = tokens->next;
		right = parse(tokens);
		if (!right)
		{
			free_command(left);
			return (NULL);
		}
		cmd = create_command(CMD_PIPE);
		if (!cmd)
		{
			free_command(left);
			free_command(right);
			return (NULL);
		}
		cmd->data.pipe_cmd.left = left;
		cmd->data.pipe_cmd.right = right;
	}
	
	return (cmd);
}
