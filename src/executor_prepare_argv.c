/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_prepare_argv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 14:30:10 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 17:35:07 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char **initialize_argv(char *cmd_path, size_t count)
{
    char **argv = malloc(sizeof(char *) * (count + 2));
    if (!argv)
        return NULL;

    argv[0] = ft_strdup(cmd_path);
    if (!argv[0]) {
        // free(argv);
        return NULL;
    }
    return argv;
}

static char *process_argument(t_argument *argument, t_data *data)
{
    if (argument->type == LITERAL)
        return ft_strdup(argument->word);
    if (argument->type == DOUBLE_QUOTE_STR)
        return handle_double_quotes(argument->word, data);
    if (argument->type == EXIT_STATUS_EXP || argument->type == ENV_EXP)
        return handle_dollar(argument->word, data);
    return NULL;
}

static char **expand_wildcard(t_argument *argument, char **argv, size_t *i)
{
    char **temp_matrix = handle_wildcard(argument->word, argv);
    if (!temp_matrix)
        return NULL;

    *i = ft_matrix_size(temp_matrix) + 1;
    return temp_matrix;
}

static char **fill_argv(t_list *list, char **argv, size_t i, t_data *data)
{
    t_list *tmp = list;

    while (tmp)
    {
        t_argument *argument = (t_argument *)tmp->content;
        char *processed_word = NULL;
        if (argument->type == WILDCARD_EXP)
        {
            argv[i] = NULL;
            argv = expand_wildcard(argument, argv, &i);
            if (!argv)
            {
                argv[i++] = ft_strdup(argument->word);
                
            }
            tmp = tmp->next;
            continue ;
        }
        processed_word = process_argument(argument, data);
        if (!processed_word)
        {
            // free_matrix(argv);
            return NULL;
        }
        argv[i++] = processed_word;
        tmp = tmp->next;
    }
    argv[i] = NULL;
    return argv;
}

char **list_to_argv(t_list *list, char *cmd_path, t_data *data)
{
    size_t count = ft_lstsize(list);
    char **argv = initialize_argv(cmd_path, count);

    if (!argv)
        return NULL;

    return fill_argv(list, argv, 1, data);
}

