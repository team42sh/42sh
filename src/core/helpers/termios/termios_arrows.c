/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_arrows
** File description:
** This file contains functions for handling arrows
*/

#include "core/minishell.h"

void handle_left_arrow(OUT term_info_t *ti)
{
    ti->_cursor_index--;
    CURSOR_LEFT_N(1);
    get_cursor_position(&ti->_cursor_pos[Y], &ti->_cursor_pos[X]);
}

void handle_right_arrow(OUT term_info_t *ti)
{
    ti->_cursor_index++;
    CURSOR_RIGHT_N(1);
    get_cursor_position(&ti->_cursor_pos[Y], &ti->_cursor_pos[X]);
}

/**
 * @brief Handles navigation through the command history using the up/down keys
 *        This function updates the terminal information structure `ti` for
 *        the user's request to move either up or down in the command history.
 *
 * @param up    A boolean indicating the direction:
 *               - `true` to move up (previous command),
 *               - `false` to move down (next command)
 * @param ti    A pointer to the term_info_t structure
 */
void handle_history_up_down(IN bool up, OUT term_info_t *ti)
{
    if (up && ti->_history_index < get_shell()->_max_history - 1) {
        reset_buffer_termios(ti);
        (ti->_history_index)++;
        if (ti->_history_index >= 0) {
            my_strcpy(ti->_buffer, get_string_index(
                get_shell()->_history_input, ti->_history_index));
            ti->_buffer_len = my_strlen(ti->_buffer);
            ti->_cursor_index = ti->_buffer_len;
        }
    }
    if (!up && ti->_history_index > -1) {
        (ti->_history_index)--;
        reset_buffer_termios(ti);
        if (ti->_history_index >= 0) {
            my_strcpy(ti->_buffer, get_string_index(
                get_shell()->_history_input, ti->_history_index));
            ti->_buffer_len = my_strlen(ti->_buffer);
            ti->_cursor_index = ti->_buffer_len;
        }
    }
}
