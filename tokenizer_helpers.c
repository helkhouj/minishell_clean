/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

char	*extract_quoted_string(char **input, char quote)
{
	char	*start;
	char	*end;
	char	*result;

	start = *input + 1;
	end = start;
	while (*end && *end != quote)
		end++;
	if (*end != quote)
		return (NULL);
	result = ft_substr(start, 0, end - start);
	*input = end + 1;
	return (result);
}

t_token_type	get_operator_type(char **input)
{
	if (**input == '|')
	{
		(*input)++;
		return (TOKEN_PIPE);
	}
	if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			*input += 2;
			return (TOKEN_REDIR_HEREDOC);
		}
		(*input)++;
		return (TOKEN_REDIR_IN);
	}
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			*input += 2;
			return (TOKEN_REDIR_APPEND);
		}
		(*input)++;
		return (TOKEN_REDIR_OUT);
	}
	return (TOKEN_WORD);
}
