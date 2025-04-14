/*
** EPITECH PROJECT, 2025
** src/main
** File description:
** Main file for the minishell project
*/

#include "core/minishell.h"

/*
 * Declaration of all BUILT IN commands.
 * (extern variable from builtins.h).
 */
const builtin_t BUILTINS[] = {
    {"exit", &exit_command},
    {"printenv", &env_command},
    {"env", &env_command},
    {"setenv", &setenv_command},
    {"unsetenv", &unsetenv_command},
    {"cd", &cd_command},
    {"alias", &alias_command},
    {"unalias", &unalias_command},
    {NULL, NULL}
};

/*
 * Loop function for the main function of the shell.
 * Without this you shell will not work :)
 * The return value of this function is useless.
 */
static int shell_loop(void)
{
    size_t line_size = 0;
    exitcode_t e_ret = 0;

    while (1) {
        if (isatty(STDIN_FILENO))
            print_shell_prompt();
        if (getline(&get_shell()->last_input_buffer, &line_size, stdin) == -1)
            return exit_command(NULL);
        remove_newline(get_shell()->last_input_buffer);
        write_command_history(get_shell()->last_input_buffer);
        e_ret = shell_execute(tokenize_line(get_shell()->last_input_buffer));
        if (e_ret == CURRENTLY_CHILD || get_shell()->should_exit)
            return OK_OUTPUT;
        free_null_check(get_shell()->vars->github_repository);
        get_shell()->vars->github_repository = get_github_repository_name();
    }
}

/*
 * Setup the shell and do some misc stuff to initialize it.
 * Doing the first init of the shell structure.
 * Doing the init of the .myshrc if the shell is in tty mode.
 * Doing the setup of signals. A.K.A supporting the Ctrl+C.
 */
static int setup_shell(void)
{
    get_shell();
    if (isatty(STDIN_FILENO))
        if (load_myshrc() == CURRENTLY_CHILD)
            return CURRENTLY_CHILD;
    setup_shell_signals();
    return OK_OUTPUT;
}

/*
 * Main function for minishell project
 * Show the prompt and execute the command when input in the buffer are been
 * added.
 *
 * Calling the function get_shell() one time before the loop to init all values
 * Then call setup_shell_signals to put the pointer of function for signals
 * like Ctrl + D or Ctrl + C.
 */
int main(void)
{
    if (setup_shell() == OK_OUTPUT)
        shell_loop();
    return exit_shell();
}
