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
int execute_script(t_script_node *script_node, char **envp, t_data *data);

// executor_cmd_path.c
char	*find_path(char *cmd, char **ep);

// executor_handle_double_quotes.c
char *handle_double_quotes(const char *word, t_data *data);

// executor_handle_dollar.c
char *handle_dollar(const char *word, t_data *data);

// executor_handle_utils.c
char *handle_env_expansion(const char *var_name);
char *expand_variable(const char *str, size_t *i);
char *append_to_result(char *result, size_t *result_len, const char *addition, size_t addition_len);

// executor_handle_wildcard_1.c
char *build_full_path(const char *dir_path, const char *entry_name);
char *append_entry_to_result(char *result, size_t *result_len, const char *full_path);
char **finalize_result(char *result, char **argv);
char **handle_wildcard(const char *word, char **argv);

// executor_handle_wildcard_2.c
int matches_pattern(const char *pattern, const char *str);
char **process_directory(const char *dir_path, const char *pattern, char **argv);

// executor_handle_wildcard_3.c
char	*create_result_buffer(void);
void	*free_resources(DIR *dir, char *d_path, char *pattern, char *result);
char	*process_entry(char *r, size_t *rl, const char *dp, const char *en);

// execute_cmd.c
int execute_command(t_cmd_node cmd_node, char **envp, t_data *data);

// executor_matrix_manipulation.c
size_t ft_matrix_size(char **matrix);
void  free_matrix(char **matrix);
char **ft_matrix_join(char **matrix1, char **matrix2);

// executor_prepare_argv.c 
char **list_to_argv(t_list *list, char *cmd_path, t_data *data);

#endif // EXECUTOR_
