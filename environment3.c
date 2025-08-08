/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	free_env_node(t_env *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

static t_env	*remove_head_node(t_env *env)
{
	t_env	*next;

	next = env->next;
	free_env_node(env);
	return (next);
}

static t_env	*remove_middle_node(t_env *prev, t_env *current)
{
	prev->next = current->next;
	free_env_node(current);
	return (prev);
}

t_env	*unset_env_value(t_env *env, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !key)
		return (env);
	if (ft_strcmp(env->key, key) == 0)
		return (remove_head_node(env));
	prev = env;
	current = env->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			remove_middle_node(prev, current);
			return (env);
		}
		prev = current;
		current = current->next;
	}
	return (env);
}

void	free_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
