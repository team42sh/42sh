/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_autocomplete
** File description:
** This file contains functions related to the autocompletion system
*/

#include "core/minishell.h"
#include "my_printf.h"

/**
 * @brief Print a single suggestion with his padding.
 *
 * @param index         The index of the suggestion in the total array
 * @param count         The number of suggestions
 * @param column_w      The padding to add
 * @param suggestion    The string suggestion to print
 */
static void print_single_suggestion(IN int index, IN int count,
    IN int column_w, IN char *suggestion)
{
    if (suggestion == NULL)
        return;
    if (index < count) {
        my_printf("%-*s", column_w, suggestion);
    }
}

/**
 * @brief Get the padding to add for each suggestions.
 *
 * @param count          The number of suggestions
 * @param suggestions[]  The array of suggestions
 *
 * @return The padding.
 */
static int get_padding_for_each_suggestions(IN int count,
    IN char *suggestions[])
{
    int max_length = 0;
    int padding = 0;

    for (int i = 0; i < count; i++)
        if (my_strlen(suggestions[i]) > max_length)
            max_length = my_strlen(suggestions[i]);
    padding = max_length + 2;
    return padding;
}

/**
 * @brief Print the suggestions rows.
 *
 * @param args[5]               The parameters
 * @param suggestions[]         The array of suggestions
 */
static void print_suggestion_row(IN int args[5], IN char *suggestions[])
{
    int col = 0;
    int i = 0;
    int printed_something = 0;

    while (col < args[1]) {
        i = col * args[2] + args[0];
        if (i < args[3]) {
            print_single_suggestion(i, args[3], args[4], suggestions[i]);
            printed_something = 1;
        }
        col++;
    }
    if (printed_something)
        my_printf("\n");
}

/**
 * @brief Print all sugestions on the screen. By organized them as well.
 *
 * @param suggestions           The string array of suggestions
 * @param count                 The len of the string array
 */
static int print_suggestions(IN char *suggestions[], IN int count)
{
    struct winsize ws = get_screen_info();
    int padding = 0;
    int num_columns = 0;
    int num_rows = 0;
    int args[5] = {0};

    if (suggestions == NULL || count <= 0)
        return 0;
    padding = get_padding_for_each_suggestions(count, suggestions);
    num_columns = (ws.ws_col / padding) > 0 ? (ws.ws_col / padding) : 1;
    num_rows = (count + num_columns - 1) / num_columns;
    args[1] = num_columns;
    args[2] = num_rows;
    args[3] = count;
    args[4] = padding;
    for (int row = 0; row < num_rows; row++) {
        args[0] = row;
        print_suggestion_row(args, suggestions);
    }
    return num_rows;
}

/**
 * @brief Setup the cursor and print all suggestions on the screen.
 *
 * @param ti            The terminal structure information
 */
static void setup_suggestions(OUT term_info_t *ti, IN char **suggestions,
    IN int len)
{
    size_t suggestions_lines = 0;
    size_t number_line_buffer = get_lines_amount_buffer(ti);

    handle_ctrl_e(ti);
    set_cursor_position(ti->_cursor_pos[POS_Y], ti->_cursor_pos[POS_X]);
    NEW_LINE();
    suggestions_lines = print_suggestions(suggestions, len);
    print_shell_prompt();
    ti->_cursor_start_pos[POS_Y] += number_line_buffer + suggestions_lines;
    ti->_cursor_pos[POS_Y] += number_line_buffer + suggestions_lines;
    if (ti->_cursor_start_pos[POS_Y] >= get_screen_info().ws_row)
        ti->_cursor_start_pos[POS_Y] = get_screen_info().ws_row;
}

/**
 * @brief Get the word to make the autocomplete for.
 *
 * @param ti            The terminal structure
 *
 * @return The word to autocomplete.
 */
static char *get_word_until_chars(IN term_info_t *ti, IN char *c)
{
    int len = 0;
    char path[4096] = {0};

    for (int i = (int) ti->_cursor_index - 1; i >= 0; i--) {
        if (is_in_str(ti->_buffer[i], c))
            break;
        len++;
    }
    if (len == 0)
        return NULL;
    for (int i = 0; i < len; i++)
        path[i] = ti->_buffer[ti->_cursor_index - len + i];
    path[len] = '\0';
    return my_strdup(path);
}

/**
 * @brief Count the suggestions amount.
 *
 * @param suggestions           The suggestions
 *
 * @return The amount.
 */
static int count_suggestions_elem(IN char **suggestions)
{
    int index = 0;

    while (suggestions[index] != NULL)
        index++;
    return index;
}

/**
 * @brief Handle the autocompletion by printing all suggestions on the screen
 *        if there is more than 1 or just strcat the result.
 *
 * @param ti            The terminal structure information
 */
void handle_autocomplete(OUT term_info_t *ti)
{
    char *curr_word = get_word_until_chars(ti, "; ");
    char **suggestions = fill_autocomplete(curr_word);

    if (ti == NULL || suggestions == NULL) {
        free(curr_word);
        return;
    }
    if (suggestions[0] != NULL && suggestions[1] == NULL) {
        if (curr_word == NULL || curr_word[0] == '\0')
            modify_buffer_single(ti, suggestions[0]);
        else
            modify_buffer_suggestion(ti, suggestions[0], curr_word);
        free_array_string(suggestions);
        write(1, "\a", 1);
        return;
    }
    setup_suggestions(ti, suggestions, count_suggestions_elem(suggestions));
    free_array_string(suggestions);
    free(curr_word);
}
