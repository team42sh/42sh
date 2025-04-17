/*
** EPITECH PROJECT, 2025
** src/builtins/unset
** File description:
** This files contains the main core of the unset command
*/

#include "core/minishell.h"

/**
 * @brief Functions to run the 'unset' builtin
 * Can delete a local variable
 *
 * @param argv The arguments passed to the command
 * @return exitcode_t
 */
exitcode_t unset_command(IN char **argv)
{
    if (argv == NULL)
        return ERROR_OUTPUT;
    if (argv[1] == NULL)
        return print_err("unset: Too few arguments.\n");
    for (int i = 1; argv[i] != NULL; i++) {
        if (remove_var(argv[i]))
            return ERROR_OUTPUT;
    }
    return OK_OUTPUT;
}
