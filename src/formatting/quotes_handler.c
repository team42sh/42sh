/*
** EPITECH PROJECT, 2025
** src/strings/my_quotes_handler
** File description:
** This file provide some functions for handling quotes in a string
** Basically removing every '"' without '\' before
*/

#include "core/minishell.h"
#include "my_printf.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief This function count the number of consecutive backslash from i to 0
 *
 * @param string The string
 * @param i Where to start in the string
 * @return int number of backslash
 */
static int count_backslash(IN char *string, IN int i)
{
    int amount_backslash = 0;

    if (string == NULL)
        return 0;
    while (i >= 0 && string[i] == '\\') {
        amount_backslash++;
        i--;
    }
    return amount_backslash;
}

/*
 * Count the number of " if it it's not preceded by a \.
 * For exemple : "salue les \"amis\"" -> 2
 */
static int count_quotes(IN char *string)
{
    int amount_quotes = 0;

    if (string == NULL)
        return 0;
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '\"' && !(count_backslash(string, i - 1) % 2)) {
            amount_quotes++;
        }
    }
    return amount_quotes;
}

/*
 * This function will remove quotes if needed.
 * If a quotes is preceded by a '\', then keep it.
 * This function will create a new string it in a new memory space.
 */
char *handle_quotes(IN char *string)
{
    int clean_len = my_strlen(string) - count_quotes(string) + 1;
    char *clean_string = malloc(sizeof(char) * clean_len);
    int clean_temp = 0;

    if (string == NULL) {
        free(clean_string);
        return NULL;
    }
    if (clean_string == NULL)
        return NULL;
    for (int i = 0; string[i] != '\0'; i++) {
        if ((string[i + 1] == '\"' && count_backslash(string, i) % 2) ||
        (string[i] == '\"' && (i == 0 || !(count_backslash(string, i - 1) % 2))))
            continue;
        clean_string[clean_temp] = string[i];
        clean_temp++;
    }
    clean_string[clean_temp] = '\0';
    return clean_string;
}

/*
 * This will handles quotes in an entire array.
 * All elements in this array need to be allocated.
 */
char **handle_quotes_array(OUT char **array)
{
    char *temp_free = NULL;

    if (array == NULL)
        return NULL;
    for (int i = 0; array[i] != NULL; i++) {
        if ((count_quotes(array[i]) & 1)) {
            dprintf(2, "Unmatched \'\"\'.\n");
            return NULL;
        }
        temp_free = array[i];
        array[i] = handle_quotes(array[i]);
        free_null_check(temp_free);
    }
    return array;
}
