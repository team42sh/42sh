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

TestSuite(minishell_one, .description="\e[32mThis test suite goal is to do the same test as the"
    " minishell 1\n\e[32mIt mostly test basic core function of the 42sh\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(minishell_one, empty_input)
{
    const char input[] = "";
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

Test(minishell_one, simple_ls)
{
    const char input[] = "cd .github\nls";
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

Test(minishell_one, command_with_tabs)
{
    const char input[] = "\t\t\t\tcd\t\t\t\t.github\t\t\t\t\n\t\t\tls\t\t\t\t";
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

Test(minishell_one, command_with_space)
{
    const char input[] = "                           cd                .github                      \n                            ls                                ";
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

Test(minishell_one, command_tabs_and_space)
{
    const char input[] = "\t\t\t     \t    \tcd  \t\t\t     \t.github    \t \t \t\n            \t\t\tls  \t\t     \t\t  \t \t     ";
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

Test(minishell_one, command_with_newline)
{
    const char input[] = "\n    \n\t\n\n\n\n\n\t\ncd .github\n\t\t\nls\n\n";
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

Test(minishell_one, wrong_command)
{
    const char input[] = "aaaa\nbbbb";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("aaaa: Command not found.\nbbbb: Command not found.\n");
}

Test(minishell_one, wrong_builtin)
{
    const char input[] = "cd oo\nenv";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("oo: No such file or directory.\n");
}

Test(minishell_one, wrong_command_with_builtin)
{
    const char input[] = "aaaaaa\ncd oo\nenv";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("aaaaaa: Command not found.\noo: No such file or directory.\n");
}

Test(minishell_one, empty_env)
{
    const char input[] = "env";
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

Test(minishell_one, set_env_basic)
{
    const char input[] = "setenv yes\nsetenv ok oui\nsetenv vouis 194\nsetenv okidoki pourquoi\nenv";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("yes=\nok=oui\nvouis=194\nokidoki=pourquoi\n");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, unsetenv_basic)
{
    const char input[] = "setenv yes ; env ; unsetenv yes; env";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("yes=\n");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, unsetenv_error)
{
    const char input[] = "unsetenv yes yes hello whynot that is a lot of variable; unsetenv; cd .github\nls";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("unsetenv: Too few arguments.\n");
}

Test(minishell_one, unsetenv_mutliple)
{
    const char input[] = "setenv hello; setenv you; setenv not_me; unsetenv hello you; env";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("not_me=\n");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, set_env_advanced, .description = "require ';' to work")
{
    const char input[] = "setenv 152;setenv _ ok ;setenv zd zd dz;setenv ok {};setenv oui $$;setenv";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("_=ok\nok={}\noui=4969\n");
    cr_assert_stderr_eq_str("setenv: Variable name must begin with a letter.\nsetenv: Too many arguments.\n");
}

Test(minishell_one, set_env_advanced_2)
{
    const char input[] = "setenv a% d; cd .github\nls";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS\nworkflows\n");
    cr_assert_stderr_eq_str("setenv: Variable name must contain alphanumeric characters.\n");
}

Test(minishell_one, cd_error)
{
    const char input[] = "cd; cd ~; cd /thisisnotafile; cd ./Makefile; cd -; cd dz dz; cd /tmp/no_access; setenv HOME /tmp/no_access; cd ~; cd";
    int pipes[2];
    struct stat st = {0};

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    remove("/tmp/no_access");
    cr_assert(stat("/tmp/no_access", &st) == -1);
    mkdir("/tmp/no_access", 0000);

    cr_assert(false_main() == 1);

    remove("/tmp/no_access");

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("cd: No home directory.\nNo $home variable set.\n/thisisnotafile: No such file or directory.\n./Makefile: Not a directory.\n: No such file or directory.\ncd: Too many arguments.\n/tmp/no_access: Permission denied.\n/tmp/no_access: Permission denied.\ncd: Can't change to home directory.\n");
}

Test(minishell_one, edge_case_home_cd)
{
    const char input[] = "setenv HOME /tmp/access; unsetenv HOME; cd; /bin/ls";
    int pipes[2];
    struct stat st = {0};

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    remove("/tmp/access");
    cr_assert(stat("/tmp/access", &st) == -1);
    mkdir("/tmp/access", 0777);

    cr_assert(false_main() == 0);

    remove("/tmp/access");

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, cd_home_user)
{
    const char input[] = "cd ~mail/../../tmp/access; ls";
    int pipes[2];
    struct stat st = {0};

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    remove("/tmp/access");
    cr_assert(stat("/tmp/access", &st) == -1);
    mkdir("/tmp/access", 0777);

    cr_assert(false_main() == 0);

    remove("/tmp/access");

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
}

Test(minishell_one, segv_prog)
{
    const char input[] = "./tests/binary_test/segv_e";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(chmod(input, strtol("0777", NULL, 8)) >= 0);

    cr_assert(false_main() == 139);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Segmentation fault (core dumped)\n");
}

Test(minishell_one, floating_point_error)
{
    const char input[] = "./tests/binary_test/float_e";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(chmod(input, strtol("0777", NULL, 8)) >= 0);

    cr_assert(false_main() == 136);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Floating exception (core dumped)\n");
}

Test(minishell_one, no_elf)
{
    const char input[] = "./tests/binary_test/no_elf";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(chmod(input, strtol("0777", NULL, 8)) >= 0);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("./tests/binary_test/no_elf: Exec format error. Binary file not executable.\n");
}

Test(minishell_one, no_elf_no_env)
{
    const char input[] = "./tests/binary_test/no_elf";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(chmod(input, strtol("0777", NULL, 8)) >= 0);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("./tests/binary_test/no_elf: Exec format error. Wrong Architecture.\n");
}

Test(minishell_one, no_perm)
{
    const char input[] = "./tests/binary_test/no_exe";
    int pipes[2];

    clearenv();
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(chmod(input, strtol("0664", NULL, 8)) >= 0);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("./tests/binary_test/no_exe: Permission denied.\n");
}
