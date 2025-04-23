/*
** EPITECH PROJECT, 2025
** main_loop.c
** File description:
** main loop function
*/

#include "core/builtins.h"
#include "core/minishell.h"

/*
 * Declaration of all BUILT IN commands.
 * (extern variable from builtins.h).
 */
const builtin_t BUILTINS[] = {
    {"exit", &exit_command},
    {"printenv", &env_command},
    {"env", &env_command},
    {"set", &set_command},
    {"setenv", &setenv_command},
    {"unsetenv", &unsetenv_command},
    {"unset", &unset_command},
    {"cd", &cd_command},
    {"alias", &alias_command},
    {"unalias", &unalias_command},
    {"echo", &echo_command},
    {"where", &where_function},
    {"which", &which_function},
    {"history", &history_command},
    {"!", &history_command},
    {NULL, NULL}
};

/**
 * @brief Get user input by choosing if we use termios or getline()
 *
 * @return NULL if reached the end of the file or the input.
 */
static char *get_user_input(void)
{
    size_t line_size = 0;

    if (isatty(STDIN_FILENO)) {
        get_shell()->last_input_buffer = termios_get_input();
        return get_shell()->last_input_buffer;
    }
    if (getline(&get_shell()->last_input_buffer, &line_size, stdin) == -1)
        return NULL;
    remove_newline(get_shell()->last_input_buffer);
    return get_shell()->last_input_buffer;
}

/*
 * Loop function for the main function of the shell.
 * Without this you shell will not work :)
 * The return value of this function is useless.
 */
int shell_loop(void)
{
    exitcode_t e_ret = 0;
    shell_t *shell = get_shell();

    if (get_user_input() == NULL)
        return exit_command(NULL);
    if (shell->last_input_buffer[0] == '\0')
        return shell_loop();
    write_command_history(shell->last_input_buffer);
    e_ret = shell_execute(tokenize_line(shell->last_input_buffer));
    if (e_ret == CURRENTLY_CHILD || shell->should_exit)
        return OK_OUTPUT;
    free_null_check(shell->vars->github_repository);
    shell->vars->github_repository = get_github_repository_name();
    shell->_max_history++;
    shell->_history_input = add_string(shell->_history_input,
        shell->last_input_buffer);
    return shell_loop();
}

/*
 * Setup the shell and do some misc stuff to initialize it.
 * Doing the first init of the shell structure.
 * Doing the init of the .myshrc if the shell is in tty mode.
 * Doing the setup of signals. A.K.A supporting the Ctrl+C.
 */
int setup_shell(void)
{
    get_shell();
    if (isatty(STDIN_FILENO)) {
        if (load_myshrc() == CURRENTLY_CHILD)
            return CURRENTLY_CHILD;
        init_termios();
    }
    setup_shell_signals();
    return OK_OUTPUT;
}
