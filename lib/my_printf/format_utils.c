/*
** EPITECH PROJECT, 2024
** format_utils
** File description:
** Utilities function to complete format_parser
*/

#include "my_printf.h"

char is_in_str(char c, char const *str)
{
    for (int k = 0; str[k] != '\0'; k++) {
        if (str[k] == c) {
            return str[k];
        }
    }
    return 0;
}

void init_formatter(formatter_t *format)
{
    for (int d = 0; d < 6; d++) {
        format->flags[d] = '\0';
    }
    for (int l = 0; l < 3; l++) {
        format->length[l] = '_';
    }
    format->length[2] = '\0';
    format->width = 0;
    format->precision = -2;
}

int get_width_len(int start_search, char const *str)
{
    int width_len = 0;

    for (int i = start_search; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            width_len++;
        } else
            break;
    }
    return width_len;
}
