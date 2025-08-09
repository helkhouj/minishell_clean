/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_ast	*parse_command(t_token **tokens)
{
	t_ast		*node;
	t_token		*current;
	char		**args;
	int			arg_count;
	t_redir		*redirs;

	node = create_ast_node(NODE_COMMAND);
	if (!node)
		return (NULL);
	node->cmd = create_cmd();
	if (!node->cmd)
		return (free(node), NULL);
	args = malloc(sizeof(char *) * MAX_ARGS);
	if (!args)
		return (free_ast(node), NULL);
	arg_count = 0;
	redirs = NULL;
	current = *tokens;
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
        if (current->type == TOKEN_WORD)
            push_arg(args, &arg_count, current->value);
        else if (current->type >= TOKEN_REDIR_IN
            && current->type <= TOKEN_REDIR_HEREDOC)
            handle_redirection_token(&current, &redirs);
		current = current->next;
	}
	args[arg_count] = NULL;
	node->cmd->args = args;
	node->cmd->redirs = redirs;
	*tokens = current;
	return (node);
}

t_ast	*parse(t_token *tokens)
{
	t_ast	*left;
	t_ast	*pipe_node;
	t_token	*current;

	if (!tokens || tokens->type == TOKEN_EOF)
		return (NULL);
	current = tokens;
	left = parse_command(&current);
	if (!left)
		return (NULL);
	while (current && current->type == TOKEN_PIPE)
	{
		current = current->next;
		pipe_node = create_ast_node(NODE_PIPE);
		if (!pipe_node)
			return (free_ast(left), NULL);
		pipe_node->left = left;
		pipe_node->right = parse_command(&current);
		if (!pipe_node->right)
			return (free_ast(pipe_node), NULL);
		left = pipe_node;
	}
	return (left);
} 

