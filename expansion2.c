/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static char	*get_var_name(char *str, int *len)
{
	int		i;
	char	*name;

	i = 0;
	if (str[i] == '?')
	{
		*len = 1;
		return (ft_strdup("?"));
	}
	while (str[i] && (str[i] == '_' || (str[i] >= 'A' && str[i] <= 'Z')
		|| (str[i] >= 'a' && str[i] <= 'z')
		|| (str[i] >= '0' && str[i] <= '9')))
		i++;
	*len = i;
	if (i == 0)
		return (NULL);
	name = ft_substr(str, 0, i);
	return (name);
}

static char	*get_var_value(char *var_name, t_shell *shell)
{
	char	*value;

	if (!var_name)
		return (ft_strdup("$"));
	if (ft_strcmp(var_name, "?") == 0)
	{
		value = malloc(12);
		if (!value)
			return (NULL);
		snprintf(value, 12, "%d", shell->exit_code);
		return (value);
	}
	value = get_env_value(shell->env, var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*expand_single_var(char *str, int *pos, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	if (str[*pos] != '$')
		return (NULL);
	(*pos)++;
	var_name = get_var_name(str + *pos, &var_len);
	var_value = get_var_value(var_name, shell);
	*pos += var_len;
	free(var_name);
	return (var_value);
}

static char	*join_parts(char *result, char *new_part)
{
	char	*joined;

	if (!result)
		return (new_part);
	if (!new_part)
		return (result);
	joined = ft_strjoin(result, new_part);
	free(result);
	free(new_part);
	return (joined);
}

static void	process_variable_expansion(char *str, int *i, int *start,
		char **result, t_shell *shell)
{
	char	*temp;
	char	*var_value;

	if (*i > *start)
	{
		temp = ft_substr(str, *start, *i - *start);
		*result = join_parts(*result, temp);
	}
	var_value = expand_single_var(str, i, shell);
	*result = join_parts(*result, var_value);
	*start = *i;
}

static void	process_remaining_text(char *str, int i, int start, char **result)
{
	char	*temp;

	if (i > start)
	{
		temp = ft_substr(str, start, i - start);
		*result = join_parts(*result, temp);
	}
}

char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	int		i;
	int		start;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$')
			process_variable_expansion(str, &i, &start, &result, shell);
		else
			i++;
	}
	process_remaining_text(str, i, start, &result);
	return (result);
}