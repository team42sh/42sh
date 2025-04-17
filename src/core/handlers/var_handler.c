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
static void free_var(IN var_node_t *var)
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

static int remove_if_first(IN char *key, OUT var_node_t *var)
{
    if (var != NULL && my_strcmp(var->key, key) == 0) {
        if (is_var_readonly(var))
            return ERROR_OUTPUT;
        get_shell()->variables = var->next;
        free_var(var);
        return OK_OUTPUT;
    }
    return COMMAND_ERROR;
}

/**
 * @brief A function to remove a local variable
 *
 * @param key
 * @return int
 */
int remove_var(IN char *key)
{
    var_node_t *var = get_shell()->variables;
    var_node_t *before = NULL;
    int remove_first_out = remove_if_first(key, var);

    if (remove_first_out != COMMAND_ERROR)
        return remove_first_out;
    before = var;
    var = var->next;
    while (var != NULL) {
        if (my_strcmp(key, var->key) != 0) {
            before = var;
            var = var->next;
        }
        if (is_var_readonly(var))
            return ERROR_OUTPUT;
        before->next = var->next;
        free_var(var);
        return 0;
    }
    return -1;
}
