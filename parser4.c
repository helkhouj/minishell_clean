/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	alloc_cmd_and_args(t_ast **node, char ***args)
{
	*node = create_ast_node(NODE_COMMAND);
	if (!*node)
		return (0);
	(*node)->cmd = create_cmd();
	if (!(*node)->cmd)
		return (free(*node), 0);
	*args = (char **)malloc(sizeof(char *) * MAX_ARGS);
	if (!*args)
		return (free_ast(*node), 0);
	return (1);
}

void	process_token(t_token **current, char ***args,
		int *arg_count, t_redir **redirs)
{
	if ((*current)->type == TOKEN_WORD)
		push_arg(*args, arg_count, (*current)->value);
	else if ((*current)->type >= TOKEN_REDIR_IN
		&& (*current)->type <= TOKEN_REDIR_HEREDOC)
		handle_redirection_token(current, redirs);
}
