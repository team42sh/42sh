/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** prg_path
*/

#include "core/minishell.h"
#include "core/update.h"

/**
 * @brief Get the program full path
 *
 * @param argv The program arguments
 * @return The full path of the program
 */
char *get_prg_full_path(IN char **argv)
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