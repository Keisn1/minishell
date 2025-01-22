/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erian <erian@student.42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:28:21 by erian             #+#    #+#             */
/*   Updated: 2025/01/22 19:58:43 by erian            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

// static void	add_exp(char *str, t_list *ep)
// {
// 	t_list	*new;
// 	t_env_var *new_ep;
// 	char	*res;

// 	res = ft_strdup(str);
// 	new = env_lstnew(res);
// 	env_lstadd_back(&env, new);
// }

// void	delete_and_add_exp(char *str, t_list *ep)
// {
// 	t_list	*tmp_lst;
// 	t_env_var *tmp_ep;
// 	int		i;

// 	i = 0;
// 	tmp_lst = ep;
// 	while (str[i] != '=')
// 		i++;
// 	while (tmp_lst->content)
// 	{
// 		tmp_ep = (t_env_var *)tmp_lst;
// 		if (ft_strncmp(str, tmp_ep->key, i) == 0)
// 		{
// 			free(tmp_ep->key);
// 			tmp_ep->key = ft_strdup(str);
// 		}
// 		tmp_lst = tmp_lst->next;
// 	}
// }

// bool	export_exist(char *data, t_list *ep)
// {
// 	t_list	*tmp_lst;
// 	t_env_var	*tmp_env;
// 	char	*tmp_str;
// 	int		i;

// 	i = 0;
// 	tmp_lst = ep;
// 	tmp_env = (t_env_var *)ep->content;
// 	while (data[i] != '=' && data[i] != '\0')
// 		i++;
// 	if (data[i] == '\0')
// 		tmp_str = ft_strdup(data);
// 	else
// 		tmp_str = ft_substr(data, 0, i);
// 	while (tmp_lst->content)
// 	{
// 		if (ft_strncmp(tmp_env->key, tmp_str, ft_strlen(tmp_str)) == 0)
// 		{
// 			free(tmp_str);
// 			return (true);
// 		}
// 		tmp_lst = tmp_lst->next;
// 		tmp_env = tmp_lst->content;
// 	}
// 	free(tmp_str);
// 	return (false);
// }

bool	valid_name(char *arg)
{
	int	i;

	i = 0;
	while (arg[i] != '=' && arg[i] != '\0')
	{
		if (arg[i] == '-')
			return (false);
		i++;
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
	// int i;
	// t_argument *arg;
	// char *argument;

	if (!ft_lstsize(cmd_node->arguments))
	{
		print_export(ep);
		return EXIT_SUCCESS;
	}
	
	// arg = (t_argument *)cmd_node->arguments->content;

	// while (cmd_node->arguments->next)
	// {
	// 	argument = (char *)arg->word;
	// 	if (!valid_name(argument))
	// 		return EXIT_FAILURE;
	// 	if (ft_strchr(argument, '='))
	// 	{
	// 		if (export_exist(argument, ep) == 0)
	// 			delete_and_add_exp(argument, ep);
	// 		else
	// 			add_exp(argument, ep);
	// 	}
	// }

	return EXIT_SUCCESS;
}

