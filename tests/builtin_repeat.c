/*
** EPITECH PROJECT, 2025
** builtin_repeat.c
** File description:
** repeat builtin unit test
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

TestSuite(repeat, .description="\e[32mThis test suite test the repeat builtin\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(repeat, no_arg)
{
    const char input[] = "repeat";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("repeat: Too few arguments.\n");
}

Test(repeat, basic_repeat)
{
    const char input[] = "repeat 5 ls Makefile";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("Makefile\nMakefile\nMakefile\nMakefile\nMakefile\n");
    cr_assert_stderr_eq_str("");
}
