/*
** EPITECH PROJECT, 2025
** var_handler
** File description:
** Functions to handle the local variables of 42sh
*/

#include "core/minishell.h"

/**
 * @brief A function to free the value of a lcoal variable
 *
 * @param var
 */
void free_var_value(OUT var_node_t *var)
{
    for (int i = 0; var->_value[i] != NULL; i++)
        free_null_check(var->_value[i]);
    free_null_check(var->_value);
}

/*
 * Free a node of the variables.
 */
static void free_var(OUT var_node_t *var)
{
    if (var == NULL)
        return;
    free_null_check(var->_key);
    free_var_value(var);
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
        next_tmp = var->_next;
        free_var(var);
    }
    get_shell()->variables = NULL;
}

/**
 * @brief A function to delete the first element of a linked list
 *
 * @param key
 * @param var
 * @param before
 * @return int
 */
static int remove_if_first(IN char *key, OUT var_node_t **var,
    OUT var_node_t **before)
{
    if (*var == NULL)
        return OK_OUTPUT;
    if (*var != NULL && my_strcmp((*var)->_key, key) == 0) {
        if (is_var_readonly(*var))
            return ERROR_OUTPUT;
        get_shell()->variables = (*var)->_next;
        free_var(*var);
        return OK_OUTPUT;
    }
    *before = *var;
    *var = (*var)->_next;
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
    int remove_first_out;

    remove_first_out = remove_if_first(key, &var, &before);
    if (remove_first_out != COMMAND_ERROR)
        return remove_first_out;
    while (var != NULL) {
        if (my_strcmp(key, var->_key) != 0) {
            before = var;
            var = var->_next;
            continue;
        }
        if (is_var_readonly(var))
            return OK_OUTPUT;
        before->_next = var->_next;
        free_var(var);
        return OK_OUTPUT;
    }
    return OK_OUTPUT;
}

/**
 * @brief A function to get a local variable by key
 *
 * @param key
 * @return char*
 */
char **var_search(IN char *key)
{
    var_node_t *var = get_shell()->variables;

    if (key == NULL)
        return NULL;
    while (var != NULL) {
        if (my_strcmp(var->_key, key) == 0)
            return var->_value;
        var = var->_next;
    }
    return NULL;
}

/**
 * @brief A function to concat the value of a local variable
 *
 * @param array
 * @return char*
 */
char *concat_strarray(IN char **array)
{
    char *str = NULL;
    int len = 0;

    if (array == NULL)
        return NULL;
    for (int i = 0; array[i] != NULL; i++)
        len += my_strlen(array[i]) + 1;
    str = malloc(sizeof(char) * (len + 1));
    if (str == NULL)
        return NULL;
    str[0] = '\0';
    for (int i = 0; array[i] != NULL; i++) {
        my_strcat(str, array[i]);
        if (array[i + 1] != NULL)
            my_strcat(str, " ");
    }
    return str;
}
