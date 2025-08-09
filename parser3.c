/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

t_redir	*create_redir(t_node_type type, char *file)
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

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

void	add_redir_to_list(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*last;

	if (*redirs)
	{
		last = *redirs;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
	else
		*redirs = new_redir;
}

void	push_arg(char **args, int *arg_count, char *value)
{
	args[*arg_count] = ft_strdup(value);
	(*arg_count)++;
}

void	handle_redirection_token(t_token **current, t_redir **redirs)
{
	t_redir	*new_redir;

	*current = (*current)->next;
	if (*current && (*current)->type == TOKEN_WORD)
	{
		new_redir = create_redir((*current)->type - TOKEN_REDIR_IN + NODE_REDIR_IN,
			(*current)->value);
		add_redir_to_list(redirs, new_redir);
	}
}
