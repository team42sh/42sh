/*
** EPITECH PROJECT, 2025
** include/macros/misc_macros.h
** File description:
** This file contains all misc macro used in the project
*/

#ifndef MISC_MACROS_H_
    #define MISC_MACROS_H_

    /*
     * Misc MACROS.
     */
    #ifndef IN
        #define IN
    #endif /* ifndef IN */

    #ifndef OUT
        #define OUT
    #endif /* ifndef OUT */

    #define DEBUG_PRINT() my_printf("AAAAA\nAAAAA")

    #define CURSOR_HIDE() my_printf("\033[?25l")
    #define CURSOR_SHOW() my_printf("\033[?25h")
    #define CLEAR_LINE() my_printf("\033[2K")
    #define CLEAR_SCREEN_FROM_CURSOR() my_printf("\033[J")
    #define CLEAR_LINE_FROM_CURSOR() my_printf("\033[K")
    #define CURSOR_LINE_START() my_printf("\r")
    #define CURSOR_UP() my_printf("\033[A")
    #define CURSOR_DOWN() my_printf("\033[B")
    #define CURSOR_ANCHOR_LEFT() my_printf("\r")

    #define CURSOR_TO_COLUMN(col) my_printf("\033[%dG", (col))
    #define CURSOR_UP_N(n) my_printf("\033[%dA", (n))
    #define CURSOR_DOWN_N(n) my_printf("\033[%dB", (n))
    #define CURSOR_RIGHT_N(n) my_printf("\033[%dC", (n))
    #define CURSOR_LEFT_N(n) my_printf("\033[%dD", (n))

    #define RESET_COLOR "\033[0m"

#endif /* ifndef MISC_MACROS_H_ */
