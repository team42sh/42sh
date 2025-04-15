/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_input
** File description:
** This file contains functions for getting the input
*/

#include "core/minishell.h"

static void handle_backspace(size_t *cursor_index, size_t *buffer_len,
    char buffer[BUFFER_TERMIOS_SIZE])
{
    if (*cursor_index > 0) {
        (*cursor_index)--;
        (*buffer_len)--;
        memmove(&buffer[*cursor_index], &buffer[*cursor_index + 1],
            *buffer_len - *cursor_index + 1);
    }
}

static void handle_character(size_t *cursor_index, size_t *buffer_len,
    char buffer[BUFFER_TERMIOS_SIZE], char c)
{
    if (*buffer_len < BUFFER_TERMIOS_SIZE - 1) {
        memmove(&buffer[*cursor_index + 1], &buffer[*cursor_index],
            *buffer_len - *cursor_index);
        buffer[*cursor_index] = c;
        (*cursor_index)++;
        (*buffer_len)++;
    }
}

char *termios_get_input(void)
{
    char buffer[BUFFER_TERMIOS_SIZE] = {0};
    size_t buffer_len = 0;
    size_t cursor_index = 0;
    char c;

    while (read(STDIN_FILENO, &c, 1) == 1) {
        switch (c) {
            case 4:
                return NULL;
            case '\t':
                my_printf("TG Y'A PAS D'AUTOCOMPLETE\n");
                break;
            case 127:
                handle_backspace(&cursor_index, &buffer_len, buffer);
                break;
            default:
                handle_character(&cursor_index, &buffer_len, buffer, c);
                break;
        }
        buffer[buffer_len] = '\0';
        write(1, "\r", 1);
        print_shell_prompt();
        my_printf("%s", buffer);
    }
    return strdup(buffer);
}
