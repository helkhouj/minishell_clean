/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	add_token(t_token **tokens, t_token *new_token)
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

static char	*extract_quoted_string(char **input, char quote)
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

static t_token_type	get_operator_type(char **input)
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

static void	skip_whitespace(char **input)
{
	while (**input == ' ' || **input == '\t')
		(*input)++;
}

static void	process_quoted_string(char **input, t_token **tokens)
{
	char	*word;

	word = extract_quoted_string(input, **input);
	if (!word)
	{
		free_tokens(*tokens);
		*tokens = NULL;
		return ;
	}
	add_token(tokens, create_token(TOKEN_WORD, word));
}

static void	process_operator(char **input, t_token **tokens)
{
	t_token_type	type;

	type = get_operator_type(input);
	add_token(tokens, create_token(type, ft_strdup("")));
}

static void	process_word(char **input, t_token **tokens)
{
	char	*word_start;
	char	*word;

	word_start = *input;
	while (**input && **input != ' ' && **input != '\t'
		&& **input != '|' && **input != '<' && **input != '>'
		&& **input != '\'' && **input != '"')
		(*input)++;
	word = ft_substr(word_start, 0, *input - word_start);
	add_token(tokens, create_token(TOKEN_WORD, word));
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;

	tokens = NULL;
	while (*input)
	{
		skip_whitespace(&input);
		if (!*input)
			break ;
		if (*input == '\'' || *input == '"')
			process_quoted_string(&input, &tokens);
		else if (*input == '|' || *input == '<' || *input == '>')
			process_operator(&input, &tokens);
		else
			process_word(&input, &tokens);
		if (!tokens)
			return (NULL);
	}
	add_token(&tokens, create_token(TOKEN_EOF, NULL));
	return (tokens);
} 