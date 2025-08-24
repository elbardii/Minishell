/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:51:40 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:51:42 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "lexer.h"

// Command types
typedef enum e_command_type
{
	CMD_SIMPLE,
	CMD_PIPE,
	CMD_REDIRECT
} t_command_type;

// Redirection types
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC,
	REDIR_APPEND
} t_redir_type;

// Redirection structure
typedef struct s_redir
{
	t_redir_type	type;
	char		*file;
	struct s_redir	*next;
} t_redir;

// Simple command structure
typedef struct s_simple_cmd
{
	char		**args;
	t_redir		*redirs;
} t_simple_cmd;

// Command structure (AST node)
typedef struct s_command
{
	t_command_type	type;
	union
	{
		t_simple_cmd	simple;
		struct
		{
			struct s_command	*left;
			struct s_command	*right;
		} pipe_cmd;
	} data;
} t_command;

// Parser function prototypes
t_command	*parse(t_token *tokens);
void		free_command(t_command *cmd);

#endif


