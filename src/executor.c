/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:36:00 by erian             #+#    #+#             */
/*   Updated: 2025/01/18 12:10:53 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include "libft.h"
#include <unistd.h>

// int execute(t_script_node *script) {
// 	t_list* args = sn.node_data.cmd_node.arguments;
// 	while (args) {
// 		t_argument arg = *(t_argument*)args->content;
// 		ft_putstr_fd(arg.word, STDOUT_FILENO);
// 		args = args->next;
// 		if (args)
// 			ft_putstr_fd(" ", STDOUT_FILENO);
// 	}

// 	ft_putendl_fd("", STDOUT_FILENO);
// 	return 0;
// }

int execute_script(t_script_node *script_node, char **envp, t_data *data)
{
	if (!script_node)
		return (0);

	if (script_node->node_type == CMD_NODE)
	{
		data->exit_status = execute_command(script_node->node_data.cmd_node, envp, data);
		return data->exit_status;
	}
	// else if (script_node->node_type == PIPE_NODE)
	// 	// Add logic for executing pipes
	// else if (script_node->node_type == AND_NODE || script_node->node_type == OR_NODE)
	// 	// Add logic for handling logical operators (&&, ||)
	// else if (script_node->node_type == ERROR_NODE)
	// {
	// 	fprintf(stderr, "minishell: syntax error: %s\n", script_node->node_data.error_node.error);
	// 	return (2); // Syntax error
	// }

	if (script_node->downstream)
		return execute_script(script_node->downstream, envp, data);

	return (0);
}
