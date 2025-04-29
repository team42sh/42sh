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
static int is_value_wordlist(IN char **str)
{
    for (int i = 0; str[i] != NULL; i++) {
        if (i > 0)
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
        if (var->_read_only != is_readonly) {
            var = var->_next;
            continue;
        }
        is_wordlist = is_value_wordlist(var->_value);
        my_printf("%s\t", var->_key);
        for (int i = 0; var->_value[i] != NULL; i++) {
            my_printf("%.*s%.*s%s%.*s", (i > 0), " ",
            (is_wordlist && i == 0), "(",
            var->_value[i], (is_wordlist && var->_value[i + 1] == NULL), ")");
        }
        my_printf("\n");
        var = var->_next;
    }
    return OK_OUTPUT;
}

/**
 * @brief A function to get the strarray value of a local variable
 *
 * @param new_value
 * @param argv
 * @param key_len
 * @param is_readonly
 * @return char**
 */
static char **get_strarray(OUT char **new_value, IN char **argv,
    IN int key_len, IN int is_readonly)
{
    int value_index = 0;

    new_value = realloc(new_value, sizeof(char *) * 2);
    new_value[value_index] = my_strdup(&argv[is_readonly + 1][key_len + 2]);
    new_value[value_index + 1] = NULL;
    if (new_value[value_index][my_strlen(new_value[value_index]) - 1] == ')')
        new_value[value_index][my_strlen(new_value[value_index]) - 1] = '\0';
    value_index++;
    for (int i = is_readonly + 2; argv[i] != NULL; i++) {
        new_value = realloc(new_value, sizeof(char *) * (value_index + 2));
        new_value[value_index] = my_strdup(argv[i]);
        new_value[value_index + 1] = NULL;
        value_index++;
    }
    if (new_value[value_index - 1]
        [my_strlen(new_value[value_index - 1]) - 1] == ')')
        new_value[value_index - 1]
            [my_strlen(new_value[value_index - 1]) - 1] = '\0';
    return new_value;
}

/**
 * @brief A function to handle the error of the wordlist of a local variable
 *
 * @param argv
 * @param is_readonly
 * @param key_len
 * @return int
 */
static int handle_wordlist_error(IN char **argv, IN int is_readonly,
    IN int key_len)
{
    int argc = array_count_string(argv);

    if (argv[is_readonly + 1][key_len] == '\0')
        return OK_OUTPUT;
    if (argv[is_readonly + 1][key_len + 1] == '(' &&
        argv[argc - 1][my_strlen(argv[argc - 1]) - 1] != ')') {
            print_err("Too many ('s.\n");
        return ERROR_OUTPUT;
    }
    if (argv[argc - 1][my_strlen(argv[argc - 1]) - 1] == ')' &&
        argv[is_readonly + 1][key_len + 1] != '(') {
            print_err("Too many )'s.\n");
        return ERROR_OUTPUT;
    }
    return OK_OUTPUT;
}

/**
 * @brief A function to get the value to assing to a local variable
 * Can be a single word or a list of words between ()
 *
 * @param key
 * @param argv
 * @return char**
 */
static char **get_new_var_value(IN char *key, IN char **argv,
    IN int is_readonly)
{
    int key_len = my_strlen(key);
    char **new_value = calloc(1, sizeof(char *));

    if (key_len == my_strlen(argv[is_readonly + 1]))
        return new_value;
    if (argv[is_readonly + 1][key_len + 1] == '(')
        new_value = get_strarray(new_value, argv, key_len, is_readonly);
    else {
        new_value = realloc(new_value, sizeof(char *) * 2);
        new_value[0] = my_strdup(&argv[is_readonly + 1][key_len + 1]);
        new_value[1] = NULL;
    }
    return new_value;
}

/**
 * @brief A function to check if a local variable is read-only
 *
 * @param var
 * @return int
 */
int is_var_readonly(IN var_node_t *var)
{
    if (var->_read_only) {
        print_err("set: $%s is read-only.\n", var->_key);
        return ERROR_OUTPUT;
    }
    return OK_OUTPUT;
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
    new_var->_key = key;
    new_var->_value = get_new_var_value(key, argv, is_readonly);
    new_var->_read_only = is_readonly;
    new_var->_next = var;
    get_shell()->variables = new_var;
    return OK_OUTPUT;
}

/**
 * @brief A function to modify or create a local variable
 *
 * @param argv
 * @return int
 */
static int modify_or_create_var(OUT char *key, IN char **argv,
    IN int is_readonly)
{
    var_node_t *var = get_shell()->variables;

    while (var != NULL) {
        if (my_strcmp(var->_key, key) != 0) {
            var = var->_next;
            continue;
        }
        if (is_var_readonly(var) == ERROR_OUTPUT)
            return ERROR_OUTPUT;
        free_var_value(var);
        var->_value = get_new_var_value(key, argv, is_readonly);
        var->_read_only = is_readonly;
        return OK_OUTPUT;
    }
    return add_variable(argv, is_readonly);
}

/**
 * @brief Functions to run the 'set' builtin
 * Can set local variables to a word or wordlist and list them
 *
 * @param argv The arguments passed to the command
 * @return exitcode_t
 */
exitcode_t set_command(IN char **argv)
{
    ssize_t argv_count = array_count_string(argv);
    int is_readonly = 0;
    int value_index = 1;
    char *key;

    if (argv[value_index] != NULL && my_strcmp(argv[value_index], "-r") == 0) {
        is_readonly = 1;
        value_index++;
        argv_count--;
    }
    if (argv_count == 1)
        return print_variables(is_readonly);
    key = my_strtok(argv[is_readonly + 1], '=');
    if (handle_wordlist_error(argv, is_readonly, my_strlen(key)) ||
        modify_or_create_var(key, argv, is_readonly)) {
        free(key);
        return ERROR_OUTPUT;
    }
    free(key);
    return OK_OUTPUT;
}
