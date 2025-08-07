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