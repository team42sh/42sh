/*
** EPITECH PROJECT, 2025
** src/formatting/variables_handler
** File description:
** This file contains functions for extracting environment variables
*/

#include "core/minishell.h"
#include "core/types.h"

/**
 * @brief A function to get the variable name between brackets
 *
 * @param string
 * @return char*
 */
static char *get_var_name_brackets(IN char *string)
{
    char *value = NULL;
    int len = 0;

    for (; string[len + 1] != '\0' && string[len + 1] != '}'; len++);
    if (string[len + 1] != '}') {
        print_err("Missing '}'.\n");
        return NULL;
    }
    value = malloc(sizeof(char) * (len + 4));
    value[0] = '$';
    value[1] = '{';
    for (int i = 2; i < len + 2; i++)
        value[i] = string[i - 1];
    value[len + 2] = '}';
    value[len + 3] = '\0';
    return value;
}

/*
 * Extract the content after the $
 */
static char *get_var_name(char *string)
{
    char *value = NULL;
    int index = 0;

    if (string == NULL || string[0] == '\0')
        return NULL;
    if (string[0] == '$')
        string++;
    while (string[index] != '\0' && (IS_ALPHA_NUM(string[index]) ||
        string[index] == '_' || string[index] == '{' || string[index] == '}')
        && string[index] != '$')
        index++;
    if (index == 0)
        return NULL;
    value = malloc(sizeof(char) * (index + 2));
    if (value == NULL)
        return NULL;
    value[0] = '$';
    value[1] = '\0';
    value = my_strncat(value, string, index);
    return value;
}

/*
 * For every character in the input string find if there is any variables.
 * If yes just extract it.
 * If any error occurs then remove all strings and don't replace anything.
 * These errors appends usually when cat /dev/urandom | ./mysh
 */
static int extract_var(char *input, int *index, string_t **strings)
{
    char *tmp = NULL;

    if (input[*index] == '$' && input[*index + 1] != '\0'
        && (IS_ALPHA(input[*index + 1]) || IS_NUMBER(input[*index + 1]) ||
        input[*index + 1] == '_' || input[*index + 1] == '{' ||
        input[*index + 1] == '}')) {
        if (input[*index + 1] == '{')
            tmp = get_var_name_brackets(&input[*index + 1]);
        else
            tmp = get_var_name(&input[*index]);
        if (tmp == NULL) {
            free_strings(*strings);
            return 2;
        }
        *strings = add_string(*strings, tmp);
        *index += my_strlen(tmp);
        return 1;
    }
    return 0;
}

/*
 * Extract the value of variables like $PATH or $VAR
 *
 * TODO: Extract when brackets.
 */
string_t *extract_vars_in_string(string_t *strings, char *input)
{
    int index = 0;
    int len;
    int tmp_result = 0;

    if (input == NULL)
        return NULL;
    len = my_strlen(input);
    while (index < len) {
        tmp_result = extract_var(input, &index, &strings);
        if (tmp_result == 2)
            return NULL;
        if (tmp_result == 0) {
            index++;
            continue;
        }
    }
    return strings;
}

/*
 * Extract the value of variables like extract_vars_in_string but for array.
 *
 * TODO: Extract when brackets.
 */
string_t *extract_vars_in_array(char **array)
{
    string_t *head = NULL;

    if (array == NULL)
        return NULL;
    for (int i = 0; array[i] != NULL; i++)
        head = extract_vars_in_string(head, array[i]);
    return head;
}

/**
 * @brief Get the query objectA function to
 * get the name of the variable to replace
 *
 * @param var_string
 * @return char*
 */
static char *get_query(IN char *var_string)
{
    char *query;

    if (var_string[0] == '$' && var_string[1] != '{')
        query = my_strdup(&var_string[1]);
    if (var_string[0] == '$' && var_string[1] == '{') {
        query = my_strdup(&var_string[2]);
        query[my_strlen(query) - 1] = '\0';
    }
    return query;
}

/**
 * @brief A function to replace a local or environment variable in the argv
 *
 * @param query The name of the variable to replace
 * @param value The value to replace it with
 * @return int
 */
static int replace_var_or_env(IN char *query, OUT char **value)
{
    int return_value = ERROR_OUTPUT;

    if (var_search(query) != NULL) {
        *value = concat_strarray(var_search(query),
            " ");
        return_value = OK_OUTPUT;
    }
    if (env_search(query) != NULL) {
        *value = env_search(query);
        return_value = OK_OUTPUT;
    }
    return return_value;
}

/**
 * @brief A function to replace a variable in user input
 *
 * @param argv
 * @param vars_replace
 * @param head
 * @return int
 */
static int replace_value(OUT char ***argv, OUT string_t *vars_replace,
    OUT string_t *head)
{
    int return_value = ERROR_OUTPUT;
    char *query = get_query(vars_replace->string);
    char *value = NULL;
    char *concatenated = NULL;

    return_value = replace_var_or_env(query, &value);
    free_null_check(concatenated);
    if (return_value == OK_OUTPUT) {
        *argv = my_strreplace_array(*argv, vars_replace->string, value);
        free_null_check(query);
        return return_value;
    }
    my_printf("%s: Undefined variable.\n", query);
    free_null_check(query);
    get_shell()->last_exit_code = 1;
    free_strings(head);
    return return_value;
}

/*
 * Replace every variables use can find in the environments.
 * If we find a $PATH then replace array the $PATH by it's value.
 */
int replace_env_variables(char ***argv)
{
    string_t *vars_replace = NULL;
    string_t *head = NULL;

    if (argv == NULL)
        return ERROR_OUTPUT;
    vars_replace = extract_vars_in_array(*argv);
    head = vars_replace;
    while (vars_replace != NULL) {
        if (replace_value(argv, vars_replace, head) == ERROR_OUTPUT)
            return ERROR_OUTPUT;
        vars_replace = vars_replace->next;
    }
    free_strings(head);
    return OK_OUTPUT;
}

/*
 * Replace all variables in the string.
 * Currently support :
 * - $? ==> Last exit code from execve
 * - $PATH and all variables like that
 */
int replace_variables(char ***argv)
{
    char *exitcode_str = my_itoa(get_shell()->last_exit_code);

    if (argv == NULL)
        return ERROR_OUTPUT;
    *argv = my_strreplace_array(*argv, "$?", exitcode_str);
    free_null_check(exitcode_str);
    if (replace_env_variables(argv) == ERROR_OUTPUT)
        return ERROR_OUTPUT;
    return OK_OUTPUT;
}
