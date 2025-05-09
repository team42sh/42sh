/*
** EPITECH PROJECT, 2025
** builtin_alias.c
** File description:
** alias builtin unit test
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

TestSuite(alias, .description="\e[32mThis test suite test the alias builtin\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(alias, dangerous_alias)
{
    const char input[] = "alias alias hey";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("alias: Too dangerous to alias that.\n");
}

Test(alias, dangerous_alias_two)
{
    const char input[] = "alias unalias hey";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("unalias: Too dangerous to alias that.\n");
}

Test(alias, normal_alias)
{
    const char input[] = "alias help alias; alias";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("help\talias\n");
    cr_assert_stderr_eq_str("");
}

Test(alias, unalias)
{
    const char input[] = "alias help alias; alias hello nop; unalias help; alias";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hello\tnop\n");
    cr_assert_stderr_eq_str("");
}

Test(alias, alias_more_arg)
{
    const char input[] = "alias help \"ls -r\"; alias; cd .github; help";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("help\t(ls -r)\nworkflows\nCODEOWNERS\n");
    cr_assert_stderr_eq_str("");
}
