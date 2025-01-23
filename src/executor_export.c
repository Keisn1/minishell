/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:28:21 by erian             #+#    #+#             */
/*   Updated: 2025/01/23 17:03:36 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

bool	valid_var(char *arg)
{
	int	i;

	i = -1;
	if (!(ft_isalpha(arg[0]) || arg[i] == '_'))
		return (false);
	while (arg[++i])
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (false);
	}
	return (true);
}

bool	add_expansions(t_list **ep, t_list *arg_lst)
{
	t_list		*arg_i;
	t_list		*new_lst_node;
	t_env_var	*new_var;
	t_argument	*current_arg;
	char		*key;
	char		*value;

	arg_i = arg_lst;
	while (arg_i && arg_i->next && arg_i->next->next)
	{
		current_arg = (t_argument *)arg_i->content;
		if (!valid_var(current_arg->word))
			return (false);
		key = ft_strdup(current_arg->word);
		// printf("->%s\n", key);

		arg_i = arg_i->next;
		current_arg = (t_argument *)arg_i->content;
		if (ft_strcmp(current_arg->word, "=") != 0)
		{
			// printf("->here\n");
			free(key);
			return (false);
		}

		arg_i = arg_i->next;
		current_arg = (t_argument *)arg_i->content;
		value = ft_strdup(current_arg->word);
		// printf("->%s\n", value);

		new_var = malloc(sizeof(t_env_var));
		if (!new_var)
		{
			free(key);
			free(value);
			return (false);
		}
		new_var->key = key;
		new_var->value = value;
		new_lst_node = ft_lstnew(new_var);
		if (!new_lst_node)
		{
			free(new_var->key);
			free(new_var->value);
			free(new_var);
			return (false);
		}
		ft_lstadd_back(ep, new_lst_node);
		arg_i = arg_i->next;
	}
	return (true);
}

t_list *clone_list(t_list *src)
{
    t_list *new_list = NULL;
    t_list *tmp = src;
    t_list *new_node;

    while (tmp)
    {
        new_node = ft_lstnew(tmp->content);
        if (!new_node)
            return (NULL);
        ft_lstadd_back(&new_list, new_node);
        tmp = tmp->next;
    }
    return (new_list);
}

void sort_for_export(t_list **ep)
{
	t_list *tmp_lst;
	t_list *prev = NULL;
	t_list *last_node;
	t_env_var *env_var;
	int list_size;

	sort_arguments(ep);
	tmp_lst = *ep;
	last_node = ft_lstlast(*ep);
	list_size = ft_lstsize(*ep);
	while (list_size > 0 && tmp_lst)
	{
		env_var = (t_env_var *)tmp_lst->content;
		if (!ft_isalpha(env_var->key[0]))
		{
			if (prev)
				prev->next = tmp_lst->next;
			else
				*ep = tmp_lst->next;
			last_node->next = tmp_lst;
			tmp_lst->next = NULL;
			last_node = tmp_lst;
			tmp_lst = (prev) ? prev->next : *ep;
		}
		else
		{
			prev = tmp_lst;
			tmp_lst = tmp_lst->next;
		}
		list_size--;
	}
}

void	print_export(t_list *ep)
{
	t_list	*tmp_ep;
	t_env_var	*env_var;
	
	tmp_ep = clone_list(ep);
	sort_for_export(&tmp_ep);
	while (tmp_ep)
	{
		env_var = (t_env_var *)tmp_ep->content;

        if (env_var->value)
            printf("declare -x %s=\"%s\"\n", env_var->key, env_var->value);
        else
            printf("declare -x %s\n", env_var->key);
        tmp_ep = tmp_ep->next;
	}
	ft_lstclear(&(tmp_ep), free_env_var);
}

int	export(t_list *ep, t_cmd_node *cmd_node)
{
	if (!ft_lstsize(cmd_node->arguments))
	{
		print_export(ep);
		return EXIT_SUCCESS;
	}
	
	if (!add_expansions(&ep, (t_list *)cmd_node->arguments))
		return EXIT_FAILURE;
	
	// print_export(ep);


	return EXIT_SUCCESS;
}

