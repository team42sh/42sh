/*
** EPITECH PROJECT, 2024
** my_printf
** File description:
** A function that reproduces printf with
** all conversions and almost every flags
*/

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "my_printf.h"
#include "flags.h"

static int perform_special(char *format, int char_count, va_list *list, int *i)
{
    formatter_t *parsed_format = parse_string(format, list);
    int return_value = 0;

    switch (format[parsed_format->conv_index]) {
        case 'n':
            (*i)++;
            return_value = print_n_flag(list, char_count);
            break;
        case '%':
            *i += print_percent_flag(parsed_format);
            return_value = 1;
            break;
        default:
            break;
    }
    free(parsed_format);
    return return_value;
}

static int perform(char *format, int char_count, va_list *list, int *index)
{
    int i = 0;
    int return_value = 0;
    formatter_t *parsed_format = parse_string(format, list);

    if (parsed_format == NULL)
        return my_putchar('%', 1);
    if (format[parsed_format->conv_index] == 'n' ||
        format[parsed_format->conv_index] == '%') {
        free(parsed_format);
        return perform_special(format, char_count, list, index);
    }
    switch (format[parsed_format->conv_index]) {
        default:
            while (FLAGS[i].character != format[parsed_format->conv_index])
                i++;
            *index += parsed_format->conv_index;
            return_value = FLAGS[i].fct(list, parsed_format);
            free(parsed_format);
            return return_value;
    }
}

int my_printf(char *format, ...)
{
    va_list args;
    int format_len = my_strlen(format);
    int char_count = 0;

    va_start(args, format);
    for (int i = 0; i < format_len; i++) {
        if (format[i] == '%') {
            char_count += perform(&format[i], char_count, &args, &i);
            continue;
        }
        my_putchar(format[i], 1);
        char_count++;
    }
    va_end(args);
    return char_count;
}
