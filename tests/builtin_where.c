/*
** EPITECH PROJECT, 2025
** builtin_where.c
** File description:
** where builtin unit test
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

TestSuite(where, .description="\e[32mThis test suite test the where builtin\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(where, no_arg)
{
    const char input[] = "where";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("where: Too few arguments.\n");
}

Test(where, slash_in_arg)
{
    const char input[] = "where /";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("where: / in command makes no sense\n");
}

Test(where, no_match)
{
    const char input[] = "where ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("/usr/bin/ls\n/bin/ls\n");
    cr_assert_stderr_eq_str("");
}

Test(where, builtin)
{
    const char input[] = "where where";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("where is a shell built-in\n");
    cr_assert_stderr_eq_str("");
}

Test(where, alias)
{
    const char input[] = "alias hey ls; where hey";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hey is aliased to ls\n");
    cr_assert_stderr_eq_str("");
}

Test(where, all_at_the_same_time)
{
    const char input[] = "alias echo ls; where echo";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("echo is aliased to ls\necho is a shell built-in\n/usr/bin/echo\n/bin/echo\n");
    cr_assert_stderr_eq_str("");
}

Test(where, multiple_arg)
{
    const char input[] = "where cd ls exit";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("cd is a shell built-in\n/usr/bin/ls\n/bin/ls\nexit is a shell built-in\n");
    cr_assert_stderr_eq_str("");
}

Test(where, not_exist)
{
    const char input[] = "where ohhhhhh";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}
