/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-bar <isel-bar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 23:51:08 by isel-bar          #+#    #+#             */
/*   Updated: 2025/08/24 23:51:10 by isel-bar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "minishell.h"
# include "lexer.h"
# include "env.h"

// Expansion functions
char	*expand_string(char *str, t_env *env, int exit_status, int quote_type);
void	expand_tokens(t_token *tokens, t_env *env, int exit_status);
// Remove tokens that became empty strings after expansion
void	compact_empty_tokens(t_token **tokens);

#endif
