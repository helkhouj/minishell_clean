/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static t_env	*find_env_node(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		if (!current->next)
			break ;
		current = current->next;
	}
	return (current);
}

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

t_env	*set_env_value(t_env *env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = find_env_node(env, key);
	if (current && ft_strcmp(current->key, key) == 0)
	{
		free(current->value);
		current->value = ft_strdup(value);
		return (env);
	}
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return (env);
	if (!env)
		return (new_node);
	current->next = new_node;
	return (env);
}
