/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_shortcut
** File description:
** This file contains some shortcut like CTRL A or CTRL E
*/

#include "core/minishell.h"

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
}

/**
 * @brief Handling of the shortcut CTRL E
 *
 * @param ti            The structure terminal information
 */
void handle_ctrl_e(OUT term_info_t *ti)
{
    if (ti == NULL)
        return;
    ti->_cursor_index = ti->_buffer_len;
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
    if (ti->_buffer_len < BUFFER_TERMIOS_SIZE - (size_t) yank_len) {
        for (size_t i = 0; i < yank_len; i++) {
            memmove(&ti->_buffer[ti->_cursor_index + 1],
                &ti->_buffer[ti->_cursor_index],
                ti->_buffer_len - ti->_cursor_index);
            ti->_buffer[ti->_cursor_index] = ti->_yank_buffer[i];
            (ti->_cursor_index)++;
            (ti->_buffer_len)++;
        }
    }
}
