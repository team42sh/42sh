/*
** EPITECH PROJECT, 2025
** minishell_one.c
** File description:
** test of the minishell one
*/

#include "core/minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
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

TestSuite(minishell_one, .description="\e[32mThis test suite goal is to do the same test as the"
    "minishell 1\n\e[32mIt mostly test basic core function of the 42sh\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(minishell_one, empty_input)
{
    const char *input = "";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    false_main();

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, simple_ls)
{
    const char *input = "cd .github\nls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    false_main();

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, wrong_command)
{
    const char *input = "aaaa\nbbbb";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    false_main();

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("aaaa: Command not found.\nbbbb: Command not found.\n");
}

Test(minishell_one, empty_env)
{
    const char *input = "env";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    false_main();

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, set_env_basic)
{
    const char *input = "setenv yes\nsetenv ok oui\nsetenv vouis 194\nsetenv okidoki pourquoi\nenv";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    false_main();

    cr_assert_stdout_eq_str("yes=\nok=oui\nvouis=194\nokidoki=pourquoi\n");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, set_env_advanced, .description = "require ';' to work")
{
    const char *input = "setenv 152;setenv _ ok ;setenv zd zd dz;setenv ok {};setenv oui $$;setenv";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    false_main();

    cr_assert_stdout_eq_str("_=ok\nok={}\noui=4969\n");
    cr_assert_stderr_eq_str("setenv: Variable name must begin with a letter.\nsetenv: Too many arguments.\n");
}

Test(minishell_one, set_env_advanced_2)
{
    const char *input = "setenv a% d;setenv";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    false_main();

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("setenv: Variable name must contain alphanumeric characters.\n");
}

Test(minishell_one, cd_error)
{
    const char *input = "cd; cd ~; cd /thisisnotafile; cd ./Makefile; cd -; cd dz dz; cd /tmp/no_access; setenv HOME /tmp/no_access; cd ~; cd";
    int pipes[2];
    struct stat st = {0};

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(stat("/tmp/no_access", &st) == -1);
    mkdir("/tmp/no_access", 0000);

    false_main();

    remove("/tmp/no_access");

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("cd: No home directory.\nNo $home variable set.\n/thisisnotafile: No such file or directory.\n./Makefile: Not a directory.\n: No such file or directory.\ncd: Too many arguments.\n/tmp/no_access: Permission denied.\n/tmp/no_access: Permission denied.\ncd: Can't change to home directory.\n");
}