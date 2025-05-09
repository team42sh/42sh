/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_globbing_processing
** File description:
** file where the functions to process globings are done
*/

#include "core/minishell.h"
#include "my_printf.h"

/**
 * @brief Process directory entries with regex
 *
 * @param dir Directory handle
 * @param regex_pattern Regex pattern string
 * @param regex Compiled regex
 * @param state Directory processing state
 * @return char** Matches array or NULL
 */
char **process_dir_entries(IN DIR *dir, IN char *regex_pattern,
    IN regex_t *regex, OUT dir_process_state_t *state)
{
    struct dirent *entry = NULL;
    int match_count = 0;
    int capacity = 10;

    state->matches = init_matches();
    state->match_count = &match_count;
    state->capacity = &capacity;
    rewinddir(dir);
    for (entry = readdir(dir); state && entry != NULL; entry = readdir(dir)) {
        if (should_skip_entry(entry, regex_pattern))
            continue;
        if (regexec(regex, entry->d_name, 0, NULL, 0) == 0 && state->matches)
            process_match(state, entry);
    }
    state->matches[match_count] = NULL;
    if (match_count == 0) {
        free(state->matches);
        return NULL;
    }
    return state->matches;
}

/**
 * @brief Process a matching entry
 *
 * @param state Directory processing state
 * @param entry Directory entry
 */
void process_match(IN dir_process_state_t *state, IN struct dirent *entry)
{
    char *new_path = NULL;

    new_path = create_path(state->base_dir, entry->d_name);
    if (!new_path)
        return;
    if (state->args[state->index + 1] != NULL)
        process_with_next(state, new_path);
    else
        add_to_matches(state, new_path);
}

/**
 * @brief Process with next args
 *
 * @param state Directory processing state
 * @param new_path New path to process
 */
void process_with_next(IN dir_process_state_t *state, IN char *new_path)
{
    char **sub_matches = NULL;

    if (!is_valid_directory(new_path)) {
        free(new_path);
        return;
    }
    sub_matches = expand_pattern_recursive(new_path, state->args,
        state->index + 1);
    free(new_path);
    add_submatches(state, sub_matches);
}
