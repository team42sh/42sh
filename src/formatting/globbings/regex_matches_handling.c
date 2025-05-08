/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_matches_handling
** File description:
** file that handles the functions for the matches
*/

#include "core/minishell.h"

/**
 * @brief Resize matches array if needed
 *
 * @param matches Current matches array
 * @param capacity Current capacity
 * @param count Current count
 * @return char** New array or NULL on error
 */
char **resize_if_needed(IN char **matches, IN int *capacity, IN int count)
{
    char **new_matches = NULL;

    if (count < *capacity - 1)
        return matches;
    *capacity *= 2;
    new_matches = realloc(matches, sizeof(char *) * (*capacity));
    return new_matches;
}

/**
 * @brief Add a path to matches array
 *
 * @param state Directory processing state
 * @param new_path Path to add
 */
void add_to_matches(IN dir_process_state_t *state, IN char *new_path)
{
    char **new_matches = NULL;
    int new_capacity = 0;

    if (*(state->match_count) >= *(state->capacity)) {
        new_capacity = *(state->capacity) * 2;
        new_matches = realloc(state->matches,
            sizeof(char *) * (new_capacity + 1));
        if (!new_matches) {
            free(new_path);
            return;
        }
        state->matches = new_matches;
        *(state->capacity) = new_capacity;
    }
    state->matches[*(state->match_count)] = new_path;
    (*(state->match_count))++;
}

/**
 * @brief Add submatches to matches array
 *
 * @param state Directory processing state
 * @param sub_matches Submatches array
 */
void add_submatches(IN dir_process_state_t *state, IN char **sub_matches)
{
    int i = 0;

    if (!sub_matches)
        return;
    while (sub_matches[i] != NULL) {
        add_to_matches(state, sub_matches[i]);
        i++;
    }
    free(sub_matches);
}

/**
 * @brief Process a matching glob pattern
 *
 * @param state Globbing state information
 * @param matches Array of glob matches
 * @return char** Updated result array or NULL on failure
 */
char **process_glob_matches(OUT glob_state_t *state, IN char **matches)
{
    int j = 0;
    int new_size = 0;
    char **new_result = NULL;

    sort_paths(matches);
    while (matches[j] != NULL) {
        if (state->result_index >= state->argc) {
            new_size = state->result_index + 10;
            new_result = reallocate_result_array(state, matches, new_size);
            state->result = new_result;
        }
        state->result[state->result_index] = matches[j];
        state->result_index++;
        j++;
    }
    free(matches);
    return state->result;
}

/**
 * @brief Handle no matches case for globbing
 *
 * @param state Globbing state information
 * @param matches Array of glob matches
 * @param i Current argument index
 * @return char** Updated result or NULL on error
 */
char **handle_no_matches(OUT glob_state_t *state, IN char **matches, IN int i)
{
    if (i == 0 || my_strcmp(state->argv[0], "ls") == 0) {
        my_printf("%s: No match.\n", state->argv[0]);
        free_array_string(state->result);
        if (matches)
            free(matches);
        return NULL;
    } else {
        state->result[state->result_index] = my_strdup(state->argv[i]);
        state->result_index++;
    }
    return state->result;
}
