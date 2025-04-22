/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_cursor
** File description:
** This file contains functions related on the cursor.
*/

#include "core/minishell.h"

/**
 * @brief Get the cursor position.
 *
 * @param row           Pointer to the row (Y)
 * @param col           Pointer to the col (X)
 */
void get_cursor_position(OUT int *row, OUT int *col)
{
    char buf[32];
    unsigned long i = 0;

    if (row == NULL || col == NULL)
        return;
    write(STDOUT_FILENO, "\033[6n", 4);
    while (i < sizeof buf - 1) {
        if (read(STDIN_FILENO, buf + i, 1) != 1)
            break;
        if (buf[i] == 'R')
            break;
        i++;
    }
    buf[i] = '\0';
    if (buf[0] == '\033' && buf[1] == '[')
        sscanf(buf + 2, "%d;%d", row, col);
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

    if (y < 0 || x < 0)
        return;
    snprintf(escape_sequence, sizeof(escape_sequence), "\033[%d;%dH", y, x);
    write(STDOUT_FILENO, escape_sequence, my_strlen(escape_sequence));
}

/**
 * @brief This function will move your cursor MOVE time and will ajust the
 * multiline automatically.
 *
 * @param move          The number of time to move the cursor
 */
void move_cursor_from_position(IN int move, OUT term_info_t *ti)
{
    int x = ti->_cursor_pos[POS_X];
    int y = ti->_cursor_pos[POS_Y];
    struct winsize ws = get_screen_info();

    for (int i = 0; i < move; i++) {
        x++;
        if (x <= ws.ws_col)
            continue;
        x = 2;
        y++;
        if (y > ws.ws_row)
            y = ws.ws_row;
    }
    ti->_cursor_pos[POS_Y] = y;
    ti->_cursor_pos[POS_X] = x;
}
