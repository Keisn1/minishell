/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_wildcard_2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 14:27:01 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 18:27:21 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int matches_pattern(const char *pattern, const char *str)
{
    while (*pattern && *str)
    {
        if (*pattern == '*')
        {
            while (*pattern == '*')
                pattern++;
            if (!*pattern)
                return 1;
            while (*str)
            {
                if (matches_pattern(pattern, str))
                    return 1;
                str++;
            }
            return 0;
        }
        else if (*pattern != *str)
            return 0;
        pattern++;
        str++;
    }
    return ((!*pattern || (*pattern == '*' && !*(pattern + 1))) && !*str);
}

char *get_dir_path(const char *word, char **pattern)
{
    char *slash = ft_strrchr(word, '/');
    if (slash)
    {
        *pattern = ft_strdup(slash + 1);
        return ft_substr(word, 0, slash - word);
    }
    *pattern = ft_strdup(word);
    return ft_strdup(".");
}

char *create_result_buffer()
{
    char *result = malloc(1);
    if (result)
        result[0] = '\0';
    return result;
}

char *build_full_path(const char *dir_path, const char *entry_name)
{
    if (ft_strcmp(dir_path, ".") == 0)
        return ft_strdup(entry_name);

    // Otherwise, construct the full path
    size_t dir_len = ft_strlen(dir_path);
    size_t entry_len = ft_strlen(entry_name);
    size_t full_len = dir_len + entry_len + 2;

    char *full_path = malloc(full_len);
    if (!full_path) return NULL;

    char *ptr = full_path;
    const char *src = dir_path;
    while (*src) *ptr++ = *src++;
    *ptr++ = '/';
    src = entry_name;
    while (*src) *ptr++ = *src++;
    *ptr = '\0';

    return full_path;
}

char *append_entry_to_result(char *result, size_t *result_len, const char *full_path)
{
    if (*result_len > 0)
        result = append_to_result(result, result_len, " ", 1);
    result = append_to_result(result, result_len, full_path, ft_strlen(full_path));
    return result;
}
