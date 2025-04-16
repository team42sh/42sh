/*
** EPITECH PROJECT, 2025
** src/commands/set
** File description:
** This files contains the main core of the set command
*/

#include "core/minishell.h"


static int is_value_wordlist(char *str)
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
static int print_variables(void)
{
    var_node_t *var = get_shell()->variables;
    int is_wordlist = 0;

    while (var != NULL) {
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
static char *get_new_var_value(char *key, char **argv)
{
    int key_len = my_strlen(key);
    char *new_value = NULL;

    if (key_len == my_strlen(argv[1]))
        return new_value;
    if (argv[1][key_len + 1] == '(') {
        new_value = my_strdup(&argv[1][key_len + 2]);
        for (int i = 2; argv[i] != NULL; i++) {
            new_value = realloc(new_value,
                my_strlen(new_value) + my_strlen(argv[i]) + 2);
            new_value = my_strcat(new_value, " ");
            new_value = my_strcat(new_value, argv[i]);
        }
        if (new_value[my_strlen(new_value) - 1] == ')')
            new_value[my_strlen(new_value) - 1] = '\0';
    } else
        new_value = my_strdup(&argv[1][key_len + 1]);
    return new_value;
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
        var->value = get_new_var_value(key, argv);
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
    var_node_t *new_var = malloc(sizeof(var_node_t));

    new_var->key = key;
    new_var->value = get_new_var_value(key, argv);
    new_var->read_only = 0;
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
    if (modify_existing(argv) == ERROR_OUTPUT)
        return add_variable(argv);
    return OK_OUTPUT;
}
