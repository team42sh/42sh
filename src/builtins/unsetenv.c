/*
** EPITECH PROJECT, 2025
** src/commands/unsetenv
** File description:
** Unsetenv COMMAND
*/

#include "core/minishell.h"

/*
 * UNSETENV COMMAND - BUILT-IN
 * This command will delete an environment variable using their key.
 * Can be used with multiples variables.
 * For example : unsetenv PATH HOME WORKDIR
 * Can be used with the wildcard '*' to unset the entire environment.
 * (MIGHT BE DELETED).
 */
exitcode_t unsetenv_command(char **argv)
{
    if (argv[1] == NULL)
        return print_err("unsetenv: Too few arguments.\n");
    for (int i = 1; argv[i] != NULL; i++) {
        remove_env(argv[i]);
    }
    return OK_OUTPUT;
}
