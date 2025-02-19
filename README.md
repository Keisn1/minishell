![](cover-minishell-bonus.png)

<p align="center">
<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/Keisn1/minishell?color=blueviolet" />
<img alt="GitHub top language" src="https://img.shields.io/github/languages/top/Keisn1/minishell?color=blue" />
<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/Keisn1/minishell?color=brightgreen" />
<img alt="GitHub Lines of Code" src="https://tokei.rs/b1/github/Keisn1/minishell?category=code" />
</p>

# Table of Content <span class="tag" data-tag-name="TOC_2"><span class="smallcaps">TOC\_2</span></span>

  - [What is Minishell?](#what-is-minishell)
  - [Building Blocks](#building-blocks)
      - [Lexer](#lexer)
      - [Parser](#parser)
      - [Executor](#executor)
  - [How the project went](#how-the-project-went)
      - [Tests and Test Driven
        Development](#tests-and-test-driven-development)
      - [Time management](#time-management)
      - [How we used Git](#how-we-used-git)

# What is Minishell?

The Minishell project at 42 School is a programming assignment where
students are tasked with creating a simple shell, oriented and similar
bash. This project helps students understand how a shell works,
including aspects like command execution, input/output redirection, and
process handling. The primary goals are to learn about parsing commands,
managing environments and processes, and implementing built-in shell
functions. It requires knowledge of system calls and a good grasp of C
programming.

The Minishell project being a group project at 42 School introduces
students to collaboration and working in teams, which is an essential
skill in software development. Here are some aspects and challenges
associated with this aspect:

1.  **Communication:** Clear and effective communication is crucial.
    Team members must regularly discuss their progress, challenges, and
    any changes to the project plan. (see
    [4.1](#*Tests%20and%20Test%20Driven%20Development))

2.  **Version Control:** Utilizing tools like Git helps manage changes
    and contributions from different team members. It also helps resolve
    merge conflicts and track project history. (see
    [4.3](#*How%20we%20used%20Git))

3.  **Task Division:** Identifying each member's strengths and dividing
    tasks accordingly can optimize productivity. This includes
    delegating tasks like parsing, built-in command implementation, and
    handling processes. (see [3](#Building%20Blocks))

4.  **Consistency:** Ensuring that code style and project structure
    remain consistent across the work of different team members can be
    difficult but is important for maintainability. (see
    [4.1](#*Tests%20and%20Test%20Driven%20Development))

5.  **Time Management:** Coordinating schedules and setting realistic
    deadlines is necessary to ensure that the project progresses
    smoothly without last-minute rushes. (see
    [4.2](#*Time%20management))

Overall, the group aspect of the Minishell project teaches students not
only technical skills but also soft skills, such as teamwork and
conflict resolution, which are critical in real-world software
development.

# Building Blocks

## Lexer

  - main idea is to have a line container

## Parser

  - developed in a test driven manner
  - AST is binary tree
      - apart from logical Nodes, the nodes are left heavy

## Executor

  - first executing a command in and of itself
      - expansions
      - redirections
      - arguments
      - builtins
      - signals
  - then executing pipelines
  - then executing logical nodes

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
