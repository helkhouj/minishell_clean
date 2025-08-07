/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

int	execute_command(t_ast *ast, t_shell *shell)
{
	char	*cmd_path;
	char	**env_array;
	int		status;
	pid_t	pid;

	if (!ast || !ast->cmd || !ast->cmd->args || !ast->cmd->args[0])
		return (127);
	if (is_builtin(ast->cmd->args[0]))
		return (execute_builtin(ast->cmd->args, shell));
	cmd_path = find_command_path(ast->cmd->args[0], shell->env);
	if (!cmd_path)
	{
		print_error(ast->cmd->args[0], "command not found");
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		if (setup_redirections(ast->cmd->redirs) == -1)
			exit(1);
		env_array = env_to_array(shell->env);
		execve(cmd_path, ast->cmd->args, env_array);
		print_error(ast->cmd->args[0], strerror(errno));
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (128 + WTERMSIG(status));
	}
	free(cmd_path);
	return (1);
}

static void	setup_child_pipe(int pipe_fd[2], int read_end, int write_end)
{
	close(pipe_fd[read_end]);
	dup2(pipe_fd[write_end], write_end == 1 ? STDOUT_FILENO : STDIN_FILENO);
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

