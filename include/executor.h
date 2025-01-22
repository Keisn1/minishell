#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "libft.h"
#include "parser.h"
#include "minishell.h"


/* ************************************************************************** */
/* c files                                                                    */
/* ************************************************************************** */
// executor.c
int execute_script(t_script_node *script_node,  t_data *data);

// executor_cmd_path.c
char	*get_path_env(t_list *envp);
char	*find_path(char *cmd, char *path_env);

// executor_handle_double_quotes.c
char    *handle_double_quotes(const char *word,  t_data *data);

// executor_handle_dollar.c
char	*handle_dollar(const char *word, t_data *data);

// executor_handle_utils.c
char *handle_env_expansion(const char *var_name, t_data *data);
char	*expand_variable(const char *str, size_t *i, t_data *data);
char *append_to_result(char *result, size_t *result_len, const char *addition, size_t addition_len);

// executor_handle_wildcard_1.c
char **handle_wildcard(const char *word, char **argv);

// executor_handle_wildcard_2.c
int matches_pattern(const char *pattern, const char *str);
char *build_full_path(const char *dir_path, const char *entry_name);

// execute_cmd.c
int execute_command(t_cmd_node *cmd_node, t_data *data);

// executor_matrix_manipulation.c
void free_char_array(char **matrix);
size_t ft_size_char_array(char **matrix);
char **ft_matrix_join(char **matrix1, char **matrix2);

int matches_pattern(const char *pattern, const char *str);

/* executor_redirections.c */
int	set_redirections(t_list **redirections, int fds[2]);
void	close_fds(int fds[2]);
// executor_prepare_argv.c
char **list_to_argv(t_list *arguments, char *cmd_path);
int expand_wildcards_in_arguments(t_list **list);

void	sort_arguments(t_list **list);

void	replace_list_next_with_new(t_list *list, t_list *new_node);
char	*get_dir_path(char* path);
char	*get_pattern(char* word);
t_list	*get_dir_entries(char *dir_path);
bool	hidden_n_star(char *entry, char *pattern);

void	expand_env(t_list *arguments, t_data *data);
int	expand_arguments(t_cmd_node *cmd_node, t_data *data);
int	expand_redirections(t_cmd_node *cmd_node, t_data *data);

// executor_export
int	export(t_list *ep, t_cmd_node *cmd_node);
#endif // EXECUTOR_
