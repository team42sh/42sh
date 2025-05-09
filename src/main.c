/*
** EPITECH PROJECT, 2025
** src/main
** File description:
** Main file for the 42sh project
*/

#include "core/minishell.h"
#include "core/update.h"
#include "build_infos.h"

#include <limits.h>

/**
 * @brief Get the program full path
 *
 * @param argv The program arguments
 * @return The full path of the program
 */
static char *get_prg_full_path(char **argv)
{
    char resolved[PATH_MAX];

    if (argv[0] == NULL)
        return NULL;
    if (argv[0][0] == '/' || argv[0][0] == '.') {
        if (realpath(argv[0], resolved))
            return strdup(resolved);
    }
    return get_binary_path(argv[0]);
}

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
