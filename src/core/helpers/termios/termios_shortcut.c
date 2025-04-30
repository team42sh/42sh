/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_shortcut
** File description:
** This file contains some shortcut like CTRL A or CTRL E
*/

#include "core/minishell.h"

/**
 * @brief Reset and put at 0 every char in the yank buffer.
 *
 * @param ti             The structure terminal information
 */
static void reset_yank_buffer(OUT term_info_t *ti)
{
    for (size_t i = 0; i < BUFFER_TERMIOS_SIZE; i++) {
        ti->_yank_buffer[i] = 0;
    }
}

/**
 * @brief Handling of the shortcut CTRL A
 *
 * @param ti            The structure terminal information
 */
void handle_ctrl_a(OUT term_info_t *ti)
{
    if (ti == NULL)
        return;
    ti->_cursor_index = 0;
    ti->_cursor_pos[POS_Y] = ti->_cursor_start_pos[POS_Y];
    ti->_cursor_pos[POS_X] = ti->_cursor_start_pos[POS_X];
}

/**
 * @brief Handling of the shortcut CTRL E
 *
 * @param ti            The structure terminal information
 */
void handle_ctrl_e(OUT term_info_t *ti)
{
    struct winsize ws = get_screen_info();
    int pos[2] = {0};

    if (ti == NULL)
        return;
    ti->_cursor_index = ti->_buffer_len;
    pos[POS_X] = ti->_cursor_start_pos[POS_X];
    pos[POS_Y] = ti->_cursor_start_pos[POS_Y];
    for (size_t i = 0; i < ti->_buffer_len; i++) {
        pos[POS_X]++;
        if (pos[POS_X] <= ws.ws_col)
            continue;
        pos[POS_X] = 2;
        pos[POS_Y]++;
        if (pos[POS_Y] > ws.ws_row)
            pos[POS_Y] = ws.ws_row;
    }
    ti->_cursor_pos[POS_X] = pos[POS_X];
    ti->_cursor_pos[POS_Y] = pos[POS_Y];
}

/**
 * @brief Handling of the shortcut CTRL K
 *
 * @param ti            The structure terminal information
 */
void handle_ctrl_k(OUT term_info_t *ti)
{
    size_t moved_i = 0;

    if (ti == NULL)
        return;
    reset_yank_buffer(ti);
    for (size_t i = ti->_cursor_index; i < ti->_buffer_len; i++) {
        ti->_yank_buffer[moved_i] = ti->_buffer[i];
        ti->_buffer[i] = 0;
        moved_i++;
    }
    ti->_buffer_len = ti->_buffer_len - moved_i;
    ti->_cursor_index = ti->_buffer_len;
}

/**
 * @brief Handling of the shortcut CTRL Y
 *
 * @param ti            The structure terminal information
 */
void handle_ctrl_y(OUT term_info_t *ti)
{
    size_t yank_len = 0;

    if (ti == NULL)
        return;
    yank_len = my_strlen(ti->_yank_buffer);
    if (ti->_buffer_len >= BUFFER_TERMIOS_SIZE - yank_len)
        return;
    for (size_t i = 0; i < yank_len; i++) {
        memmove(&ti->_buffer[ti->_cursor_index + 1],
            &ti->_buffer[ti->_cursor_index],
            ti->_buffer_len - ti->_cursor_index);
        ti->_buffer[ti->_cursor_index] = ti->_yank_buffer[i];
        ti->_cursor_index++;
        ti->_buffer_len++;
    }
    move_cursor_from_position(yank_len, ti);
}
