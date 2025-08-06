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

static t_redir	*create_redir(t_node_type type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}

static t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

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
		{
			args[arg_count] = ft_strdup(current->value);
			arg_count++;
		}
		else if (current->type >= TOKEN_REDIR_IN 
			&& current->type <= TOKEN_REDIR_HEREDOC)
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
			{
				t_redir *new_redir = create_redir(current->type - TOKEN_REDIR_IN 
					+ NODE_REDIR_IN, current->value);
				if (redirs)
				{
					t_redir *last = redirs;
					while (last->next)
						last = last->next;
					last->next = new_redir;
				}
				else
					redirs = new_redir;
			}
		}
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