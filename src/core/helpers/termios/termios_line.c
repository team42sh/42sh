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
 * @param ti         The terminal structure information
 */
void print_multiline_buffer(IN term_info_t *ti)
{
    struct winsize screen = get_screen_info();
    int x = ti->_cursor_start_pos[X];
    int y = ti->_cursor_start_pos[Y];

    set_cursor_position(y, x);
    CLEAR_SCREEN_FROM_CURSOR();
    for (size_t i = 0; i < ti->_buffer_len; i++) {
        write(STDOUT_FILENO, &ti->_buffer[i], 1);
        x++;
        if (x <= screen.ws_col)
            continue;
        x = 1;
        y++;
        if (y > screen.ws_row)
            y = screen.ws_row;
    }
}

/**
 * @brief Set the cursor to this position.
 *
 * @param y             The Y
 * @param x             The X
 */
void set_cursor_position(IN int y, IN int x)
{
    char escape_sequence[32] = {0};

    snprintf(escape_sequence, sizeof(escape_sequence), "\033[%d;%dH", y, x);
    write(STDOUT_FILENO, escape_sequence, my_strlen(escape_sequence));
}
