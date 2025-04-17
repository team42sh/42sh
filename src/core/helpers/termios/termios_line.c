/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_display.c
** File description:
** Functions for display management in terminal with line wrapping support
*/

#include "core/minishell.h"

/**
 * @brief Get the total number of lines occupied by the current input.
 *
 * @param prompt_len    Length of the prompt in characters
 * @param buffer_len    Length of the input buffer in characters
 * @param cols          Width of the terminal in columns
 *
 * @return Number of lines occupied.
 */
static size_t get_line_count(size_t prompt_len, size_t buffer_len, size_t cols)
{
    size_t total_len = prompt_len + buffer_len;

    return (total_len + cols - 1) / cols;
}

/**
 * @brief Clear the current input display by erasing lines and
 *        resetting cursor position
 *
 * @param lines         Number of lines to clear
 */
static void clear_input_display(size_t lines)
{
    char up_cmd[32] = {0};

    write(STDOUT_FILENO, "\r", 1);
    if (lines > 1) {
        snprintf(up_cmd, sizeof(up_cmd), "\033[%zuA", lines - 1);
        write(STDOUT_FILENO, up_cmd, my_strlen(up_cmd));
    }
    write(STDOUT_FILENO, "\033[J", 3);
}

/**
 * @brief Position the cursor at the correct location based on cursor index.
 *
 * @param prompt_len     Length of the prompt
 * @param cursor_index   Current cursor position in the buffer
 * @param cols           Width of the terminal in columns
 */
static void position_cursor(size_t prompt_len, size_t cursor_index,
    size_t cols)
{
    char down_cmd[32] = {0};
    char right_cmd[32] = {0};
    size_t cursor_pos = prompt_len + cursor_index + 1;
    size_t cursor_line = cursor_pos / cols;
    size_t cursor_col = cursor_pos % cols;

    write(STDOUT_FILENO, "\r", 1);
    if (cursor_line > 0) {
        snprintf(down_cmd, sizeof(down_cmd), "\033[%zuB", cursor_line);
        write(STDOUT_FILENO, down_cmd, my_strlen(down_cmd));
    }
    if (cursor_col > 0) {
        snprintf(right_cmd, sizeof(right_cmd), "\033[%zuC", cursor_col);
        write(STDOUT_FILENO, right_cmd, my_strlen(right_cmd));
    }
}

/**
 * @brief Highlight the character under the cursor if show_cursor is true.
 *
 * @param ti            Term information structure
 * @param show_cursor   Whether to highlight the cursor or not
 */
static void highlight_cursor(term_info_t *ti, bool show_cursor)
{
    if (!show_cursor)
        return;
    if (ti->_cursor_index >= ti->_buffer_len) {
        write(STDOUT_FILENO, "\033[6;30;48;5;254m \033[0m", 21);
    } else {
        write(STDOUT_FILENO, "\033[6;30;48;5;254m", 16);
        write(STDOUT_FILENO, &ti->_buffer[ti->_cursor_index], 1);
        write(STDOUT_FILENO, "\033[0m", 4);
    }
}

/**
 * @brief Print the input line with the cursor.
 *        Handles multi-line input correctly with terminal wrapping.
 *
 * @param ti            Term information structure
 * @param show_cursor   Show the cursor or not
 */
void print_input_termios(IN term_info_t *ti, IN bool show_cursor)
{
    size_t cols = 0;
    size_t prompt_len = 0;
    size_t current_lines = 0;

    if (ti == NULL)
        return;
    cols = get_terminal_width();
    prompt_len = get_len_prompt();
    current_lines = get_line_count(prompt_len, ti->_buffer_len, cols);
    clear_input_display(current_lines);
    print_shell_prompt();
    write(STDOUT_FILENO, ti->_buffer, ti->_buffer_len);
    position_cursor(prompt_len, ti->_cursor_index, cols);
    highlight_cursor(ti, show_cursor);
}
