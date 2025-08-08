/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*home;

	if (!args[1])
	{
		home = get_env_value(shell->env, "HOME");
		if (!home)
		{
			print_error("cd", "HOME not set");
			return (1);
		}
		path = home;
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		print_error("cd", strerror(errno));
		return (1);
	}
	return (0);
}

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!((str[0] >= 'a' && str[0] <= 'z')
		|| (str[0] >= 'A' && str[0] <= 'Z') || str[0] == '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z')
			|| (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	char	*equals;
	char	*key;
	char	*value;

	equals = ft_strchr(arg, '=');
	if (equals)
	{
		key = ft_substr(arg, 0, equals - arg);
		value = ft_strdup(equals + 1);
	}
	else
	{
		key = ft_strdup(arg);
		value = ft_strdup("");
	}
	if (!is_valid_identifier(key))
	{
		print_error("export", "not a valid identifier");
		free(key);
		free(value);
		return (1);
	}
	shell->env = set_env_value(shell->env, key, value);
	free(key);
	free(value);
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return (builtin_env(shell));
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], shell))
			return (1);
		i++;
	}
	return (0);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			print_error("unset", "not a valid identifier");
			return (1);
		}
		shell->env = unset_env_value(shell->env, args[i]);
		i++;
	}
	return (0);
}
