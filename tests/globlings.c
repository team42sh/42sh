/*
** EPITECH PROJECT, 2025
** globlings.c
** File description:
** globlings unit test
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

TestSuite(globlings, .description="\e[32mThis test suite test the globlings\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);


Test(globlings, basic_globling)
{
    const char input[] = "ls .github/*";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str(".github/CODEOWNERS\n\n.github/workflows:\nchocolatine.yml\ndocker.yml\nrelease.yml\n");
    cr_assert_stderr_eq_str("");
}

Test(globlings, advanced_globling)
{
    const char input[] = "ls .g??h?b/*k*";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("chocolatine.yml\ndocker.yml\nrelease.yml\n");
    cr_assert_stderr_eq_str("");
}

Test(globlings, no_match)
{
    const char input[] = "ls ?";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("ls: No match.\n");
}

Test(globlings, test_space_files)
{
    const char input[] = "ls ./tests/binary_test/*pace*";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("./tests/binary_test/file space:\ntest space.txt\n\n./tests/binary_test/space_two:\nnothing%&*yes\n");
    cr_assert_stderr_eq_str("");
}
