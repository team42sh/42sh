/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_shortcut
** File description:
** This file contains some shortcut like CTRL A or CTRL E
*/

#include "core/minishell.h"
#include "my_printf.h"

void handle_ctrl_a(OUT term_info_t *ti)
{
    if (ti == NULL)
        return;
    ti->_cursor_index = 0;
}

void handle_ctrl_e(OUT term_info_t *ti)
{
    if (ti == NULL)
        return;
    ti->_cursor_index = ti->_buffer_len;
}
