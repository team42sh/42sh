/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_display.c
** File description:
** Functions for display management in terminal with line wrapping support
*/

#include "core/minishell.h"

/**
 * @brief Print the buffer and use the multiline.
 *
 * @param ti          The terminal structure information
 */
void print_multiline_buffer(IN term_info_t *ti)
{
    struct winsize screen = get_screen_info();
    int x = ti->_cursor_start_pos[POS_X];
    int y = ti->_cursor_start_pos[POS_Y];

    set_cursor_position(y, x);
    CLEAR_SCREEN_FROM_CURSOR();
    for (size_t i = 0; i < ti->_buffer_len; i++) {
        write(STDOUT_FILENO, &ti->_buffer[i], 1);
        x++;
        if (x <= screen.ws_col && ti->_buffer[i] != '\n')
            continue;
        x = 2;
        y++;
        if (y <= screen.ws_row)
            continue;
        y = screen.ws_row;
        ti->_cursor_start_pos[POS_Y]--;
        if (ti->_cursor_index != ti->_buffer_len)
            ti->_cursor_pos[POS_Y]--;
    }
}

/**
 * @brief How many lines your current buffer is ?
 *        This begin at the cursor start pos.
 *
 * @param ti            The terminal structure information
 *
 * @return The number of lines.
 */
size_t get_lines_amount_buffer(IN term_info_t *ti)
{
    struct winsize screen = get_screen_info();
    int x = ti->_cursor_start_pos[POS_X];
    int y = ti->_cursor_start_pos[POS_Y];

    for (size_t i = 0; i < ti->_buffer_len; i++) {
        x++;
        if (x <= screen.ws_col)
            continue;
        x = 2;
        y++;
    }
    return y - ti->_cursor_start_pos[POS_Y] + 1;
}
