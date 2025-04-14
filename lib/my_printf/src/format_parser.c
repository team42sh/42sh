/*
** EPITECH PROJECT, 2024
** format_parser
** File description:
** Parser for my_printf which returns a
** structure with all parsed informations
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "my_printf.h"

char const *flags_var = "#0 -+";
char const *conversion = "dcfFgGeEsSbouxXin%aAp";
char const *modifier_str = "hhllqLjzt";
char const *modifiers[] = {"h", "hh", "l", "ll", "q", "L", "j", "z", "t"};

static int pow_ten(int exponent)
{
    int i = 0;
    int result = 1;

    while (i < exponent) {
        result *= 10;
        i++;
    }
    return result;
}

static int parse_flag(char const *str, formatter_t *format, int j)
{
    int i = 0;
    char c_flags[6] = {'.', '.', '.', '.', '.', '\0'};

    while (str[j] != '\0') {
        if (is_in_str(str[j], flags_var) != 0 && !is_in_str(str[j], c_flags)) {
            format->flags[i] = str[j];
            c_flags[i] = str[j];
            i++;
            j++;
            continue;
        }
        if (!is_in_str(str[j], flags_var) && !is_in_str(str[j], conversion) &&
            !is_in_str(str[j], modifier_str) &&
            (str[j] < '0' || str[j] > '9') && str[j] != '.' && str[j] != '*')
            return -1;
        if (!is_in_str(str[j], flags_var))
            return j;
        j++;
    }
    return j;
}

static int parse_width(char const *str, formatter_t *format, int start_search)
{
    int width = 0;
    int width_len = 0;
    int j = start_search;

    if (start_search == -1)
        return -1;
    if (is_in_str(str[start_search], conversion) != 0)
        return start_search;
    width_len = get_width_len(start_search, str);
    for (j = start_search; str[j] != '\0'; j++) {
        if (str[j] >= '0' && str[j] <= '9') {
            width += (str[j] - 48) * pow_ten(width_len - j + start_search - 1);
        } else
            break;
    }
    format->width = width;
    if (!is_in_str(str[j], conversion) &&
        !is_in_str(str[j], modifier_str) && str[j] != '.')
        return -1;
    return j;
}

static int get_precision_length(char const *str, int start)
{
    int precision_length = 0;

    if (start == -1)
        return -1;
    if (str[start] != '.') {
        return 0;
    }
    for (int j = start + 1; str[j] != '\0'; j++) {
        if (str[j] >= '0' && str[j] <= '9') {
            precision_length++;
        } else
            break;
    }
    return precision_length;
}

static int get_start_precision(char const *str, int start)
{
    int j = start;

    if (start == -1)
        return -1;
    if (is_in_str(str[start], conversion) != 0)
        return -1;
    if (str[start] == '.') {
        return start;
    }
    for (int i = j; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            return i;
        }
    }
    return -1;
}

static int parse_precision(char const *str, formatter_t *format, int start)
{
    int j = start;
    int precision = 0;
    int precision_length = get_precision_length(str, start);

    start = get_start_precision(str, start);
    if (start == -1)
        return j;
    for (j = start + 1; str[j] != '\0'; j++) {
        if (str[j] >= '0' && str[j] <= '9') {
            precision += (str[j] - 48) * pow_ten(precision_length - j + start);
        } else
            break;
    }
    if (str[start] == '.' && str[start + 1] == '-' && precision_length == 0)
        precision = -1;
    format->precision = precision;
    if (!is_in_str(str[j], conversion) && !is_in_str(str[j], modifier_str))
        return -1;
    return j;
}

static int parse_modifier(char const *str, formatter_t *format, int start)
{
    if (start == -1)
        return -1;
    if (is_in_str(str[start], conversion) != 0)
        return start;
    for (int i = 0; i < 9; i++) {
        if (my_strstr(&str[start], modifiers[i]) != NULL) {
            format->length[0] = modifiers[i][0];
            format->length[1] = modifiers[i][1];
            format->length[2] = modifiers[i][2];
            break;
        }
    }
    return start + my_strlen(format->length);
}

static void remove_error_case(formatter_t *format, char const *str)
{
    if (str[0] == '\0') {
        format = NULL;
        return;
    }
    if (format->flags[0] == '\0')
        return;
    if (!is_in_str('-', format->flags) || !is_in_str('0', format->flags)) {
        return;
    }
    for (int i = 0; i < my_strlen(format->flags); i++) {
        if (format->flags[i] == '0') {
            format->flags[i] = 'N';
        }
    }
}

static void find_conv_index(formatter_t *format, char const *str, int i)
{
    for (int j = i; str[j] != '\0'; j++) {
        if (is_in_str(str[j], conversion)) {
            format->conv_index = j;
            break;
        }
    }
    remove_error_case(format, str);
}

formatter_t *parse_string(char const *str, va_list *list)
{
    formatter_t *format = malloc(sizeof(formatter_t));
    int i = 0;

    init_formatter(format);
    i = parse_flag(str, format, 1);
    if (str[i] == '*') {
        format->width = va_arg(*list, int);
        i++;
    } else
        i = parse_width(str, format, i);
    if (str[i] == '.' && str[i + 1] == '*') {
        format->precision = va_arg(*list, int);
        i += 2;
    } else
        i = parse_precision(str, format, i);
    i = parse_modifier(str, format, i);
    if (i == -1)
        return NULL;
    find_conv_index(format, str, i);
    return format;
}
