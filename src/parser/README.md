# Script node

``` c
typedef enum e_node_type
{
        CMD_NODE,
        PIPE_NODE,
        AND_NODE,
        OR_NODE,
        ERROR_NODE,
}                           t_node_type;

typedef struct s_script_node
{
    t_node_type             node_type;
    t_node_data             node_data;
    struct s_script_node    *upstream;
    struct s_script_node    *downstream;
    unsigned int            num_children;
}                           t_script_node;
```

  - Represents a single node in the AST.
  - Contains the type of node, which could be a **Command**, **Pipe**,
    **Logical** operation (**AND**, **OR**), or an **Error**
  - Links to other nodes via the `upstream` and `downstream` pointers,
    facilitating the tree structure
  - Can have a varying number of children, indicated by `num_children`

# Example Command structure

A command like

``` shell
(echo "hello" && wc -l < input.txt ) || (echo "World" | cat > output.txt)
```

could be illustrated as follows

``` 
                       [|| (OR)]
               /                        \
         [&& (AND)]                    [PIPE]
          /     \                    /         \
       (CMD)     (CMD REDIR)   (CMD)        (REDIR)
       [echo]    [wc -l]        [echo]        [cat]
        |           |              |            |
Args: "hello"     input.txt       "World"    output.txt
```

# Parsing order

``` c
t_script_node   *parse(t_dllist *tokens)
{
        ...
                tokens = find_last_logical(tokens);
        if (((t_token *)tokens->content)->type == AND
                || ((t_token *)tokens->content)->type == OR)
                return (parse_logical(tokens));
        tokens = find_last_pipe(tokens);
        if (((t_token *)tokens->content)->type == PIPE)
                return (parse_pipe(tokens));
        return (parse_cmd(tokens));
}
```

The entry point for the parsing is the parse function. The function is
recursive.

1.  find the last *logical operator token*
    1.  parse the downstream **pipeline**
    2.  parse upstream **script**
2.  If no *logical operator token* was found, find the last *pipe token*
    1.  parse the downstream **command**
    2.  parse upstream **pipeline**
3.  If no *pipe operator token* was found
    1.  parse the **command**

I believe that this identifies as a recursive ascent parser and I think
that it is a binary left-heavy tree, but don't hold me to it, though.

# Command Node

``` c
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

typedef struct s_cmd_node
{
    t_token                 cmd_token;
    t_list                  *arguments;
    t_list                  *redirections;
}                           t_cmd_node;
```

  - Moreover the CMD<sub>NODE</sub> holds information about arguments
    and redirections
  - The redirections and arguments are being held in order
