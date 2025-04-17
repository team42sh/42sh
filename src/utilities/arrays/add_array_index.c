/*
** EPITECH PROJECT, 2025
** src/utilities/arrays/add_array_index
** File description:
** This file contains the whole logic for adding an element at a
** given index in a array
*/

#include "core/minishell.h"
#include <unistd.h>

/*
 * Add an element to the array at the given index.
 * This will copy the entire array and add the element and then do the rest of
 * the array. By the way the function free the original one.
 */
char **array_add_index_string(char **original, char *to_add, size_t index)
{
    size_t original_count = array_count_string(original);
    char **new_arr = malloc(sizeof(char *) * (original_count + 2));
    size_t current_index = 0;

    while (current_index != index) {
        new_arr[current_index] = original[current_index];
        current_index++;
    }
    new_arr[current_index] = my_strdup(to_add);
    while (current_index < original_count) {
        new_arr[current_index + 1] = original[current_index];
        current_index++;
    }
    new_arr[original_count + 1] = NULL;
    free(original);
    return new_arr;
}

/*
 * Append the array append to the array original at the index index.
 * Use array_add_index_string to perfom this action.
 */
char **array_append_index_string(char **original, char **append, size_t index)
{
    size_t i = 0;

    if (original == NULL || append == NULL)
        return NULL;
    while (append[i] != NULL) {
        original = array_add_index_string(original, append[i], index + i);
        if (!original)
            return NULL;
        i++;
    }
    return original;
}

/*
 * Remove the element at the index index in the array original.
 */
char **array_remove_index_string(char **original, size_t index)
{
    size_t original_count = array_count_string(original);
    char **new_arr = NULL;
    size_t j = 0;

    if (index >= original_count)
        return original;
    new_arr = malloc(sizeof(char *) * original_count);
    if (new_arr == NULL)
        return NULL;
    for (size_t i = 0; i < original_count; i++) {
        if (i == index) {
            free(original[i]);
            continue;
        }
        new_arr[j] = original[i];
        j++;
    }
    new_arr[j] = NULL;
    if (original != NULL)
        free(original);
    return new_arr;
}
