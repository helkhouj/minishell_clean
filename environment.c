/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 10:00:00 by student           #+#    #+#             */
/*   Updated: 2024/01/15 10:00:00 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	t_env	*current;
	char	*equals;
	int		i;

	env = NULL;
	current = NULL;
	i = 0;
	while (envp[i])
	{
		equals = ft_strchr(envp[i], '=');
		if (equals)
		{
			new_node = malloc(sizeof(t_env));
			if (!new_node)
				return (free_env(env), NULL);
			new_node->key = ft_substr(envp[i], 0, equals - envp[i]);
			new_node->value = ft_strdup(equals + 1);
			new_node->next = NULL;
			if (!env)
				env = new_node;
			else
				current->next = new_node;
			current = new_node;
		}
		i++;
	}
	return (env);
}

char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	if (!key)
		return (NULL);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

t_env	*set_env_value(t_env *env, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return (env);
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (env);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!env)
		return (new_node);
	current->next = new_node;
	return (env);
}

t_env	*unset_env_value(t_env *env, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !key)
		return (env);
	if (ft_strcmp(env->key, key) == 0)
	{
		current = env->next;
		free(env->key);
		free(env->value);
		free(env);
		return (current);
	}
	prev = env;
	current = env->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
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