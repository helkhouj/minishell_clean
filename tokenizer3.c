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