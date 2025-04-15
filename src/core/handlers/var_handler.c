/*
** EPITECH PROJECT, 2025
** var_handler
** File description:
** Functions to handle the local variables of 42sh
*/

#include "core/minishell.h"

/*
 * Free a node of the variables.
 */
static void free_var(var_node_t *var)
{
    if (var == NULL)
        return;
    free_null_check(var->key);
    free_null_check(var->value);
    free_null_check(var);
}

/**
 * @brief A function to clear all the local variables of the shell
 */
void clear_var(void)
{
    var_node_t *next_tmp = NULL;

    for (var_node_t *var = get_shell()->variables; var != NULL;
        var = next_tmp) {
        next_tmp = var->next;
        free_var(var);
    }
    get_shell()->variables = NULL;
}
