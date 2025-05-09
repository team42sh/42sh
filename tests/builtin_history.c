/*
** EPITECH PROJECT, 2025
** builtin_history.c
** File description:
** history builtin unit test
*/

#include "core/minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdbool.h>
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

TestSuite(history, .description="\e[32mThis test suite test the history builtin\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(history, basic_test)
{
    const char input[] = "history -c\nhistory -h";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("history -h\n");
    cr_assert_stderr_eq_str("");
}

Test(history, number)
{
    const char input[] = "history -c\necho -n\necho -n\necho -n\necho -n\necho -n\nhistory -h 3";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("echo -n\necho -n\nhistory -h 3\n");
    cr_assert_stderr_eq_str("");
}

Test(history, number_reverse)
{
    const char input[] = "history -c\necho -n\necho -n\necho -n\necho -n\necho -n\nhistory -hr 3";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("history -hr 3\necho -n\necho -n\n");
    cr_assert_stderr_eq_str("");
}

Test(history, wrong_number)
{
    const char input[] = "history -1";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Usage: history [-chr] [# number of events].\n");
}

Test(history, str_number)
{
    const char input[] = "history ziqud";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("history: Badly formed number.\n");
}
