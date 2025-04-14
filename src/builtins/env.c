/*
** EPITECH PROJECT, 2025
** src/commands/env
** File description:
** This files contains the main core of env command
*/

#include "core/minishell.h"

/*
 * ENV COMMAND - BUILT-IN
 * This command show the current environments variables in the shell.
 */
exitcode_t env_command(char **argv __attribute__((unused)))
{
    env_node_t *env = get_shell()->env;

    while (env != NULL) {
        my_printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    return OK_OUTPUT;
}
