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

    #define HIDE_CURSOR() my_printf("\033[?25l")
    #define SHOW_CURSOR() my_printf("\033[?25h")
    #define RESET_LINE() write(STDOUT_FILENO, "\r\033[2K", 5)

    #define CURSOR_COLOR "\033[6;30;48;5;254m"
    #define RESET_COLOR "\033[0m"

#endif /* ifndef MISC_MACROS_H_ */
