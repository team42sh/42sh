/*
** EPITECH PROJECT, 2025
** src/shell_manager
** File description:
** This file contains the functions for managing the shell structure
*/

#include "core/minishell.h"

/**
 * @brief Setup every informations for the shell.
 *
 * @param shell                  The shell structure
 */
static void
setup_shell_informations(shell_t *shell)
{
    shell->env = NULL;
    shell->variables = NULL;
    shell->aliases = NULL;
    shell->last_input_buffer = NULL;
    reset_initial_env();
    shell->last_exit_code = 0;
    shell->should_exit = false;
    shell->current_child_pid = -1;
    shell->is_piped_shell = !isatty(STDIN_FILENO);
    shell->saved_stdin = dup(STDIN_FILENO);
    shell->saved_stdout = dup(STDOUT_FILENO);
    shell->vars = create_shell_vars();
}

/**
 * @brief Exit the shell and free everything.
 *
 * @return The final exitcode to return at the end of the shell.
 */
exitcode_t
exit_shell(void)
{
    exitcode_t exit_code = get_shell()->last_exit_code;

    clear_env();
    clear_var();
    clear_aliases();
    free_shell_vars();
    free_null_check(get_shell()->last_input_buffer);
    free_null_check(get_shell());
    return exit_code;
}

/**
 * @brief Get the shell structure using static variable.
 *
 * @return THE SHELL.
 */
shell_t *
get_shell(void)
{
    static shell_t *shell = NULL;

    if (shell == NULL) {
        shell = malloc(sizeof(shell_t));
        if (shell == NULL) {
            print_err("mysh: not enough memory in system to run!\n");
            exit(84);
        }
        setup_shell_informations(shell);
    }
    return shell;
}
