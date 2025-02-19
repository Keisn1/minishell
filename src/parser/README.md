# t<sub>scriptnode</sub>

``` c
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
  - Contains the type of node, which could be a command, pipe, logical
    operation (AND, OR), or an error.
  - Links to other nodes via the `upstream` and `downstream` pointers,
    facilitating the tree structure.
  - Can have a varying number of children, indicated by `num_children`.
