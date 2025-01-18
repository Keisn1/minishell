/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:50:09 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 18:10:58 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char **finalize_result(char *result, char **argv)
{
	char	**split_result;
	char	**expanded_argv;

	if (!result || ft_strlen(result) == 0)
	{
		free(result);
		return (argv);
	}
	split_result = ft_split(result, ' ');
	expanded_argv = ft_matrix_join(argv, split_result);
	free_matrix(split_result);
	free(result);
	return (expanded_argv);
}

/*
r - result
rl - result length
dp - directory path
en - entry name
*/
char	*process_entry(char *r, size_t *rl, const char *dp, const char *en)
{
	char	*full_path;

	full_path = build_full_path(dp, en);
	if (!full_path)
	{
		free(r);
		return (NULL);
	}
	// printf("<> %zu <> %s <> %s <>\n", *rl, dp, en);
	r = append_entry_to_result(r, rl, full_path);
	free(full_path);
	return (r);
}

void free_resources(DIR *dir, char *dir_path, char *pattern, char *result)
{
    if (dir)
		closedir(dir);
    if (dir_path)
		free(dir_path);
    if (pattern)
		free(pattern);
    if (result)
		free(result);
}

char **process_directory(const char *dir_path, const char *pattern, char **argv)
{
    DIR *dir = opendir(dir_path);
    if (!dir)
        return argv; 

    char *result = create_result_buffer();
    if (!result)
    {
        closedir(dir);
        return NULL;
    }
    struct dirent *entry;
    size_t result_len = 0;
    while ((entry = readdir(dir)) != NULL)
    {
		if (ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0)
            continue ;
        if (matches_pattern(pattern, entry->d_name))
        {
            result = process_entry(result, &result_len, dir_path, entry->d_name);
            if (!result)
            {
                free_resources(dir, NULL, NULL, result);
                return NULL;
            }
        }
    }
    closedir(dir);
    return finalize_result(result, argv);
}

char **handle_wildcard(const char *word, char **argv)
{
    char	*dir_path;
	char	*pattern;
	char	**split;
	char	**expanded_argv;

	dir_path = get_dir_path(word, &pattern);
	if (!dir_path || !pattern)
	{
		free(dir_path);
		free(pattern);
		return (NULL);
	}
	expanded_argv = process_directory(dir_path, pattern, argv);
	split = ft_split(word, ' ');
	if (expanded_argv == argv)
	{
		free_matrix(expanded_argv);
        expanded_argv = ft_matrix_join(argv, split);
	}
	free_matrix(split);
	free(dir_path);
	free(pattern);
	return (expanded_argv);
}
