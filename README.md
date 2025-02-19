![](cover-minishell-bonus.png)

<p align="center">
<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Keisn1/minishell?color=blueviolet" />
<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/Keisn1/minishell?color=blue" />
<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/Keisn1/minishell?color=brightgreen" />
<img alt="GitHub Lines of Code" src="https://tokei.rs/b1/github/Keisn1/minishell?category=code" />
</p>

# Table of Content <span class="tag" data-tag-name="TOC_2"><span class="smallcaps">TOC\_2</span></span>

  - [What is Minishell?](#what-is-minishell)
      - [First Group project](#first-group-project)
      - [Building blocks](#building-blocks)
  - [How the project went](#how-the-project-went)
      - [Tests and Test Driven
        Development](#tests-and-test-driven-development)
      - [Time management](#time-management)
      - [How we used Git](#how-we-used-git)
  - [The parts](#the-parts)

# What is Minishell?

The Minishell project at 42 School is a programming assignment where
students are tasked with creating a simple shell, oriented and similar
bash. This project helps students understand how a shell works,
including aspects like command execution, input/output redirection, and
process handling. The primary goals are to learn about parsing commands,
managing environments and processes, and implementing built-in shell
functions. It requires knowledge of system calls and a good grasp of C
programming.

## First Group project

The Minishell project being a group project at 42 School introduces
students to collaboration and working in teams, which is an essential
skill in software development. Here are some aspects and challenges
associated with this aspect:

1.  **Communication:** Clear and effective communication is crucial.
    Team members must regularly discuss their progress, challenges, and
    any changes to the project plan. (see
    [3.1](#*Tests%20and%20Test%20Driven%20Development))

2.  **Version Control:** Utilizing tools like Git helps manage changes
    and contributions from different team members. It also helps resolve
    merge conflicts and track project history. (see
    [3.3](#*How%20we%20used%20Git))

3.  **Task Division:** Identifying each member's strengths and dividing
    tasks accordingly can optimize productivity. This includes
    delegating tasks like parsing, built-in command implementation, and
    handling processes. (see [2.2](#Building%20Blocks))

4.  **Consistency:** Ensuring that code style and project structure
    remain consistent across the work of different team members can be
    difficult but is important for maintainability. (see
    [3.1](#*Tests%20and%20Test%20Driven%20Development))

5.  **Time Management:** Coordinating schedules and setting realistic
    deadlines is necessary to ensure that the project progresses
    smoothly without last-minute rushes. (see
    [3.2](#*Time%20management))

Overall, the group aspect of the Minishell project teaches students not
only technical skills but also soft skills, such as teamwork and
conflict resolution, which are critical in real-world software
development.

## Building blocks

The building blocks of the Minishell project typically include the
following components:

1.  **Command Parsing:** This involves reading and interpreting the user
    input to break it down into commands and arguments. It requires
    handling different types of inputs, such as strings with quotes,
    escaped characters, and special symbols like pipes (`|`) and
    redirections (`<`, `>`) as well as logical operator (`&&` and `||`)
    and wildcards (`*`) if you do the bonus, which we have done.

2.  **Execution of Commands:** Implementing logic to execute both
    built-in commands (such as `cd`, `echo`, `exit`) and external
    commands by forking processes and using system calls like `execve`.

3.  **Environment Handling:** Managing environment variables, which
    includes retrieving, setting, and updating variables that are
    required for command execution and shell operations.

4.  **Input/Output Redirection:** Implementing the ability to redirect
    input and output streams using operators like `>`, `>>`, and `<`,
    allowing users to redirect stdout and stdin to files or other
    commands.

5.  **Pipelines:** Supporting the use of pipelines, where the output of
    one command serves as the input for another, is done through
    inter-process communication.

6.  **Signal Handling:** Managing signals such as `Ctrl+C` and `Ctrl+Z`
    to terminate or suspend processes properly, ensuring the shell
    remains responsive to user input.

7.  **Built-in Commands:** Developing built-in functions for commonly
    used shell commands that are executed directly within the shell
    without calling external executables.

These components come together to form a functional shell, providing a
simplified yet educational experience of how terminal interfaces operate
at a basic level.

# How the project went

  - lots of reading of documentation of how bash works
  - communication between partners
      - trust between partners heavily on tests
      - communication about behavior heavily relying on tests,
        especially during lexer and parser stage
  - correct parsing of strings is a big part
      - over-engineering in the beginning has been a problem that bid us
        later because of variety of cases that can appear, with the bash
        "laisse-faire" approach of expansions etc.

## Tests and Test Driven Development

## Time management

## How we used Git

# The parts

For detailed information on the
[lexer](https://github.com/Keisn1/minishell/tree/main/src/lexer)'s role
in tokenizing input, the
[parser](https://github.com/Keisn1/minishell/tree/main/src/parser)'s
function in interpreting command structures, and the
[executor](https://github.com/Keisn1/minishell/tree/main/src/executor)'s
job of handling command execution, as well as the setup of the [REPL
(main
execution)](https://github.com/Keisn1/minishell/tree/main/src/main)
please refer to the respective README files.
