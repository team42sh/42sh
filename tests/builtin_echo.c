/*
** EPITECH PROJECT, 2025
** builtin_echo.c
** File description:
** echo builtin unit test
*/

#include "core/minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

static int false_main(void)
{
    if (setup_shell() == OK_OUTPUT)
        shell_loop();
    return exit_shell();
}

TestSuite(builtin_echo, .description="\e[32mThis test suite test the echo builtin function\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(builtin_echo, normal_echo)
{
    const char input[] = "echo hello";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hello\n");
    cr_assert_stderr_eq_str("");
}

Test(builtin_echo, echo_with_quotes)
{
    const char input[] = "echo \"test \"";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("test \n");
    cr_assert_stderr_eq_str("");
}

Test(builtin_echo, echo_with_quotes_error)
{
    const char input[] = "echo \"test ";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Unmatched \'\"\'.\n");
}

Test(builtin_echo, echo_with_option)
{
    const char input[] = "echo -n yes";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("yes");
    cr_assert_stderr_eq_str("");
}

Test(builtin_echo, echo_with_spaces)
{
    const char input[] = "echo hello d     d";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hello d d\n");
    cr_assert_stderr_eq_str("");
}

Test(builtin_echo, echo_with_quote_inhib)
{
    const char input[] = "echo \"\\taa\\t\"";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("\taa\t\n");
    cr_assert_stderr_eq_str("");
}
