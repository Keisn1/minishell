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
char    *handle_double_quotes(const char *word, int last_exit_status);

// executor_handle_dollar.c
char	*handle_dollar(const char *word, int last_exit_status);

// executor_handle_utils.c
char *handle_env_expansion(const char *var_name);
char *expand_variable(const char *str, size_t *i);
char *append_to_result(char *result, size_t *result_len, const char *addition, size_t addition_len);

// executor_handle_wildcard_1.c
char **handle_wildcard(const char *word, char **argv);

// executor_handle_wildcard_2.c
int matches_pattern(const char *pattern, const char *str);
char *get_dir_path(const char *word, char **pattern);
char *create_result_buffer();
char *build_full_path(const char *dir_path, const char *entry_name);
char *append_entry_to_result(char *result, size_t *result_len, const char *full_path);

// execute_cmd.c
int execute_command(t_cmd_node *cmd_node, t_data *data);

// executor_matrix_manipulation.c
void free_matrix(char **matrix);
size_t ft_matrix_size(char **matrix);
char **ft_matrix_join(char **matrix1, char **matrix2);

int matches_pattern(const char *pattern, const char *str);

/* executor_redirections.c */
int set_redirections(t_list* redirections, int fds[2]);
void	close_fds(int fds[2]);
// executor_prepare_argv.c
char **list_to_argv(t_list *arguments, char *cmd_path);
int expand_wildcards_in_arguments(t_list **list);

void	sort_arguments(t_list **list);

void	replace_list_next_with_new(t_list *list, t_list *new);
char	*get_dir_path_2(char* path);
char	*get_pattern(char* word);
t_list	*get_dir_entries(char *dir_path);
bool	no_hidden_file(char *entry, char *pattern);
#endif // EXECUTOR_
