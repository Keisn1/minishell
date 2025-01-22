#!/usr/bin/env python3


# Ctrl+C = SIGINT
# Ctrl+D = EOF

import pexpect
import re
import time


def remove_cariage(text):
    ansi_escape = re.compile(
        r"""\r""",
        re.VERBOSE,
    )

    return ansi_escape.sub("", text)


def remove_ansi_sequences(text):
    ansi_escape = re.compile(
        r"""
        \x1B  # ESC
        (?:   # 7-bit C1 Control Sequence
            [@-Z\\-_]
        |     # or [ for CSI
            \[
            [0-?]*  # Parameter bytes
            [ -/]*  # Intermediate bytes
            [@-~]   # Final byte
        )
    """,
        re.VERBOSE,
    )

    return ansi_escape.sub("", text)


def test_sigint_in_interactive_mode():
    # Start an interactive shell
    minishell = pexpect.spawn("./minishell", encoding="utf-8")

    # Expect the prompt (this may need to be adjusted depending on your environment)
    minishell.expect(r"\$ ")

    minishell.sendcontrol("c")
    time.sleep(0.01)

    assert minishell.isalive()

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    assert minishell.before is not None
    assert minishell.before[: len("^C")] == "^C"
    assert minishell.before[-len("\r\nkfreyer") :] == "\r\nkfreyer"

    minishell.sendline("echo $?")

    try:
        minishell.expect(r"\$ ", timeout=0.1)
    except pexpect.TIMEOUT:
        print("Timeout occurred")
        assert False, "Timeout occured"

    output = remove_ansi_sequences(minishell.before)
    output = output.split("\n")
    output = [remove_cariage(out) for out in output]
    code = int(output[1])
    assert code == 130

    minishell.sendline("exit")
    minishell.close()


#     print(minishell.before.strip())

#     # Send Ctrl-C to interrupt (send control character)

#     # Expect the command output
#     minishell.expect(r"\$ ")

#     interrupted_output = minishell.before.strip()

# print("Output after sending Ctrl-C:")
# print(interrupted_output)

# # Send the exit command to close the session
# minishell.sendline("exit")
# minishell.close()

# bash = pexpect.spawn("./bash", encoding="utf-8")

# bash.expect(r"\$ ")

# # Send a command to be executed in the shell
# bash.sendline("echo Hello from Bash")
# bash.expect(r"\$ ")
# print(bash.before.strip())

# # Send Ctrl-C to interrupt (send control character)
# bash.sendcontrol("c")

# # Expect the command output
# bash.expect(r"\$ ")

# assert bash.isalive()
# print(bash.isalive())
# print(bash.isatty())
# interrupted_output = bash.before.strip()

# print("Output after sending Ctrl-C:")
# print(interrupted_output)
# # Send the exit command to close the bash session
# bash.sendline("exit")
# bash.close()
