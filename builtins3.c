/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	exit_code = shell->exit_code;
	if (args[1])
	{
		exit_code = atoi(args[1]);
		if (args[2])
		{
			print_error("exit", "too many arguments");
			return (1);
		}
	}
	printf("exit\n");
	exit(exit_code);
}
