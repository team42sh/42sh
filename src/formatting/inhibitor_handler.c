/*
** EPITECH PROJECT, 2025
** src/strings/my_quotes_handler
** File description:
** This file provide some functions for handling inhibitors in
*/

#include "core/minishell.h"
#include "core/types.h"

/**
 * @brief This function count the number of escaped character in a string
 *
 * @param string the string
 * @return int the number of character that are to be escaped
 */
static int count_escaped(IN char *string)
{
    int in_quote = 0;
    int count = 0;

    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '\"' && !(count_backslash(string, i - 1) % 2))
            in_quote = !in_quote;
        if (!in_quote && string[i] == '\\' &&
            char_in_str(NOT_INHIBITED, string[i + 1]) == -1) {
            count++;
            i = (string[i + 1] == '\0' ? i : i + 1);
        }
    }
    return count;
}

/**
 * @brief This function transform all the \ that are not between quote and
 * not in the blacklist of transformation.
 *
 * @param string the string to handle
 * @return char* the new string
 */
char *handle_inhibitors(IN char *string)
{
    int clean_len = my_strlen(string) - count_escaped(string) + 1;
    char *clean_string = calloc(sizeof(char), clean_len);
    int clean_temp = 0;
    int in_quote = 0;

    if (string == NULL || clean_string == NULL) {
        free(clean_string);
        return NULL;
    }
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '\"' && !(count_backslash(string, i - 1) % 2))
            in_quote = !in_quote;
        if (!in_quote && string[i] == '\\' &&
            char_in_str(NOT_INHIBITED, string[i + 1]) == -1)
            i++;
        clean_string[clean_temp] = string[i];
        clean_temp++;
    }
    clean_string[clean_temp] = '\0';
    return clean_string;
}

/**
 * @brief Handle the inhibitors of an array of char
 *
 * @param array the arrays of string to be inhibited
 * @return char** the new array
 */
char **handle_inhibitors_array(IN char **array)
{
    char *temp_free = NULL;

    if (array == NULL)
        return NULL;
    for (int i = 0; array[i] != NULL; i++) {
        temp_free = array[i];
        array[i] = handle_inhibitors(array[i]);
        free_null_check(temp_free);
    }
    return array;
}
