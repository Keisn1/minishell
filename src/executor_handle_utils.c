/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_handle_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 12:47:50 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 18:27:31 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*handle_env_expansion(const char *var_name)
{
	char *value;
	
	value = getenv(var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*expand_variable(const char *str, size_t *i)
{
	char	*var_name;
	char	*var_value;
	size_t	start;
	
	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	var_name = ft_strndup(&str[start], *i - start);
	var_value = handle_env_expansion(var_name);
	free(var_name);
	if (var_value)
		return (var_value);
	return (ft_strdup(""));
}

char	*append_to_result(char *result, size_t *result_len, const char *addition, size_t addition_len)
{
	char	*new_result;

	new_result = malloc(*result_len + addition_len + 1);
	if (!new_result)
	{
		free(result);
		return NULL;
	}
	if (result)
	{
		ft_memcpy(new_result, result, *result_len);
		free(result);
	}
	ft_memcpy(new_result + *result_len, addition, addition_len);
	*result_len += addition_len;
	new_result[*result_len] = '\0';
	return (new_result);
}
