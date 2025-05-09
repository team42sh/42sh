/*
** EPITECH PROJECT, 2025
** or_and_test.c
** File description:
** or_and test
*/

#include "core/minishell.h"
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
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

TestSuite(and_or_tests, .description="\e[32mThis test suite goal is to test the && and || option\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(and_or_tests, basic_and)
{
    const char input[] = "cd .github ; true && ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("");
}

Test(and_or_tests, basic_and_two)
{
    const char input[] = "cd .github ; false && ls && ls";
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

Test(and_or_tests, basic_and_failed)
{
    const char input[] = "cd .github ; false && ls";
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

Test(and_or_tests, basic_or)
{
    const char input[] = "cd .github ; false || ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("");
}

Test(and_or_tests, basic_or_two)
{
    const char input[] = "cd .github ; true || ls || ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(and_or_tests, basic_or_failed)
{
    const char input[] = "cd .github ; false || cd .github";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str(".github: No such file or directory.\n");
}

Test(and_or_tests, and_no_first_arg)
{
    const char input[] = "cd .github ; && ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("");
}

Test(and_or_tests, and_no_second_arg)
{
    const char input[] = "cd .github ; ls &&";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Invalid null command.\n");
}

Test(and_or_tests, and_no_arg)
{
    const char input[] = "&&";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(and_or_tests, and_lot_of_no_arg)
{
    const char input[] = "&&&&&&&&&&&&&&&&&&&&&&&&&&&";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(and_or_tests, or_no_first_arg)
{
    const char input[] = "cd .github ; || ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Invalid null command.\n");
}

Test(and_or_tests, or_no_second_arg)
{
    const char input[] = "cd .github ; ls ||";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Invalid null command.\n");
}

Test(and_or_tests, or_no_arg)
{
    const char input[] = "&&";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Invalid null command.\n");
}

Test(and_or_tests, or_lot_of_no_arg)
{
    const char input[] = "|||||||||";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Invalid null command.\n");
}

Test(and_or_tests, priority_test)
{
    const char input[] = "cd .github ; ls || pwd && pwd && ls || pwd || pwd";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("");
}

Test(and_or_tests, priority_test_two)
{
    const char input[] = "cd .github ; ls && false || false && ls && true || ls && ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\nCODEOWNERS\nworkflows\nCODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("");
}
