/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	setup_child_pipe(int pipe_fd[2], int read_end, int write_end)
{
	close(pipe_fd[read_end]);
	if (write_end == 1)
		dup2(pipe_fd[write_end], STDOUT_FILENO);
	else
		dup2(pipe_fd[write_end], STDIN_FILENO);
	close(pipe_fd[write_end]);
}

static int	execute_left_child(t_ast *ast, t_shell *shell, int pipe_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_pipe(pipe_fd, 0, 1);
		exit(execute_ast(ast->left, shell));
	}
	return (pid);
}

static int	execute_right_child(t_ast *ast, t_shell *shell, int pipe_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_pipe(pipe_fd, 1, 0);
		exit(execute_ast(ast->right, shell));
	}
	return (pid);
}

int	execute_pipeline(t_ast *ast, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status;

	if (pipe(pipe_fd) == -1)
		return (1);
	left_pid = execute_left_child(ast, shell, pipe_fd);
	right_pid = execute_right_child(ast, shell, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (128 + WTERMSIG(status));
}
