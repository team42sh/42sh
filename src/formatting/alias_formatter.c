/*
** EPITECH PROJECT, 2025
** src/formatting/alias_formatter.c
** File description:
** This file contains functions for handling aliases with proper loop detection
*/

#include "core/minishell.h"

/**
 * @brief Modify every words if need to be replaced with aliases.
 *
 * @param input         The ARGV
 * @param aliases       The aliases structure
 * @param modified      Did we modified the ARGV ?
 *
 * @return The final ARGV.
 */
static char **modify_every_words(OUT char **input, IN alias_t *aliases,
    OUT bool *modified)
{
    size_t len_word_aliases = 0;
    char **words_aliases = 0;
    size_t index = 0;

    while (input[index] != NULL) {
        if (index > 0)
            break;
        if (!my_strcmp(input[index], aliases->original_string)) {
            words_aliases = my_strarray(aliases->alias_string);
            len_word_aliases = array_count_string(words_aliases);
            input = array_append_index_string(input, words_aliases, index);
            input = array_remove_index_string(input, index + len_word_aliases);
            *modified = true;
            free_array_string(words_aliases);
            break;
        }
        index++;
    }
    return input;
}

/**
 * @brief Free the processed originals array.
 *
 * @param processed_originals    Array to free
 * @param processed_count        Number of elements in array
 */
static void free_processed_originals(OUT char **processed_originals,
    IN size_t processed_count)
{
    if (!processed_originals)
        return;
    for (size_t i = 0; i < processed_count; i++)
        free(processed_originals[i]);
    free(processed_originals);
}

/**
 * @brief Check if an alias is part of a loop.
 *
 * @param original_string        The original string to check
 * @param processed_originals    Array of already processed originals
 * @param processed_count        Number of processed originals
 *
 * @return true if loop detected, false otherwise.
 */
static bool is_alias_loop(IN char *original_string,
    IN char **processed_originals, IN size_t processed_count, OUT char **input)
{
    for (size_t i = 0; i < processed_count; i++) {
        if (my_strcmp(original_string, processed_originals[i]) == 0) {
            free_processed_originals(processed_originals, processed_count);
            free_array_string(input);
            print_err("Alias loop.\n");
            return true;
        }
    }
    return false;
}

/**
 * @brief Add original string to processed originals.
 *
 * @param processed_originals    Array of processed originals
 * @param processed_count        Number of processed originals
 * @param original_string        String to add
 *
 * @return Updated array of processed originals?
 */
static char **add_to_processed_originals(OUT char **processed_originals,
    OUT size_t *processed_count, IN char *original_string)
{
    (*processed_count)++;
    processed_originals = realloc(processed_originals,
        (*processed_count) * sizeof(char *));
    processed_originals[(*processed_count) - 1] = my_strdup(original_string);
    return processed_originals;
}

/**
 * @brief Process a single alias replacement iteration.
 *
 * @param input                  Array of strings to process
 * @param processed_originals    Array of already processed originals
 * @param processed_count        Number of processed originals
 * @param modified               Whether any replacement was made
 *
 * @return NULL if loop detected, updated input otherwise.
 */
static char **process_alias_iteration(OUT char **input,
    OUT char ***processed_originals, OUT size_t *processed_count,
    OUT bool *modified)
{
    alias_t *aliases = get_shell()->aliases;

    while (aliases != NULL) {
        if (my_strcmp(aliases->original_string, aliases->alias_string) == 0) {
            aliases = aliases->next;
            continue;
        }
        input = modify_every_words(input, aliases, modified);
        if (*modified == true && is_alias_loop(aliases->original_string,
            *processed_originals, *processed_count, input))
            return NULL;
        if (*modified == true) {
            *processed_originals = add_to_processed_originals(
                *processed_originals, processed_count,
                aliases->original_string);
            break;
        }
        aliases = aliases->next;
    }
    return input;
}

/**
 * @brief Replace all aliases in the input array.
 *
 * @param input      Array of strings to process
 *
 * @return Updated array with aliases replaced, NULL if loop detected.
 */
char **replaces_all_aliases(IN char **input)
{
    char **processed_originals = NULL;
    size_t processed_count = 0;
    bool modified = true;
    char **result = NULL;

    while (modified == true) {
        modified = false;
        result = process_alias_iteration(input, &processed_originals,
            &processed_count, &modified);
        if (result == NULL)
            return NULL;
        input = result;
    }
    free_processed_originals(processed_originals, processed_count);
    return input;
}
