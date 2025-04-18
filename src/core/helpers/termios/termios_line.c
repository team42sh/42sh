/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_display.c
** File description:
** Functions for display management in terminal with line wrapping support
*/

#include "core/minishell.h"

void print_multiline_string(const char *str, int start_pos)
{
    int term_width = get_screen_info().ws_col;
    int current_col = start_pos % term_width;
    int i = 0;

    if (str == NULL)
        return;
    while (str[i] != '\0') {
        if (current_col == term_width) {
            write(STDOUT_FILENO, "\r\n", 2);
            current_col = 0;
        }
        write(STDOUT_FILENO, &str[i], 1);
        current_col++;
        i++;
    }
}

void set_cursor_position(int y, int x)
{
    char escape_sequence[32] = {0};

    snprintf(escape_sequence, sizeof(escape_sequence), "\033[%d;%dH", y, x);
    write(STDOUT_FILENO, escape_sequence, my_strlen(escape_sequence));
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
    if (ti == NULL)
        return;
    (void) show_cursor;
}
