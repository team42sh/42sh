/*
** EPITECH PROJECT, 2025
** src/formatting/globbings/regex_globbing
** File description:
** Improved implementation of regex globbing for shell
*/

#include "core/minishell.h"

/**
 * @brief Create terminal result (when no more args)
 *
 * @param base_dir Base directory
 * @return char** Result array
 */
static char **create_terminal_result(IN char *base_dir)
{
    char **result = NULL;

    result = malloc(sizeof(char *) * 2);
    if (!result)
        return NULL;
    result[0] = my_strdup(base_dir);
    if (!result[0]) {
        free(result);
        return NULL;
    }
    result[1] = NULL;
    return result;
}

/**
 * @brief Handle simple path args (no glob)
 *
 * @param base_dir Base directory
 * @param args Path args
 * @return char* New path or NULL
 */
static char *handle_simple_args(IN char *base_dir, IN char *args)
{
    char *new_path = NULL;

    new_path = create_path(base_dir, args);
    if (!new_path)
        return NULL;
    if (!dir_exists(new_path)) {
        free(new_path);
        return NULL;
    }
    return new_path;
}

/**
 * @brief Handle globbing args
 *
 * @param base_dir Base directory
 * @param args args array
 * @param index Current index
 * @return char** Matches array or NULL
 */
static char **handle_glob(IN char *base_dir, IN char **args,
    IN int index)
{
    DIR *dir = NULL;
    regex_t regex;
    char *regex_pattern = NULL;
    char **matches = NULL;
    dir_process_state_t state;

    dir = opendir(base_dir);
    if (!dir)
        return NULL;
    regex_pattern = init_regex(args[index], &regex);
    state.base_dir = base_dir;
    state.args = args;
    state.index = index;
    matches = process_dir_entries(dir, regex_pattern, &regex, &state);
    regfree(&regex);
    free(regex_pattern);
    closedir(dir);
    return matches;
}

/**
 * @brief Recursively expand a globbing pattern
 *
 * @param base_dir Base directory
 * @param args Pattern args
 * @param index Current args index
 * @return char** Array of matching paths
 */
char **expand_pattern_recursive(IN char *base_dir, IN char **args,
    IN int index)
{
    char *new_base = NULL;
    char **result = NULL;
    int has_glob = 0;

    if (!args[index])
        return create_terminal_result(base_dir);
    has_glob = has_globbing_chars(args[index]);
    if (!has_glob) {
        new_base = handle_simple_args(base_dir, args[index]);
        if (!new_base)
            return NULL;
        result = expand_pattern_recursive(new_base, args, index + 1);
        free(new_base);
        return result;
    }
    return handle_glob(base_dir, args, index);
}

/**
 * @brief Initialize result for non-globbing pattern
 *
 * @param pattern Path pattern with no globbing chars
 * @return char** Array containing the pattern
 */
static char **init_non_glob_result(IN char *pattern)
{
    char **result = NULL;

    result = malloc(sizeof(char *) * 2);
    if (!result)
        return NULL;
    result[0] = my_strdup(pattern);
    result[1] = NULL;
    return result;
}

/**
 * @brief Get base directory for path expansion
 *
 * @param pattern Path pattern to analyze
 * @return char* Base directory string
 */
static char *get_base_directory(IN char *pattern)
{
    char *base_dir = NULL;

    if (pattern[0] == '/')
        base_dir = my_strdup("/");
    else
        base_dir = my_strdup("./");
    return base_dir;
}

/**
 * @brief Process a path with globbing patterns
 *
 * @param pattern Path pattern with globbing chars
 * @return char** Array of matching paths
 */
char **expand_glob_pattern(IN char *pattern)
{
    char *base_dir = NULL;
    char **args = NULL;
    char **matches = NULL;

    if (!pattern)
        return NULL;
    if (!has_globbing_chars(pattern))
        return init_non_glob_result(pattern);
    base_dir = get_base_directory(pattern);
    if (!base_dir)
        return NULL;
    args = split_path(pattern);
    if (!args) {
        free(base_dir);
        return NULL;
    }
    matches = expand_pattern_recursive(base_dir, args, 0);
    free(base_dir);
    free_array_string(args);
    return matches;
}

/**
 * @brief Process an argument with potential globbing
 *
 * @param state Globbing state information
 * @param i Current argument index
 * @return char** Updated result or NULL on error
 */
char **process_argument(OUT glob_state_t *state, IN int i)
{
    char **matches = NULL;

    if (has_globbing_chars(state->argv[i])) {
        matches = expand_glob_pattern(state->argv[i]);
        if (matches != NULL && matches[0] != NULL)
            state->result = process_glob_matches(state, matches);
        if (matches == NULL)
            state->result = handle_no_matches(state, matches, i);
    } else {
        state->result[state->result_index] = my_strdup(state->argv[i]);
        state->result_index++;
    }
    return state->result;
}

/**
 * @brief Handle globbing in command arguments
 *
 * @param argv Array of arguments
 * @return char** Expanded array of arguments with globbing resolved
 */
char **process_globbing(IN char **argv)
{
    glob_state_t state;

    state.result_index = 0;
    state.argv = argv;
    if (!argv)
        return NULL;
    state.argc = array_count_string(argv);
    state.result = malloc(sizeof(char *) * (state.argc + 1));
    if (!state.result)
        return NULL;
    for (int i = 0; i <= state.argc; i++)
        state.result[i] = NULL;
    for (int i = 0; i < state.argc; i++) {
        if (!process_argument(&state, i)) {
            free_array_string(state.result);
            return NULL;
        }
    }
    state.result[state.result_index] = NULL;
    return state.result;
}

/**
 * @brief Main function to handle globbing with regex
 *
 * @param argv Array of arguments potentially containing globbing patterns
 * @return char** New array with globbing patterns expanded to matching files
 */
char **regex_globbing(IN char **argv)
{
    char **result = NULL;
    char *new_path = NULL;

    if (!argv || !argv[0])
        return NULL;
    result = process_globbing(argv);
    for (int i = 0; result && result[i]; i++) {
        new_path = remove_dot_slash(result[i]);
        free(result[i]);
        result[i] = new_path;
    }
    free_array_string(argv);
    return result;
}
