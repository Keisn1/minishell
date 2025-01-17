/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/17 15:06:12 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int error_fork()
{
	perror("fork");
	return EXIT_FAILURE;
}

void close_fds(int fds[2]) {
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	return;
}

static int custom_exec(char *cmd_path, char **args, char **ep, int fds[2]) {
	pid_t pid = fork();
	int status;

	if (pid < 0)
		return error_fork();
	if (pid == 0)
	{
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		
		if (execve(cmd_path, args, ep) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		close_fds(fds);
		return EXIT_SUCCESS;
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return 1;
	}
	if (WIFEXITED(status))
		return  WEXITSTATUS(status);
	fprintf(stderr, "Child process did not terminate normally\n");
	return EXIT_FAILURE;
}

int set_redirections(t_list* redirections, int fds[2]) {
	t_list *head = redirections;
	while (head) {
		t_redirection r = *((t_redirection*)head->content);
		if (r.type == IN) {
			if (fds[0] != STDIN_FILENO)
				close(fds[0]);
			fds[0] = open(r.word, O_RDONLY);
			if (fds[0]< 0) {
				perror("open");
				close_fds(fds);
				return 1;
			}
		}
		if (r.type == OUT) {
			fds[1]  = open(r.word, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (fds[1] < 0) {
				perror("open");
				close_fds(fds);
				return 1;
			}
		}
		if (r.type == APPEND) {
			fds[1]  = open(r.word, O_APPEND|O_WRONLY|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (fds[1] < 0) {
				perror("open");
				close_fds(fds);
				return 1;
			}
		}
		if (r.type == HERED) {
			int hered_pipe[2];
			pipe(hered_pipe);
			write(hered_pipe[1], r.word, ft_strlen(r.word));
			close(hered_pipe[1]);

			if (fds[0] != STDIN_FILENO)
				close(fds[0]);
			fds[0] = hered_pipe[0];
		}
		head = head->next;
	}
	return 0;
}

int execute_command(t_cmd_node cmd_node, char **ep, t_data *data)
{
    char *cmd_path;
    char **argv = NULL;

    int fds[2] = {STDIN_FILENO, STDOUT_FILENO};
	
    if (set_redirections(cmd_node.redirections, fds))
        return 1;

    int res = 0;

    if (cmd_node.cmd_token.type != NONE)
	{
        cmd_path = find_path(cmd_node.cmd_token.content, ep);
        if (!cmd_path)
        {
            fprintf(stderr, "Command not found: %s\n", cmd_node.cmd_token.content);
            return 127;
        }
        argv = list_to_argv(cmd_node.arguments, cmd_path, data);
        if (!argv)
		{
            free(cmd_path);
            return 1;
        }
        res = custom_exec(cmd_path, argv, ep, fds);
        free_matrix(argv);
        free(cmd_path);
    }
    close_fds(fds);
    return res;
}
