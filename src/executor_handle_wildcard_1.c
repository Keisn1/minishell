/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:50:09 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 15:59:31 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*get_dir_path(const char *word, char **pattern)
{
	char	*slash;

	slash = ft_strrchr(word, '/');
	if (slash)
	{
		*pattern = strdup(slash + 1);
		return (ft_substr(word, 0, slash - word));
	}
	*pattern = ft_strdup(word);
	return (ft_strdup("."));
}

char	*build_full_path(const char *dir_path, const char *entry_name)
{
	char		*ptr;
	size_t		full_len;
	char		*full_path;
	const char	*src;

	full_len = ft_strlen(dir_path) + ft_strlen(entry_name) + 2;
	full_path = malloc(full_len);
	if (!full_path)
		return (NULL);
	ptr = full_path;
	src = dir_path;
	while (*src)
		*ptr++ = *src++;
	*ptr++ = '/';
	src = entry_name;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (full_path);
}

char	*append_entry_to_result(char *res, size_t *res_len, const char *f_path)
{
	if (*res_len > 0)
	{
		res = append_to_result(res, res_len, " ", 1);
		if (!res)
			return (NULL);
	}
	res = append_to_result(res, res_len, f_path, ft_strlen(f_path));
	return (res);
}

char	**finalize_result(char *result, char **argv)
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

char	**handle_wildcard(const char *word, char **argv)
{
	char	*dir_path;
	char	*pattern;
	char	**expanded_argv;

	dir_path = get_dir_path(word, &pattern);
	if (!dir_path || !pattern)
	{
		free(dir_path);
		free(pattern);
		return (NULL);
	}
	expanded_argv = process_directory(dir_path, pattern, argv);
	free(dir_path);
	free(pattern);
	return (expanded_argv);
}
