/*
** EPITECH PROJECT, 2024
** my_strstr
** File description:
** A function that finds a string into another string
*/

#include <stddef.h>
#include "my_printf.h"

static int get_first_index(char const *str, char const *to_find, int i)
{
    int len_find = my_strlen(to_find);
    int j = 0;

    while (str[i + j] == to_find[j]) {
        if (j == len_find - 1) {
            return (i + j - len_find + 1);
        }
        j++;
    }
    return -1;
}

char const *my_strstr(char const *str, char const *to_find)
{
    int len_str = my_strlen(str);
    int i = 0;
    int first_occ = -1;

    while (i < len_str) {
        if (get_first_index(str, to_find, i) != -1) {
            first_occ = get_first_index(str, to_find, i);
            break;
        } else {
            first_occ = -1;
        }
        i++;
    }
    if (first_occ == -1) {
        return NULL;
    }
    i = first_occ;
    return &str[i];
}
