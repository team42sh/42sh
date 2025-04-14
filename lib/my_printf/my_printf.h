/*
** EPITECH PROJECT, 2024
** my_printf.h
** File description:
** Header file containing all functions nescessary to my_printf
*/

#ifndef MY_PRINTF_H
    #define MY_PRINTF_H
    #include <stdarg.h>
    #define ABS(x) ((x) < 0 ? -(x) : (x))
    #define POS(x) ((x) < 0 ? 0 : 1)
    #define MAX(a, b) (((a) < (b)) ? (b) : (a))
    #define MIN(a, b) (((a) < (b)) ? (a) : (b))
/* Utilities */
typedef struct formatter {
    char flags[6];
    int width;
    int precision;
    char length[3];
    int conv_index;
} formatter_t;

formatter_t *parse_string(char const *str, va_list *list);
char const *my_strstr(char const *, char const *);
int get_width_len(int, char const *);
char is_in_str(char, char const *);
int my_printf(char *, ...);
int my_putchar(char, int);
int my_strlen(char const *);
int my_put_nbr(unsigned long long, int);
int my_put_double(double, int, int);
int my_put_double_no_trailing(double, int, int);
int my_put_exp_no_trailing(double, int, int, int);
int my_putstr(char const *, int);
int show_width_int(formatter_t *, int, int);
int show_width_double(formatter_t *, int, int);
int show_width_hex(formatter_t *, int, int, int);
int is_left_alligned(formatter_t *);
int show_sign(int, formatter_t *);
void init_formatter(formatter_t *format);
/* char * flags */
int print_s_flag(va_list *list, formatter_t *format);
/* int flags */
int print_int(va_list *list, formatter_t *format);
/* char flags */
int print_percent_flag(formatter_t *);
int print_c_flag(va_list *list, formatter_t *format);
/* unsigned int flags */
int print_b_flag(va_list *list, formatter_t *format);
int print_o_flag(va_list *list, formatter_t *format);
int print_u_flag(va_list *list, formatter_t *format);
int print_x_flag(va_list *list, formatter_t *format);
int print_uppercase_x_flag(va_list *list, formatter_t *format);
/* double flags */
int print_uppercase_f_flag(va_list *list, formatter_t *format);
int print_f_flag(va_list *list, formatter_t *format);
int print_uppercase_e_flag(va_list *list, formatter_t *format);
int print_e_flag(va_list *list, formatter_t *format);
int print_g_flag(va_list *list, formatter_t *format);
int print_uppercase_g_flag(va_list *list, formatter_t *format);
int print_a_flag(va_list *list, formatter_t *format);
int print_uppercase_a_flag(va_list *list, formatter_t *format);
/* Misc flags */
int print_n_flag(va_list *list, int char_count);
int print_address(va_list *list, formatter_t *format);
#endif /* ifndef MY_PRINTF_H */
