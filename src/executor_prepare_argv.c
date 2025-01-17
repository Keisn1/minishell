/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_prepare_argv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:02:55 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 15:24:20 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	**allocate_and_init_argv(char *cmd_path, size_t count)
{
	char	**argv;

	argv = malloc(sizeof(char *) * (count + 2));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(cmd_path);
	if (!argv[0])
	{
		free(argv);
		return (NULL);
	}
	return (argv);
}

static char	*process_argument(t_argument *argument, t_data *data)
{
	char	*processed_word;

	processed_word = NULL;
	if (argument->type == LITERAL)
		processed_word = ft_strdup(argument->word);
	else if (argument->type == DOUBLE_QUOTE_STR)
		processed_word = handle_double_quotes(argument->word, data);
	else if (argument->type == EXIT_STATUS_EXP || argument->type == ENV_EXP)
		processed_word = handle_dollar(argument->word, data);
	return (processed_word);
}

static char	**handle_wildcard_expansion(t_argument *arg, char **argv, size_t *i)
{
	char	**temp_matrix;

	temp_matrix = handle_wildcard(arg->word, argv);
	if (!temp_matrix)
	{
		free_matrix(argv);
		return (NULL);
	}
	free_matrix(argv);
	argv = temp_matrix;
	*i = ft_matrix_size(argv);
	return (argv);
}

static char	**fill_argv(t_list *list, char **argv, size_t *i, t_data *data)
{
	t_argument	*argument;
	char		*processed_word;

	while (list)
	{
		argument = (t_argument *)list->content;
		processed_word = NULL;
		if (argument->type == WILDCARD_EXP)
		{
			argv = handle_wildcard_expansion(argument, argv, i);
			if (!argv)
				return (NULL);
			list = list->next;
			continue ;
		}
		processed_word = process_argument(argument, data);
		if (!processed_word)
		{
			free_matrix(argv);
			return (NULL);
		}
		argv[*i++] = processed_word;
		list = list->next;
	}
	return (argv);
}

char	**list_to_argv(t_list *list, char *cmd_path, t_data *data)
{
	size_t	i;
	size_t	count;
	t_list	*tmp;
	char	**argv;

	i = 1;
	count = ft_lstsize(list);
	tmp = list;
	argv = allocate_and_init_argv(cmd_path, count);
	if (!argv)
		return (NULL);
	argv = fill_argv(tmp, argv, &i, data);
	if (!argv)
		return (NULL);
	argv[i] = (NULL);
	return (argv);
}
