/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_autocomplete
** File description:
** This file contains functions related to the autocompletion system
*/

#include "core/minishell.h"

/**
 * @brief Print a single suggestion with his padding.
 *
 * @param index         The index of the suggestion in the total array
 * @param count         The number of suggestions
 * @param column_w      The padding to add
 * @param suggestion    The string suggestion to print
 */
static void print_single_suggestion(int index, int count, int column_w,
    char *suggestion)
{
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
static int get_padding_for_each_suggestions(int count, char *suggestions[])
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
    int i = 0;

    if (suggestions == NULL || count <= 0)
        return 0;
    padding = get_padding_for_each_suggestions(count, suggestions);
    num_columns = (ws.ws_col / padding) > 0 ? (ws.ws_col / padding) : 0;
    num_rows = (count + num_columns - 1) / num_columns;
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_columns; col++) {
            i = col * num_rows + row;
            print_single_suggestion(i, count, padding, suggestions[i]);
        }
        my_printf("\n");
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
    if (ti->_cursor_start_pos[POS_Y] >= get_screen_info().ws_row)
        ti->_cursor_start_pos[POS_Y] = get_screen_info().ws_row;
}

/**
 * @brief Handle the autocompletion by printing all suggestions on the screen
 *        if there is more than 1 or just strcat the result.
 *
 * @param ti            The terminal structure information
 */
void handle_autocomplete(OUT term_info_t *ti)
{
    char **suggestions = NULL;
    char *current_sugg = NULL;
    size_t current_len = 0;

    if (ti == NULL)
        return;
    if (suggestions == NULL)
        return;
    if (suggestions[0] != NULL && suggestions[1] == NULL) {
        current_sugg = suggestions[0];
        current_len = my_strlen(current_sugg);
        for (size_t i = 0; i < current_len; i++)
            handle_character(ti, current_sugg[i]);
        set_cursor_position(ti->_cursor_pos[POS_Y], ti->_cursor_pos[POS_X]);
        return;
    }
    setup_suggestions(ti, suggestions, 0);
}
