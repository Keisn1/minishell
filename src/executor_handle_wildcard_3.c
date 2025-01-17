/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 16:26:29 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 16:39:53 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*create_result_buffer(void)
{
	char	*result;

	result = malloc(1);
	if (result)
		result[0] = '\0';
	return (result);
}

void	*free_resources(DIR *dir, char *d_path, char *pattern, char *result)
{
	if (dir)
		closedir(dir);
	if (d_path)
		free(d_path);
	if (pattern)
		free(pattern);
	if (result)
		free(result);
	return (NULL);
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
	r = append_entry_to_result(r, rl, full_path);
	free(full_path);
	return (r);
}
