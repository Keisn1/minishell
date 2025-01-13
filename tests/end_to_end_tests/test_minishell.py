#!/usr/bin/env python3
import pytest

from tests.end_to_end_tests.conftest import get_prompt_minishell, start_process


@pytest.mark.parametrize(
    "cmd",
    [
        (["ls -a", "ls -l"]),
        (["cat CMakeLists.txt"]),
        (["echo $PATH"]),
        (["echo asdf $PATH asdf"]),
        (["echo  asdf    $PATH   asdf  "]),
        (['echo "$PATH"']),
    ],
)
def test_minishell(cmd):
    prompt = get_prompt_minishell()
    bash = start_process("bash")
    minishell = start_process("./minishell")

    assert bash.stdin is not None
    assert minishell.stdin is not None

    cmds = "\n".join(cmd + ["echo $?\n"])

    stdout_bash, _ = bash.communicate(cmds.encode())
    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())

    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line
    stdout_minishell = [
        line
        for line in stdout_minishell.decode().split("\n")
        if not (line.startswith(prompt) or line.startswith("heredoc>"))
    ]
    stderr_minishell = stderr_minishell.decode()

    assert "ERROR" not in stdout_minishell
    assert "ERROR" not in stderr_minishell
    assert len(stdout_bash) == len(stdout_minishell)
    assert len(stderr_minishell) == 0
    for out1, out2 in zip(stdout_bash, stdout_minishell):
        assert out1 == out2, f"{out1} != {out2}"
