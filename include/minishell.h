#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

# include "libft.h"
# include "lexer.h"

/* ************************************************************************** */
/* global variables                                                               * */
/* ************************************************************************** */

#include <signal.h>
extern volatile sig_atomic_t	signal_received;

/* ************************************************************************** */
/* structures                                                               * */
/* ************************************************************************** */

typedef struct s_env_var {
	char* key;
	char* value;
} t_env_var;

//structure for main data
typedef struct	s_data
{
	t_list			*ep;		/* list of t_env_var */
	char			*line;
	int				exit_status;
	bool			exit;
}				t_data;

/* ************************************************************************** */
/* src                                                                      * */
/* ************************************************************************** */

// free.c
void	free_data(t_data *data);

t_data	*init(char **ep);
char	*meeting_line(t_data **data);
char * rl_gets(const char* prompt);
void	handle_signals(int signum);
char	*read_line_from_child(int read_fd);

// heredoc.c
int			heredoc_loop(t_dllist **tokens, t_data *data);
char	*read_line_from_child(int read_fd);

t_env_var *new_env_var(char *key, char *value);
void free_env_var(void *content);

int	error_fork(void);
#endif
