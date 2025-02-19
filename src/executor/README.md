To understand the execution part, one is advised to have a look at the
parser. The Executor can basically be seen as an execution of the
Abstract Syntax Tree that is built during the parsing.

# Entry Point

``` c
int     execute_script(t_script_node *sn, t_data *data)
{
        int     exit_upstream;

        if (!sn)
                return (0);
        if (sn->node_type == CMD_NODE)
                return (execute_cmd_node(&sn->node_data.cmd_node, data));
        if (sn->node_type == PIPE_NODE)
                return (execute_pipeline(sn, data));
        if (sn->node_type == AND_NODE)
        {
                exit_upstream = execute_script(sn->upstream, data);
                if (exit_upstream)
                        return (exit_upstream);
                return (execute_script(sn->downstream, data));
        }
        if (sn->node_type == OR_NODE)
        {
                exit_upstream = execute_script(sn->upstream, data);
                if (!exit_upstream)
                        return (exit_upstream);
                return (execute_script(sn->downstream, data));
        }
        return (EXIT_SUCCESS);
}
```

1.  If the script node is either a AND- or OR-Node, then we call the
    execute<sub>script</sub> function again on the downstream and
    upstream node.
2.  If it is a pipeline, we execute the pipeline.
3.  If it is a command, we execute the command.

# Executing the pipeline

``` c

int     execute_pipeline(t_script_node *sn, t_data *data)
{
        ...
        if (pid == 0)
                exec_upstream_child(pipedes, sn, data);
        close(pipedes[1]);
        exit_status_downstream = exec_pipe_downstream(pipedes[0], sn, data);
        if (waitpid(pid, &status, 0) == -1)
        ...
                }
```

We let the upstream being executed in a child process while the
downstream is being executed in the parent process.

# Executing a command

``` c
int     execute_cmd_node(t_cmd_node *cn, t_data *data)
{
        int     fds[2];
        int     res;

        fds[0] = STDIN_FILENO;
        fds[1] = STDOUT_FILENO;
        if (expand(cn, data))
                return (EXIT_FAILURE);
        if (set_redirections(&(cn->redirections), fds))
                return (EXIT_FAILURE);
        res = EXIT_SUCCESS;
        if (is_builtin(cn->cmd_token.content))
                res = execute_builtin(cn, data, fds);
        else if (cn->cmd_token.content && cn->cmd_token.type == WORD
                         && ft_strlen(cn->cmd_token.content))
                res = execute_cmd(cn, data, fds);
        close_fds(fds);
        return (res);
}
```

1.  We expand anything we find in the redirections or arguments inside
    the command node (cn)
2.  then we set up file descriptors inside set<sub>redirections</sub>
3.  We then decide if it is a builtin or a system-command that shall be
    executed and act accordingly

## Expansion

Expansions are happening within the structures which hold arguments and
redirections. The command node holds lists to these structs. Nodes
inside the arguments or redirections list are being replaced with new
nodes where the words are being the expanded version of what they held
before.

``` c
typedef struct s_cmd_node
{
    t_token                 cmd_token;
    t_list                  *arguments;
    t_list                  *redirections;
}                           t_cmd_node;

typedef struct s_argument
{
    char                    *word;
    t_word_type             type;
}                           t_argument;

typedef struct s_redirection
{
    int                     fd;
    t_redirection_type      type;
    char                    *word;
    t_word_type             word_type;
}                           t_redirection;
```
