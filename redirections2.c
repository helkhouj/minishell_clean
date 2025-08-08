/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	open_input_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_error(filename, strerror(errno));
		return (-1);
	}
	return (fd);
}

static int	open_output_file(char *filename, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		print_error(filename, strerror(errno));
		return (-1);
	}
	return (fd);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

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

int	setup_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == NODE_REDIR_IN)
		{
			if (setup_input_redirection(current->file) == -1)
				return (-1);
		}
		else if (current->type == NODE_REDIR_OUT)
		{
			if (setup_output_redirection(current->file, 0) == -1)
				return (-1);
		}
		else if (current->type == NODE_REDIR_APPEND)
		{
			if (setup_output_redirection(current->file, 1) == -1)
				return (-1);
		}
		else if (current->type == NODE_REDIR_HEREDOC)
		{
			if (setup_heredoc_redirection(current->file) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

void	restore_redirections(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
}