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
