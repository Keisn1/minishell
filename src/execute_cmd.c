/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 15:36:06 by erian             #+#    #+#             */
/*   Updated: 2025/01/22 16:11:08 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include "parser.h"
#include <unistd.h>

int	error_fork(void)
{
	perror("fork");
	return (EXIT_FAILURE);
}

static int	custom_exec(char *cmd_path, char **args, t_list *ep, int fds[2])
{
	pid_t	pid;
	char	**env_matrix;
	int		status;
	int		count;

	pid = fork();
	if (pid < 0)
		return (error_fork());
	if (pid == 0)
	{
		env_matrix = (char **)malloc(sizeof(char *) * (ft_lstsize(ep) + 1));
		if (!env_matrix)
		{
			close_fds(fds);
			ft_putendl_fd("Allocation error", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		count = 0;
		while (ep)
		{
			char* first = ft_strjoin(((t_env_var*)ep->content)->key, "=");
			char* second;
			if (((t_env_var*)ep->content)->value) {
				second = ft_strjoin(first, ((t_env_var*)ep->content)->value);
			} else {
				second = ft_strdup(first);
			}

			env_matrix[count] =  ft_strdup(second);
			free(first);
			free(second);
			ep = ep->next;
			count++;
		}
		env_matrix[count] = NULL;
		dup2(fds[0], STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO);
		if (execve(cmd_path, args, env_matrix) == -1)
		{
			free_char_array(env_matrix);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		close_fds(fds);
		free_char_array(env_matrix);
		return (EXIT_SUCCESS);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	fprintf(stderr, "Child process did not terminate normally\n");
	return (EXIT_FAILURE);
}

int	echo(t_cmd_node cmd_node, int fds[2])
{
	char	**argv;
	int		res;
	char	**head;
	bool	print_newline;

	argv = NULL;
	argv = list_to_argv(cmd_node.arguments, "");
	res = 0;
	if (!argv)
		return (1);
	head = argv;
	head++;
	if (*head)
	{
		print_newline = true;
		if (!ft_strcmp(*head, "-n"))
		{
			print_newline = false;
			head++;
		}
		while (*head)
		{
			ft_putstr_fd(*head, fds[1]);
			head++;
			if (*head)
				ft_putstr_fd(" ", fds[1]);
		}
		if (print_newline)
			ft_putendl_fd("", fds[1]);
	}
	else
	{
		ft_putendl_fd("", fds[1]);
	}
	free_char_array(argv);
	return (res);
}

int expand(t_cmd_node *cmd_node, t_data *data) {

	if (expand_redirections(cmd_node, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (expand_arguments(cmd_node, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return EXIT_SUCCESS;
}

int execute_builtin(t_cmd_node *cmd_node, t_data *data, int fds[2]) {

	if (!ft_strcmp("echo", cmd_node->cmd_token.content))
		return echo(*cmd_node, fds);
	if (!ft_strcmp("export", cmd_node->cmd_token.content))
		return (export(data->ep, cmd_node));
	return EXIT_SUCCESS;
}


int	execute_command(t_cmd_node *cmd_node, t_data *data)
{
	char	*cmd_path;
	char	**argv;
	int		fds[2] = {STDIN_FILENO, STDOUT_FILENO};
	int		res;
	char	*path_env;

	argv = NULL;

	if (expand(cmd_node, data))
		return (EXIT_FAILURE);
	if (set_redirections(&(cmd_node->redirections), fds))
		return (EXIT_FAILURE);
	res = EXIT_SUCCESS;
	if (cmd_node->cmd_token.type == BUILTIN)
		res = execute_builtin(cmd_node, data, fds);
	else if (cmd_node->cmd_token.type == WORD)
	{
		path_env = get_path_env(data->ep);
		cmd_path = find_path(cmd_node->cmd_token.content, path_env);
		if (!cmd_path)
		{
			fprintf(stderr, "Command not found: %s\n",
					cmd_node->cmd_token.content);
			res = 127;
		} else {
			argv = list_to_argv(cmd_node->arguments, cmd_path);
			if (!argv)
				res = EXIT_FAILURE;
			else {
				res = custom_exec(cmd_path, argv, data->ep, fds);
				free_char_array(argv);
			}
			free(cmd_path);
		}
	}
	close_fds(fds);
	return (res);
}
