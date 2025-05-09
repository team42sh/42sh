/*
** EPITECH PROJECT, 2025
** local_variable.c
** File description:
** local variable unit test
*/

#include "core/minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
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

TestSuite(local_variable, .description="\e[32mThis test suite test the local variable\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(local_variable, basic_set)
{
    const char input[] = "set hello=yes; set okay=no; unset okay; set";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hello\tyes\npath\t(/usr/bin /bin)\n");
    cr_assert_stderr_eq_str("");
}

Test(local_variable, error_set)
{
    const char input[] = "set 3";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("set: Variable name must begin with a letter.\n");
}

Test(local_variable, unset_nothing)
{
    const char input[] = "unset";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("unset: Too few arguments.\n");
}

Test(local_variable, unset_not_exist)
{
    const char input[] = "unset abababa";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(local_variable, multiple_set)
{
    const char input[] = "set a=b; set a=c; set";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("a\tc\npath\t(/usr/bin /bin)\n");
    cr_assert_stderr_eq_str("");
}

Test(local_variable, multiple_set_readonly)
{
    const char input[] = "set -r a=b; set a=c";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("set: $a is read-only.\n");
}

Test(local_variable, unset_readonly)
{
    const char input[] = "set -r a=b; unset a";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("unset: $a is read-only.\n");
}

Test(local_variable, set_readonly)
{
    const char input[] = "set -r a=b; set -r";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("a\tb\n");
    cr_assert_stderr_eq_str("");
}

Test(local_variable, use_var)
{
    const char input[] = "set command=ls; cd .github ; $command";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("");
}

Test(local_variable, var_inside_string)
{
    const char input[] = "set command=ch; \"e\"${command}\"o\" hello";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hello\n");
    cr_assert_stderr_eq_str("");
}

Test(local_variable, illegal_name)
{
    const char input[] = "set 2";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("set: Variable name must begin with a letter.\n");
}

Test(local_variable, illegal_name_two)
{
    const char input[] = "set a+";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("set: Variable name must contain alphanumeric characters.\n");
}

Test(local_variable, not_defined)
{
    const char input[] = "echo &{aaa}; echo &ppp";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("aaa: Undefined variable.\nppp: Undefined variable.\n");
}

Test(local_variable, env_var)
{
    const char input[] = "setenv PATH \"hehe haha\"; echo $PATH";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("hehe haha\n");
    cr_assert_stderr_eq_str("");
}
