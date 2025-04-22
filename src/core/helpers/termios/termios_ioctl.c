/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_ioctl
** File description:
** This file contains functions related with the ioctl
*/

#include "core/minishell.h"

/**
 * @brief Get the terminal line width.
 *
 * @return The terminal width.
 */
struct winsize get_screen_info(void)
{
    struct winsize win = {0};

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    win.ws_col++;
    return win;
}

/**
 * @brief Check if there is any remaining input in the STDIN.
 *        This function is used for the copy paste multiples lines.
 *
 * @return TRUE or FALSE.
 */
bool has_remaining_input(void)
{
    int bytes = 0;

    ioctl(STDIN_FILENO, FIONREAD, &bytes);
    return bytes > 0;
}

/**
 * @brief Print the whole STDIN if there is any bytes in it.
 */
void print_remaining_stdin(void)
{
    char buffer[1024] = {0};
    size_t bytes = 0;
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    bytes = read(STDIN_FILENO, buffer, sizeof buffer);
    while (bytes - 1 > 0) {
        buffer[bytes] = '\0';
        write(STDOUT_FILENO, buffer, bytes);
        bytes = read(STDIN_FILENO, buffer, sizeof buffer);
    }
    fcntl(STDIN_FILENO, F_SETFL, flags);
}
