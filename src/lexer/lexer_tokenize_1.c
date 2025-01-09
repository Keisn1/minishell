/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 10:42:33 by erian             #+#    #+#             */
/*   Updated: 2025/01/09 09:51:29 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static void	skip_spaces(char *line, int *i)
{
	while (line[*i] && line[*i] == ' ')
		(*i)++;
}

static char	*extract_word(t_line_container *lc)
{
	int		start;
	char	*word;
	size_t	len;

	skip_spaces((char *)lc->line, &(lc->pos));
	start = lc->pos;
	if (!lc->line[lc->pos])
		return (NULL);
	if (ft_strchr("<>&|", lc->line[lc->pos]))
		skip_operator(lc);
	else if (lc->line[lc->pos] >= '0' && lc->line[lc->pos] <= '2'
		&& lc->line[lc->pos + 1] == '>' && lc->line[lc->pos + 2] != '>')
		lc->pos += 2;
	else if (lc->line[lc->pos] == '\'' || lc->line[lc->pos] == '\"')
		skip_quoted_text(lc);
	else if (lc->line[lc->pos] == '$')
		skip_variable(lc);
	else
		skip_unquoted_word(lc);
	len = lc->pos - start;
	if (len == 0)
		return (NULL);
	word = allocate_word(lc->line, start, len);
	return (word);
}

t_token	*create_token(char *content, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->content = content;
	token->type = type;
	return (token);
}

t_token	*get_next_token(t_line_container *lc)
{
	char	*word;

	if (!lc->line)
		return (NULL);
	if (lc->line[lc->pos] == '\0')
		return (create_token(NULL, END_OF_FILE));
	word = extract_word(lc);
	return (create_token(word, assign_type(word)));
}