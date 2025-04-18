/*
** EPITECH PROJECT, 2025
** src/formatting/variables_handler
** File description:
** This file contains functions for extracting environment variables
*/

#include "core/minishell.h"
#include "core/types.h"

/*
 * Extract the content after the $
 *
 * TODO: Extract when brackets.
 */
static char *get_var_name(char *string)
{
    char *value = NULL;
    int index = 0;

    if (string == NULL || string[0] == '\0')
        return NULL;
    if (string[0] == '$')
        string++;
    while (string[index] != '\0' && !is_input_delimiter(string[index])
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
        && !is_input_delimiter(input[*index + 1])) {
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

    if (strings == NULL || input == NULL)
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
        if (env_search(&vars_replace->string[1]) == NULL) {
            my_printf("%s: Undefined variable.\n", &vars_replace->string[1]);
            get_shell()->last_exit_code = 1;
            free_strings(head);
            return ERROR_OUTPUT;
        }
        *argv = my_strreplace_array(*argv, vars_replace->string,
            env_search(&vars_replace->string[1]));
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
