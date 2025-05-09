/*
** EPITECH PROJECT, 2025
** builtin_which.c
** File description:
** which builtin unit test
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

TestSuite(which, .description="\e[32mThis test suite test the which builtin\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(which, no_arg)
{
    const char input[] = "which";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("which: Too few arguments.\n");
}

Test(which, no_match)
{
    const char input[] = "which ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("/usr/bin/ls\n");
    cr_assert_stderr_eq_str("");
}

Test(which, builtin)
{
    const char input[] = "which which";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("which: shell built-in command.\n");
    cr_assert_stderr_eq_str("");
}

Test(which, alias)
{
    const char input[] = "alias hey ls; which hey";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hey: \t aliased to ls\n");
    cr_assert_stderr_eq_str("");
}

Test(which, all_at_the_same_time)
{
    const char input[] = "alias echo ls; which echo";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("echo: \t aliased to ls\n");
    cr_assert_stderr_eq_str("");
}

Test(which, multiple_arg)
{
    const char input[] = "which cd ls exit";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("cd: shell built-in command.\n/usr/bin/ls\nexit: shell built-in command.\n");
    cr_assert_stderr_eq_str("");
}

Test(which, not_exist)
{
    const char input[] = "which ohhhhhh";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("ohhhhhh: Command not found.\n");
}
