/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

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
	while (**input && **input != ' ' && **input != '\t' && **input != '|'
		&& **input != '<' && **input != '>' && **input != '\''
		&& **input != '"')
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
