/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	setup_input_redirection(char *file)
{
	int	fd;

	fd = open_input_file(file);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

static int	setup_output_redirection(char *file, int append)
{
	int	fd;

	fd = open_output_file(file, append);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

static int	setup_heredoc_redirection(char *file)
{
	int	fd;

	fd = handle_heredoc(file);
	if (fd == -1)
		return (-1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

static int	setup_one_redirection(t_redir *node)
{
    if (node->type == NODE_REDIR_IN)
        return (setup_input_redirection(node->file));
    if (node->type == NODE_REDIR_OUT)
        return (setup_output_redirection(node->file, 0));
    if (node->type == NODE_REDIR_APPEND)
        return (setup_output_redirection(node->file, 1));
    if (node->type == NODE_REDIR_HEREDOC)
        return (setup_heredoc_redirection(node->file));
    return (0);
}

int	setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
        if (setup_one_redirection(current) == -1)
            return (-1);
		current = current->next;
	}
	return (0);
}

void	restore_redirections(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
}
