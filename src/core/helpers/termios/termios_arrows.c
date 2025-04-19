/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_arrows
** File description:
** This file contains functions for handling arrows
*/

#include "core/minishell.h"

/**
* @brief Handle the left ARROW, by moving the cursor index in the buffer.
*        And moving the cursor to right position.
*
* @param ti     A pointer to the term_info_t structure
*/
void handle_left_arrow(OUT term_info_t *ti)
{
    ti->_cursor_index--;
    if (ti->_cursor_pos[POS_X] == 1) {
        CURSOR_UP_N(1);
        CURSOR_ANCHOR_LEFT();
        CURSOR_RIGHT_N(get_screen_info().ws_col);
    } else
        CURSOR_LEFT_N(1);
    get_cursor_position(&ti->_cursor_pos[POS_Y], &ti->_cursor_pos[POS_X]);
}

/**
* @brief Handle the right ARROW, by moving the cursor index in the buffer.
*        And moving the cursor to right position.
*
* @param ti     A pointer to the term_info_t structure
*/
void handle_right_arrow(OUT term_info_t *ti)
{
    ti->_cursor_index++;
    if (ti->_cursor_pos[POS_X] == get_screen_info().ws_col - 1) {
        CURSOR_DOWN_N(1);
        CURSOR_ANCHOR_LEFT();
        get_cursor_position(&ti->_cursor_pos[POS_Y], &ti->_cursor_pos[POS_X]);
        return;
    }
    if (ti->_cursor_pos[POS_X] > get_screen_info().ws_col - 1) {
        CURSOR_DOWN_N(1);
        CURSOR_ANCHOR_LEFT();
        CURSOR_RIGHT_N(1);
        get_cursor_position(&ti->_cursor_pos[POS_Y], &ti->_cursor_pos[POS_X]);
        return;
    }
    CURSOR_RIGHT_N(1);
    get_cursor_position(&ti->_cursor_pos[POS_Y], &ti->_cursor_pos[POS_X]);
}

/**
 * @brief Handles navigation through the command history using the up key
 *        This function updates the terminal information structure `ti` for
 *        the user's request to move either up in the command history.
 *
 * @param ti    A pointer to the term_info_t structure
 */
void handle_history_up(OUT term_info_t *ti)
{
    if (ti->_history_index < get_shell()->_max_history - 1) {
        reset_buffer_termios(ti);
        handle_ctrl_a(ti);
        (ti->_history_index)++;
        if (ti->_history_index >= 0) {
            my_strcpy(ti->_buffer, get_string_index(
                get_shell()->_history_input, ti->_history_index));
            ti->_buffer_len = my_strlen(ti->_buffer);
            ti->_cursor_index = ti->_buffer_len;
            handle_ctrl_e(ti);
        }
    }
}

/**
 * @brief Handles navigation through the command history using the down key
 *        This function updates the terminal information structure `ti` for
 *        the user's request to move either down in the command history.
 *
 * @param ti    A pointer to the term_info_t structure
 */
void handle_history_down(OUT term_info_t *ti)
{
    if (ti->_history_index > -1) {
        (ti->_history_index)--;
        reset_buffer_termios(ti);
        handle_ctrl_a(ti);
        if (ti->_history_index >= 0) {
            my_strcpy(ti->_buffer, get_string_index(
                get_shell()->_history_input, ti->_history_index));
            ti->_buffer_len = my_strlen(ti->_buffer);
            ti->_cursor_index = ti->_buffer_len;
            handle_ctrl_e(ti);
        }
    }
}
