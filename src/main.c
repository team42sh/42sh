/*
** EPITECH PROJECT, 2025
** src/main
** File description:
** Main file for the 42sh project
*/

#include "core/minishell.h"

/**
 * @brief Main function for 42sh project.
 *
 * @return Final exit code.
 */
int main(void)
{
    if (setup_shell() == OK_OUTPUT)
        shell_loop();
    return exit_shell();
}
