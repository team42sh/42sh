/*
** EPITECH PROJECT, 2025
** src/strings/str_replace
** File description:
** This file contain a function for replacing a string inside
** a string by another one
*/

#include "../../../include/core/minishell.h"

/*
 * Check if to_replace occurs at a specific index in the string.
 * Return 1 if an occurrence is found, 0 otherwise.
 */
static int check_occurrence_at_index(char *string, char *to_replace, int i)
{
    int temp_replace_index = 0;

    while (to_replace[temp_replace_index] != '\0') {
        if (string[i + temp_replace_index] != to_replace[temp_replace_index])
            return 0;
        temp_replace_index++;
    }
    return 1;
}

/*
 * Will count the number of time the word to_replace is in the string.
 * Return the number of occurences of to_replace.
 */
static int get_occurence_to_replace(char *string, char *to_replace)
{
    int index = 0;
    int occurences_amount = 0;

    while (string[index] != '\0') {
        if (check_occurrence_at_index(string, to_replace, index)) {
            occurences_amount++;
            index += my_strlen(to_replace);
        } else {
            index++;
        }
    }
    return occurences_amount;
}

/*
* Replace all occurrences of to_replace in the string with replace_string.
* Return a newly allocated string with replacements.
* TODO: Check if new_string malloc failed.
*/
char *my_strreplace(char *string, char *from, char *to)
{
    int from_len = my_strlen(from);
    int str_len = my_strlen(string);
    int occ = get_occurence_to_replace(string, from);
    int new_len = str_len - (from_len * occ) + (my_strlen(to) * occ) + 1;
    char *new_string = malloc(sizeof(char) * new_len);
    int new_index = 0;

    for (int i = 0; i < str_len;) {
        if (check_occurrence_at_index(string, from, i)) {
            my_strncpy(&new_string[new_index], to, my_strlen(to));
            new_index += my_strlen(to);
            i += from_len;
            continue;
        }
        new_string[new_index] = string[i];
        new_index++;
        i++;
    }
    new_string[new_index] = '\0';
    return new_string;
}

/*
 * This will replace words in an entire array.
 * All elements in this array need to be allocated.
 * Array need to be NULL terminated.
 */
char **my_strreplace_array(char **array, char *to_replace, char *replace_str)
{
    char *temp_free = NULL;

    for (int i = 0; array[i] != NULL; i++) {
        temp_free = array[i];
        array[i] = my_strreplace(array[i], to_replace, replace_str);
        free_null_check(temp_free);
    }
    return array;
}

/*
 * Same stuff as my_strreplace but it only replace if the entire string is
 * the same as the to string.
 */
char *my_strreplace_full(char *string, char *from, char *to)
{
    if (my_strcmp(string, from) == 0) {
        free(string);
        string = my_strdup(to);
    }
    return string;
}

/*
 * This will replace words in an entire array.
 * All elements in this array need to be allocated.
 * Array need to be NULL terminated.
 * Same stuff as my_strreplace_array.
 */
char **my_strreplace_array_full(char **array, char *to_replace, char *replace)
{
    for (int i = 0; array[i] != NULL; i++)
        array[i] = my_strreplace_full(array[i], to_replace, replace);
    return array;
}
