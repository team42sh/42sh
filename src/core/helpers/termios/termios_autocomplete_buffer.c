/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_autocomplete_buffer
** File description:
** This file contains functions for editing the buffer for the autocomplete
*/

#include "core/minishell.h"

/**
 * @brief Extract the last folder or file name from a path
 *        (starting from the last '/')
 *
 * @param path The full path string
 *
 * @return A pointer to the last folder or file name in the path.
 */
static char *get_last_folder_char(IN char *path)
{
    int index = my_strlen(path);

    while (index >= 0 && path[index] != '/')
        index--;
    return &path[index];
}

/**
 * @brief Check if the suggestion is a symbolic link (ends with '@').
 *
 * @param sugg The suggestion string
 *
 * @return true if it ends with '@', false otherwise.
 */
static bool is_suggestion_link(IN char *sugg)
{
    size_t len = my_strlen(sugg);

    return (len > 0 && sugg[len - 1] == '@');
}

/**
 * @brief Check if the suggestion is a directory (only if it's a symlink).
 *
 * @param final_sugg The cleaned suggestion path
 * @param is_link Whether the original suggestion was a symbolic link
 *
 * @return true if it's a directory, false otherwise.
 */
static bool is_suggestion_directory(IN char *final_sugg, IN bool is_link)
{
    return (is_link && is_file_type(final_sugg, S_IFDIR));
}

/**
 * @brief Build the final suggestion string by combining the prefix and
 *        cleaned suggestion.
 *
 * @param curr_word The original word typed by the user
 * @param current_sugg The suggestion to insert
 * @param is_link Whether the suggestion is a symbolic link
 *
 * @return A newly allocated string with the full suggestion to be inserted.
 */
static char *build_final_suggestion(IN char *curr_word, IN char *current_sugg,
    IN bool is_link)
{
    char *curr_word_without_slash = get_last_folder_char(curr_word);
    size_t to_delete = my_strlen(&curr_word_without_slash[1]);
    size_t prefix_len = my_strlen(curr_word) - to_delete;
    char *final_sugg = malloc((prefix_len + my_strlen(current_sugg) + 2));
    char *clean_sugg = NULL;

    my_strncpy(final_sugg, curr_word, prefix_len);
    final_sugg[prefix_len] = '\0';
    if (is_link) {
        clean_sugg = my_strdup(current_sugg);
        clean_sugg[my_strlen(clean_sugg) - 1] = '\0';
        my_strcat(final_sugg, clean_sugg);
        free(clean_sugg);
    } else {
        my_strcat(final_sugg, current_sugg);
    }
    return final_sugg;
}

/**
 * @brief Insert the suggestion into the buffer, cleaning it if necessary.
 *
 * @param ti Terminal information structure
 * @param current_sugg The suggestion to insert
 * @param is_link Whether the suggestion is a symbolic link
 */
static void insert_suggestion(OUT term_info_t *ti, IN char *current_sugg,
    IN bool is_link)
{
    size_t len = my_strlen(current_sugg);
    char *clean = NULL;

    if (is_link) {
        clean = my_strdup(current_sugg);
        clean[len - 1] = '\0';
        for (size_t i = 0; i < len - 1; i++)
            handle_character(ti, clean[i]);
        free(clean);
        return;
    }
    for (size_t i = 0; i < len; i++) {
        if (current_sugg[i] == ' ')
            handle_character(ti, '\\');
        handle_character(ti, current_sugg[i]);
    }
}

/**
 * @brief A function to modify a single buffer
 *
 * @param ti
 * @param current_sugg
 */
void modify_buffer_single(IN term_info_t *ti, IN char *current_sugg)
{
    int len = my_strlen(current_sugg);

    for (int i = 0; i < len; i++) {
        handle_character(ti, current_sugg[i]);
    }
}

/**
 * @brief Modify the buffer by replacing the current word with the
 *        selected suggestion.
 *
 * @param ti Terminal information structure
 * @param current_sugg The suggestion to insert
 * @param curr_word The current word to be replaced (will be freed)
 */
void modify_buffer_suggestion(IN term_info_t *ti, IN char *current_sugg,
    OUT char *curr_word)
{
    char *curr_word_without_slash = get_last_folder_char(curr_word);
    size_t to_delete = my_strlen(&curr_word_without_slash[1]);
    bool is_link = is_suggestion_link(current_sugg);
    char *final_sugg = build_final_suggestion(curr_word, current_sugg,
        is_link);
    bool is_dir = is_suggestion_directory(final_sugg, is_link);

    for (size_t i = 0; i < to_delete; i++)
        handle_backspace(ti);
    insert_suggestion(ti, current_sugg, is_link);
    if (is_dir)
        handle_character(ti, '/');
    free(curr_word);
    free(final_sugg);
}
