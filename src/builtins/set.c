/*
** EPITECH PROJECT, 2025
** src/builtins/set
** File description:
** This files contains the main core of the set command
*/

#include "core/minishell.h"

/**
 * @brief A function that returns 1 if the value of a
 * local variable is a wordlist and not a word
 *
 * @param str
 * @return int
 */
static int is_value_wordlist(IN char *str)
{
    if (str == NULL)
        return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ')
            return 1;
    }
    return 0;
}

/**
 * @brief A fucntion to print the local variables stored in the shell
 *
 * @return int
 */
static int print_variables(IN int is_readonly)
{
    var_node_t *var = get_shell()->variables;
    int is_wordlist = 0;

    while (var != NULL) {
        if (var->read_only != is_readonly) {
            var = var->next;
            continue;
        }
        is_wordlist = is_value_wordlist(var->value);
        my_printf("%s\t", var->key);
        if (var->value != NULL)
            my_printf("%.*s%s", is_wordlist, "(", var->value);
        my_printf("%.*s\n", is_wordlist, ")");
        var = var->next;
    }
    return OK_OUTPUT;
}

/**
 * @brief A function to get the value to assing to a local variable
 * Can be a single word or a list of words between ()
 *
 * @param key
 * @param argv
 * @return char*
 */
static char *get_new_var_value(IN char *key, IN char **argv,
    IN int is_readonly)
{
    int key_len = my_strlen(key);
    char *new_value = NULL;

    if (key_len == my_strlen(argv[is_readonly + 1]))
        return new_value;
    if (argv[is_readonly + 1][key_len + 1] == '(') {
        new_value = my_strdup(&argv[is_readonly + 1][key_len + 2]);
        for (int i = is_readonly + 2; argv[i] != NULL; i++) {
            new_value = realloc(new_value,
                my_strlen(new_value) + my_strlen(argv[i]) + 2);
            new_value = my_strcat(new_value, " ");
            new_value = my_strcat(new_value, argv[i]);
        }
        if (new_value[my_strlen(new_value) - 1] == ')')
            new_value[my_strlen(new_value) - 1] = '\0';
    } else
        new_value = my_strdup(&argv[is_readonly + 1][key_len + 1]);
    return new_value;
}

/**
 * @brief A function to check if a local variable is read-only
 *
 * @param var
 * @return int
 */
static int is_var_readonly(IN var_node_t *var)
{
    if (var->read_only) {
        print_err("set: $%s is read-only.\n", var->key);
        return ERROR_OUTPUT;
    }
    return OK_OUTPUT;
}

/**
 * @brief A function to modify an existing local variable
 *
 * @param argv
 * @return int
 */
static int modify_existing(IN char **argv, IN int is_readonly)
{
    char *key = my_strtok(argv[is_readonly + 1], '=');
    var_node_t *var = get_shell()->variables;

    while (var != NULL) {
        if (my_strcmp(var->key, key) != 0) {
            var = var->next;
            continue;
        }
        if (is_var_readonly(var) == ERROR_OUTPUT) {
            free(key);
            return OK_OUTPUT;
        }
        free_null_check(var->value);
        var->value = get_new_var_value(key, argv, is_readonly);
        var->read_only = is_readonly;
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
static int add_variable(IN char **argv, IN int is_readonly)
{
    var_node_t *var = get_shell()->variables;
    char *key = my_strtok(argv[is_readonly + 1], '=');
    var_node_t *new_var = malloc(sizeof(var_node_t));

    if (!IS_ALPHA(key[0]) && key[0] != '_') {
        print_err("set: Variable name must begin with a letter.\n");
        free(key);
        return ERROR_OUTPUT;
    }
    new_var->key = key;
    new_var->value = get_new_var_value(key, argv, is_readonly);
    new_var->read_only = is_readonly;
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
exitcode_t set_command(IN char **argv)
{
    ssize_t argv_count = array_count_string(argv);
    int is_readonly = 0;
    int value_index = 1;

    if (argv[value_index] != NULL && my_strcmp(argv[value_index], "-r") == 0) {
        is_readonly = 1;
        value_index++;
        argv_count--;
    }
    if (argv_count == 1)
        return print_variables(is_readonly);
    if (modify_existing(argv, is_readonly) == ERROR_OUTPUT)
        return add_variable(argv, is_readonly);
    return OK_OUTPUT;
}
