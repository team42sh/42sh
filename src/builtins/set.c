/*
** EPITECH PROJECT, 2025
** src/commands/set
** File description:
** This files contains the main core of the set command
*/

#include "core/minishell.h"

/**
 * @brief A fucntion to print the local variables stored in the shell
 *
 * @return int
 */
static int print_variables(void)
{
    var_node_t *var = get_shell()->variables;

    while (var != NULL) {
        my_printf("%s\t", var->key);
        if (var->value != NULL)
            my_printf("%s", var->value);
        my_printf("\n");
        var = var->next;
    }
    return OK_OUTPUT;
}

/**
 * @brief A function to modify an existing local variable
 *
 * @param argv
 * @return int
 */
static int modify_existing(char **argv)
{
    char *key = my_strtok(argv[1], '=');
    var_node_t *var = get_shell()->variables;

    while (var != NULL) {
        if (my_strcmp(var->key, key) != 0) {
            var = var->next;
            continue;
        }
        free_null_check(var->value);
        if (my_strlen(argv[1]) != my_strlen(key))
            var->value = my_strdup(&argv[1][my_strlen(key) + 1]);
        free(key);
        return OK_OUTPUT;
    }
    free(key);
    return ERROR_OUTPUT;
}

/**
 * @brief A function to add a local variable if it doesn't exist
 *
 * @param argv
 * @return int
 */
static int add_variable(char **argv)
{
    var_node_t *var = get_shell()->variables;
    char *key = my_strtok(argv[1], '=');
    int key_len = my_strlen(key);
    var_node_t *new_var = malloc(sizeof(var_node_t));

    new_var->key = key;
    new_var->value = NULL;
    new_var->read_only = 0;
    if (key_len != my_strlen(argv[1]))
        new_var->value = my_strdup(&argv[1][key_len + 1]);
    new_var->next = var;
    get_shell()->variables = new_var;
    return OK_OUTPUT;
}

/**
 * @brief Functions to run the 'set' builtin
 *
 * @param argv The arguments passed to the command
 * @return exitcode_t
 */
exitcode_t set_command(char **argv)
{
    ssize_t argv_count = array_count_string(argv);

    if (argv_count == 1)
        return print_variables();
    if (argv_count == 2) {
        if (modify_existing(argv) == ERROR_OUTPUT)
            return add_variable(argv);
    }
    return OK_OUTPUT;
}
