/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:36:00 by erian             #+#    #+#             */
/*   Updated: 2025/01/12 10:53:53 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"
#include <sys/wait.h>
#include <unistd.h>

int	execute_script(t_script_node *sn, t_data *data)
{
	if (!sn)
		return (0);
	if (sn->node_type == CMD_NODE)
		return (execute_command(&sn->node_data.cmd_node, data));
	if (sn->node_type == PIPE_NODE)
		return (execute_pipeline(sn, data));
	return (EXIT_SUCCESS);
}
