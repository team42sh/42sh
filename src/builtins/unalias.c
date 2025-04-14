/*
** EPITECH PROJECT, 2025
** src/builtins/alias.c
** File description:
** UNALIAS COMMAND
*/

#include "core/minishell.h"

/*
 * UNALIAS COMMAND - BUILT-IN
 * No args = Error message
 * 1+ arg(s) = Remove the aliases
 * * = Remove all the aliases
 */
exitcode_t unalias_command(char **argv)
{
    if (argv[1] == NULL)
        return print_err("%s: Too few arguments.\n", argv[0]);
    for (int i = 1; argv[i] != NULL; i++) {
        if (!my_strcmp(argv[i], "*")) {
            clear_aliases();
            return OK_OUTPUT;
        }
        remove_alias(argv[i]);
    }
    return OK_OUTPUT;
}
