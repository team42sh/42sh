/*
** EPITECH PROJECT, 2025
** minishell_two.c
** File description:
** test of the minishell two
*/

#include "core/minishell.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/logging.h>
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

TestSuite(minishell_two, .description="\e[32mThis test suite goal is to do the same test as the"
    " minishell 2\n\e[32mIt mostly test different redirection of the 42sh\e[0m",
    .disabled = false, .timeout = 10., .init = redirect_all_std);

Test(minishell_two, separator_complex)
{
    const char input[] = "cd .github\n;;;;\n;;;;\n\n\n\n\n;;;;;;;ls";
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

Test(minishell_two, big_input)
{
    const char input[] = "ls -lR | cat -e";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);
}

Test(minishell_two, ls_redirect)
{
    const char input[] = "cd .github ; ls | cat -e";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("CODEOWNERS$\nworkflows$\n");
    cr_assert_stderr_eq_str("");
}

static void remove_file_one(void)
{
    remove("../tests/binary_test/redirect.txt");
}

Test(minishell_two, ls_redirect_into_file, .fini = remove_file_one)
{
    const char input[] = "cd .github;ls > ../tests/binary_test/redirect.txt";
    int pipes[2];
    FILE *file = NULL;

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    file = fopen("../tests/binary_test/redirect.txt", "r");
    cr_assert_not_null(file);
    cr_file_match_str(file, "CODEOWNERS\nworkflows\n");
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    fclose(file);
}

Test(minishell_two, cat_redirect_input)
{
    const char input[] = "cat < .github/CODEOWNERS";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("* @team42sh/normsguard\n");
    cr_assert_stderr_eq_str("");
}

Test(minishell_two, cat_heredocs)
{
    const char input[] = "cat << test\nyes\nokok\nptdr\ntest \ntesting\ngrostest\ntest";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    cr_assert_stdout_eq_str("yes\nokok\nptdr\ntest \ntesting\ngrostest\n");
    cr_assert_stderr_eq_str("");
}

Test(minishell_two, error_no_heredocs)
{
    const char input[] = "cat <<";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_heredocs_no_command)
{
    const char input[] = "<< yes";
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

Test(minishell_two, error_heredocs_nothing)
{
    const char input[] = "<<";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_pipe_first_arg)
{
    const char input[] = "| ls";
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

Test(minishell_two, error_pipe_second_arg)
{
    const char input[] = "ls |";
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

Test(minishell_two, error_pipe_all_arg)
{
    const char input[] = "|";
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

Test(minishell_two, error_pipe_all_arg_multiple)
{
    const char input[] = "||||||||||||;ls";
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

Test(minishell_two, error_pipe_not_command)
{
    const char input[] = "pt | pt";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("pt: Command not found.\npt: Command not found.\n");
}

Test(minishell_two, error_pipe_not_command_exept_first)
{
    const char input[] = "ls | pt";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("pt: Command not found.\n");
}

static void remove_file_two(void)
{
    remove("test_one");
}

Test(minishell_two, error_redirection_no_command, .fini=remove_file_two)
{
    const char input[] = "> test_one";
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

Test(minishell_two, error_redirection_no_redirect)
{
    const char input[] = "ls >";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_redirection_nothing)
{
    const char input[] = ">";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_redirection_directory)
{
    const char input[] = "ls > ./tests";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("./tests: Is a directory.\n");
}

Test(minishell_two, error_redirection_not_possible)
{
    const char input[] = "ls > opzqjdozjq/tests";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("opzqjdozjq/tests: No such file or directory.\n");
}

static void remove_file_three(void)
{
    remove("./testing/help");
    remove("./testing");
}

Test(minishell_two, error_redirection_no_perm, .fini=remove_file_three)
{
    const char input[] = "ls > ./testing/help";
    int pipes[2];

    if (getuid() == 0) {
        cr_log_warn("You are root, the actual test cannot try to check the perm, it is skipped");
        cr_skip();
    }
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    mkdir("./testing", 0000);
    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("./testing/help: Permission denied.\n");
}

Test(minishell_two, error_input_no_file)
{
    const char input[] = "ls <";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_input_nothing)
{
    const char input[] = "<";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_input_no_command)
{
    const char input[] = "< Makefile";
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

Test(minishell_two, error_input_directory)
{
    const char input[] = "cat < tests";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("cat: -: Is a directory\n");
}

Test(minishell_two, error_input_not_real)
{
    const char input[] = "cat < yes";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("yes: No such file or directory.\n");
}

Test(minishell_two, error_input_no_perm)
{
    const char input[] = "cat < testing/yes";
    int pipes[2];

    if (getuid() == 0) {
        cr_log_warn("You are root, the actual test cannot try to check the perm, it is skipped");
        cr_skip();
    }
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    mkdir("./testing", 0000);
    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("testing/yes: Permission denied.\n");
}

static void remove_file_four(void)
{
    remove("yay_test");
}

Test(minishell_two, double_simple_redirect, .fini=remove_file_four)
{
    const char input[] = "/bin/echo blabla > yay_test\n/bin/echo hehe > yay_test";
    int pipes[2];
    FILE *file;

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    file = fopen("yay_test", "r");
    cr_assert_not_null(file);
    cr_file_match_str(file, "hehe\n");
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    fclose(file);
}

static void remove_file_five(void)
{
    remove("append_test");
}

Test(minishell_two, double_double_redirect, .fini=remove_file_five)
{
    const char input[] = "/bin/echo blabla >> append_test\n/bin/echo hehe >> append_test";
    int pipes[2];
    FILE *file;

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    file = fopen("append_test", "r");
    cr_assert_not_null(file);
    cr_file_match_str(file, "blabla\nhehe\n");
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    fclose(file);
}

static void remove_file_six(void)
{
    remove("test_file");
}

Test(minishell_two, error_double_redirection_no_command, .fini=remove_file_six)
{
    const char input[] = ">> test_file";
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

Test(minishell_two, error_double_redirection_no_redirect)
{
    const char input[] = "ls >>";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_double_redirection_nothing)
{
    const char input[] = ">>";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Missing name for redirect.\n");
}

Test(minishell_two, error_double_redirection_directory)
{
    const char input[] = "ls >> ./tests";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("./tests: Is a directory.\n");
}

Test(minishell_two, error_souble_redirection_not_possible)
{
    const char input[] = "ls >> opzqjdozjq/tests";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("opzqjdozjq/tests: No such file or directory.\n");
}

static void remove_file_seven(void)
{
    remove("./other_test/help");
    remove("./other_test");
}

Test(minishell_two, error_double_redirection_no_perm, .fini=remove_file_seven)
{
    const char input[] = "ls >> ./other_test/help";
    int pipes[2];

    if (getuid() == 0) {
        cr_log_warn("You are root, the actual test cannot try to check the perm. It is skipped");
        cr_skip();
    }
    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    mkdir("./other_test", 0000);
    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("./other_test/help: Permission denied.\n");
}

static void remove_file_eight(void)
{
    remove("combinaison");
}

Test(minishell_two, combinaison, .fini=remove_file_eight)
{
    const char input[] = "/bin/echo hello | cat -e > combinaison; cat < combinaison | cat -e >> combinaison";
    int pipes[2];
    FILE *file;

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 0);

    file = fopen("combinaison", "r");
    cr_assert_not_null(file);
    cr_file_match_str(file, "hello$\nhello$$\n");
    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("");
    fclose(file);
}

static void remove_unwanted(void)
{
    remove("unwanted");
}

Test(minishell_two, ambiguous_redirect_double_output, .fini=remove_unwanted)
{
    const char input[] = "ls > unwanted >> unwanted";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Ambiguous output redirect.\n");
}

Test(minishell_two, ambiguous_redirect_double_input, .fini=remove_unwanted)
{
    const char input[] = "ls < unwanted << unwanted";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Ambiguous input redirect.\n");
}

Test(minishell_two, ambiguous_redirect_double_input_pipe)
{
    const char input[] = "ls | ls << test";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Ambiguous input redirect.\n");
}

Test(minishell_two, ambiguous_redirect_input_pipe)
{
    const char input[] = "ls | ls < test";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Ambiguous input redirect.\n");
}

Test(minishell_two, ambiguous_redirect_output_and_pipe, .fini=remove_unwanted)
{
    const char input[] = "ls > unwanted | ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Ambiguous output redirect.\n");
}

Test(minishell_two, ambiguous_redirect_double_output_and_pipe, .fini=remove_unwanted)
{
    const char input[] = "ls >> unwanted | ls";
    int pipes[2];

    cr_assert(pipe(pipes) == 0);
    dprintf(pipes[1], "%s", input);
    close(pipes[1]);
    dup2(pipes[0], 0);
    close(pipes[0]);

    cr_assert(false_main() == 1);

    cr_assert_stdout_eq_str("");
    cr_assert_stderr_eq_str("Ambiguous output redirect.\n");
}

Test(minishell_two, no_command_sandwich, .fini=remove_unwanted)
{
    const char input[] = "ls |> unwanted";
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

Test(minishell_two, program_inside_itself)
{
    const char input[] = "./42sh\ncd .github ; ls";
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