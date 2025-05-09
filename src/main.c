/*
** EPITECH PROJECT, 2025
** src/main
** File description:
** Main file for the 42sh project
*/

#include "core/minishell.h"
#include "core/update.h"
#include "build_infos.h"

/**
 * @brief Main function for 42sh project.
 *
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Final exit code.
 */
int main(IN int argc __attribute__((unused)), IN char **argv)
{
    char *prg_path = get_prg_full_path(argv);

    if (setup_shell() == OK_OUTPUT) {
        if (SHOW_UPDATES && isatty(STDIN_FILENO))
            check_for_updates(prg_path);
        shell_loop();
    }
    free(prg_path);
    return exit_shell();
}
