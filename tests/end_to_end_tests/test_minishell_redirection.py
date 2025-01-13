import pytest
import os
from conftest import start_process, get_prompt_minishell


@pytest.mark.parametrize(
    "cmd",
    [(['echo "hello" > tests/end_to_end_tests/test_files/non_existent.txt'])],
)
def test_out_redirections(cmd):
    bash = start_process("bash")
    assert bash.stdin is not None

    cmds = "\n".join(cmd + ["echo $?\n"])
    stdout_bash, _ = bash.communicate(cmds.encode())
    stdout_bash = stdout_bash.decode().split("\n")[:-1]  # cut empty line

    with open("tests/end_to_end_tests/test_files/non_existent.txt", "r") as f:
        file_bash = f.readlines()
    os.remove("tests/end_to_end_tests/test_files/non_existent.txt")

    prompt = get_prompt_minishell()
    minishell = start_process("./minishell")
    assert minishell.stdin is not None
    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())

    with open("tests/end_to_end_tests/test_files/non_existent.txt", "r") as f:
        file_minishell = f.readlines()
    os.remove("tests/end_to_end_tests/test_files/non_existent.txt")

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

    assert len(file_bash) == len(file_minishell)
    for out1, out2 in zip(file_bash, file_minishell):
        assert out1 == out2, f"{out1} != {out2}"


@pytest.mark.parametrize(
    "cmd",
    [
        (["wc -c < tests/end_to_end_tests/test_files/input1.txt"]),
        (["wc -c < tests/end_to_end_tests/test_files/input2.txt"]),
        (["< tests/end_to_end_tests/test_files/input2.txt"]),
        (["< tests/end_to_end_tests/test_files/input2.txt wc -c"]),
    ],
)
def test_in_redirections(cmd):
    minishell = start_process("./minishell")
    prompt, _ = minishell.communicate()
    prompt = prompt.decode()

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


@pytest.mark.parametrize(
    "cmd, err_msg",
    [
        (["wc < asdf"], "No such file or directory"),
        (
            ["wc < tests/end_to_end_tests/test_files/input1.txt > error/error"],
            "No such file or directory",
        ),
        (["<"], "parsing error redirection"),
    ],
)
def test_redirection_errors(cmd, err_msg):
    minishell = start_process("./minishell")

    assert minishell.stdin is not None
    cmds = "\n".join(cmd + ["echo $?\n"])

    stdout_minishell, stderr_minishell = minishell.communicate(cmds.encode())
    stdout_minishell = stderr_minishell.decode()
    stderr_minishell = stderr_minishell.decode()
    assert "ERROR" not in stdout_minishell
    assert "ERROR" not in stderr_minishell
    assert len(stderr_minishell) != 0
    assert err_msg in stderr_minishell