/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 15:35:44 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 16:44:09 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

// process directory entries
static char	*pde(DIR *d, const char *dp, const char *pattern, int *match_found)
{
	char			*result;
	size_t			result_len;
	struct dirent	*entry;

	result = create_result_buffer();
	result_len = 0;
	if (!result)
	{
		closedir(d);
		return (NULL);
	}
	entry = readdir(d);
	while (entry != NULL)
	{
		if (matches_pattern(pattern, entry->d_name))
		{
			*match_found = 1;
			result = process_entry(result, &result_len, dp, entry->d_name);
			if (!result)
				return (free_resources(d, NULL, NULL, result));
		}
		entry = readdir(d);
	}
	return (result);
}

// finalize_result_for_directory
static char	**frfd(DIR *dir, char *result, int match_found, char **argv)
{
	closedir(dir);
	if (!match_found)
	{
		perror("No pattern found");
		free(result);
		return (NULL);
	}
	return (finalize_result(result, argv));
}

int	matches_pattern(const char *pattern, const char *str)
{
	while (*pattern && *str)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			if (!*pattern)
				return (1);
			while (*str)
			{
				if (matches_pattern(pattern, str))
					return (1);
				str++;
			}
			return (0);
		}
		else if (*pattern != *str)
			return (0);
		pattern++;
		str++;
	}
	return ((!*pattern || (*pattern == '*' && !*(pattern + 1))) && !*str);
}

char	**process_directory(const char *dp, const char *pattern, char **argv)
{
	DIR		*dir;
	int		match_found;
	char	*result;

	dir = opendir(dp);
	match_found = 0;
	if (!dir)
	{
		perror("Error opening directory");
		return (NULL);
	}
	result = pde(dir, dp, pattern, &match_found);
	if (!result)
		return (NULL);
	return (frfd(dir, result, match_found, argv));
}
