/*
** EPITECH PROJECT, 2024
** flags.h
** File description:
** Header file containing all my_printf flags and their function
*/

#ifndef FLAGS_H
    #define FLAGS_H
    #include "my_printf.h"
    #include <stdarg.h>
struct flag {
    char character;
    int (*fct)(va_list *list, formatter_t *);
};
const struct flag FLAGS[] = {
    {'s', &print_s_flag},
    {'d', &print_int},
    {'i', &print_int},
    {'c', &print_c_flag},
    {'b', &print_b_flag},
    {'o', &print_o_flag},
    {'u', &print_u_flag},
    {'x', &print_x_flag},
    {'X', &print_uppercase_x_flag},
    {'f', &print_f_flag},
    {'F', &print_uppercase_f_flag},
    {'e', &print_e_flag},
    {'E', &print_uppercase_e_flag},
    {'g', &print_g_flag},
    {'G', &print_uppercase_g_flag},
    {'p', &print_address},
    {'a', &print_a_flag},
    {'A', &print_uppercase_a_flag}
};
#endif /* ifndef FLAGS_H */
