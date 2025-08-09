/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env(envp);
	shell->exit_code = 0;
	shell->stdin_fd = dup(STDIN_FILENO);
	shell->stdout_fd = dup(STDOUT_FILENO);
}

static void	cleanup_shell(t_shell *shell)
{
	free_env(shell->env);
	close(shell->stdin_fd);
	close(shell->stdout_fd);
}

static int	run_iteration(t_shell *shell)
{
    char	*input;

    input = readline("minishell$ ");
    if (!input)
    {
        printf("exit\n");
        return (0);
    }
    if (g_signal == SIGINT)
    {
        shell->exit_code = 130;
        g_signal = 0;
    }
    if (*input)
    {
        add_history(input);
        process_input(input, shell);
    }
    free(input);
    return (1);
}

static char	*expand_command_args(char **args, t_shell *shell)
{
	char	*expanded;
	char	*temp;
	int		i;

	if (!args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded = expand_variables(args[i], shell);
		if (expanded)
		{
			temp = args[i];
			args[i] = expanded;
			free(temp);
		}
		i++;
	}
	return (NULL);
}

static void	process_input(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_ast	*ast;
	int		exit_code;

	if (!input || !*input)
		return ;
	tokens = tokenize(input);
	if (!tokens)
		return ;
	ast = parse(tokens);
	free_tokens(tokens);
	if (!ast)
		return ;
	if (ast->cmd && ast->cmd->args)
		expand_command_args(ast->cmd->args, shell);
	exit_code = execute_ast(ast, shell);
	shell->exit_code = exit_code;
	restore_redirections(shell->stdin_fd, shell->stdout_fd);
	free_ast(ast);
}

int	main(int argc, char **argv, char **envp)
{
    t_shell	shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, envp);
    setup_signals();
    while (run_iteration(&shell))
        ;
    cleanup_shell(&shell);
    return (shell.exit_code);
}
