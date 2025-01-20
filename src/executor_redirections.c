/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 12:40/12 by kfreyer           #+#    #+#             */
/*   Updated: 2025/01/17 12:40:12 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "parser.h"
#include <fcntl.h>
#include <unistd.h>

void	close_fds(int fds[2])
{
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	return ;
}

int	set_output(t_redirection r, int fds[2], int hered_pipe[2])
{
	(void)hered_pipe;
	if (r.type == OUT)
	{
		fds[1] = open(r.word, O_CREAT | O_WRONLY | O_TRUNC,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fds[1] < 0)
		{
			perror("open");
			close_fds(fds);
			return EXIT_FAILURE;
		}
	}
	if (r.type == APPEND)
	{
		fds[1] = open(r.word, O_APPEND | O_WRONLY | O_CREAT,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fds[1] < 0)
		{
			perror("open");
			close_fds(fds);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

int	set_input(t_redirection r, int fds[2], int hered_pipe[2])
{
	if (r.type == IN)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = open(r.word, O_RDONLY, NULL);
		if (fds[0] < 0)
		{
			perror("open");
			close_fds(fds);
			return (EXIT_FAILURE);
		}
	}
	if (r.type == HERED)
	{
		pipe(hered_pipe);
		write(hered_pipe[1], r.word, ft_strlen(r.word));
		close(hered_pipe[1]);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = hered_pipe[0];
	}
	return (EXIT_SUCCESS);
}

t_list	*ignore_wildcard_in_redirection(t_redirection redirection)
{
	t_redirection	*new;

	new = (t_redirection *)malloc(sizeof(t_redirection));
	new->word = ft_strdup(redirection.word);
	new->word_type = LITERAL;
	new->type = redirection.type;
	new->fd = redirection.fd;
	return (ft_lstnew(new));
}

t_list	*handle_wildcard_redirection(t_redirection redirection)
{
	char	*dir_path;
	char	*pattern;
	t_list	*dir_entries;
	t_list	*new_redirection;

	dir_path = get_dir_path_2(redirection.word);
	pattern = get_pattern(redirection.word);
	if (ft_strcmp(dir_path, "") && ft_strcmp(dir_path, "."))
		new_redirection = ignore_wildcard_in_redirection(redirection);
	else
	{
		dir_entries = get_dir_entries(dir_path);
		t_list dummy = {NULL, dir_entries};
		t_list *prev = &dummy;
		t_list *curr = dummy.next;
		while (curr) {
			char* entry = (char *)curr->content;
			if (!matches_pattern(pattern, entry) &&
				no_hidden_file(entry, pattern)) {
				prev->next = curr->next;
				curr->next = NULL;
				ft_lstclear(&curr, free);
				curr = prev->next;
			} else {
				prev = prev->next;
				curr = curr->next;
			}
		}
		dir_entries = dummy.next;
		if (ft_lstsize( dir_entries)>1) {
			ft_putendl_fd("ambigious redirect", STDERR_FILENO);
			ft_lstclear(&dir_entries, free);
			free(dir_path);
			free(pattern);
			return NULL;
		}

		t_redirection *new = (t_redirection *)malloc(sizeof(t_redirection));
		new->word = ft_strdup((char*)dir_entries->content);
		new->word_type = LITERAL;
		new->type = redirection.type;
		new->fd = redirection.fd;
		new_redirection = (ft_lstnew(new));
		ft_lstclear(&dir_entries, free);
	}
	free(dir_path);
	free(pattern);
	return (new_redirection);
}

int	expand_wildcards_in_redirections(t_list **list)
{
	t_list	*head;
	t_list	*new;
	t_list	dummy;

	if (!list || !*list)
		return (EXIT_FAILURE);
	dummy = (t_list){NULL, *list};
	head = &dummy;
	while (head->next)
	{
		if (((t_redirection *)head->next->content)->word_type == WILDCARD_EXP)
		{
			new = handle_wildcard_redirection(*(t_redirection *)head->next->content);
			if (!new)
				return (EXIT_FAILURE);
			replace_list_next_with_new(head, new);
		}
		head = head->next;
	}
	*list = dummy.next;
	return (EXIT_SUCCESS);
}


int	set_redirections(t_list *redirections, int fds[2])
{
	t_list			*head;
	t_redirection	r;
	int				hered_pipe[2];

	if (redirections) {
		if (expand_wildcards_in_redirections(&redirections) == EXIT_FAILURE)
			return EXIT_FAILURE;
	}
	head = redirections;
	while (head)
	{
		r = *((t_redirection *)head->content);
		if (set_input(r, fds, hered_pipe) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (set_output(r, fds, hered_pipe) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		head = head->next;
	}
	return (0);
}
