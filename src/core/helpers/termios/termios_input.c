/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_input
** File description:
** This file contains functions for getting the input
*/

#include "core/minishell.h"

/**
 * @brief Handles the deletion of a character using backspace.
 *        Moves the cursor one position to the left and removes the character
 *        at that position, shifting the rest of the buffer.
 *
 * @param ti          Term information structure
 */
static void handle_backspace(OUT term_info_t *ti)
{
    if (ti == NULL)
        return;
    if (ti->_cursor_index > 0) {
        (ti->_cursor_index)--;
        (ti->_buffer_len)--;
        memmove(&ti->_buffer[ti->_cursor_index],
            &ti->_buffer[ti->_cursor_index + 1],
            ti->_buffer_len - ti->_cursor_index + 1);
    }
}

/**
 * @brief Inserts a character into the buffer at the current cursor position.
 *        Shifts the content of the buffer to the right and
 *        add the new character.
 *
 * @param ti          Term information structure
 * @param c           The character to insert.
 */
static void handle_character(OUT term_info_t *ti, IN char c)
{
    if (ti == NULL)
        return;
    if (ti->_buffer_len < BUFFER_TERMIOS_SIZE - 1) {
        memmove(&ti->_buffer[ti->_cursor_index + 1],
            &ti->_buffer[ti->_cursor_index],
            ti->_buffer_len - ti->_cursor_index);
        ti->_buffer[ti->_cursor_index] = c;
        (ti->_cursor_index)++;
        (ti->_buffer_len)++;
    }
}

/**
 * @brief ..
 *
 * @param ti          Term information structure
 */
static void handle_escape(OUT term_info_t *ti)
{
    char sequence[2] = {0};

    if (ti == NULL)
        return;
    if (read(STDIN_FILENO, &sequence[0], 1) != 1)
        return;
    if (read(STDIN_FILENO, &sequence[1], 1) != 1)
        return;
    if (sequence[0] != '[')
        return;
    if (sequence[1] == 'D' && ti->_cursor_index > 0)
        (ti->_cursor_index)--;
    if (sequence[1] == 'C' && ti->_cursor_index < ti->_buffer_len)
        (ti->_cursor_index)++;
}

/**
 * @brief Handles special input characters like TAB, ENTER, and BACKSPACE.
 *        Performs the appropriate operation based on the character
 *        read from stdin.
 *
 * @param c                     The character to interpret
 * @param ti          Term information structure
 *
 * @return true if input is complete (exemple: ENTER pressed).
 */
static bool choose_char_case(IN char c, OUT term_info_t *term_info)
{
    switch (c) {
        case CTRL_D_VALUE:
        case '\t':
            my_printf("Auto Complete not implemented yet!\n");
            break;
        case '\n':
            print_input_termios(term_info, false);
            write(1, "\n", 1);
            return true;
        case '\e':
            handle_escape(term_info);
            break;
        case BACKSPACE_VALUE:
            handle_backspace(term_info);
            break;
        default:
            handle_character(term_info, c);
            break;
    }
    return false;
}

/**
 * @brief Resets the input buffer and its variables.
 *        Called when a signal (exemple SIGINT) requests to clear the buffer.
 *
 * @param ti          Term information structure
 */
void reset_buffer_termios(OUT term_info_t *term_info)
{
    if (term_info == NULL)
        return;
    for (int i = 0; i < BUFFER_TERMIOS_SIZE; i++)
        term_info->_buffer[i] = 0;
    term_info->_buffer_len = 0;
    term_info->_cursor_index = 0;
    get_shell()->_term_info->_sig_buffer_reset = 0;
}

/**
 * @brief Print the input line with the cursor.
 *        Used when pressing a key or Ctrl C in signal_handler.c
 *
 * @param ti            Term information structure
 * @param show_cursor   Show the cursor or not
 */
void print_input_termios(IN term_info_t *term_info, IN bool show_cursor)
{
    if (term_info == NULL)
        return;
    RESET_LINE();
    print_shell_prompt();
    write(STDOUT_FILENO, term_info->_buffer, term_info->_cursor_index);
    if (show_cursor)
        my_printf("\033[6;30;48;5;254m");
    if ((term_info->_cursor_index >= term_info->_buffer_len ||
        term_info->_buffer_len == 0) && show_cursor)
        write(1, " \033[0m", 5);
    else if (term_info->_buffer_len > 0)
        my_printf("%c\033[0m", term_info->_buffer[term_info->_cursor_index]);
    if (term_info->_buffer_len > 0)
        my_printf("\033[0m%s",
            &term_info->_buffer[term_info->_cursor_index + 1]);
}

/**
 * @brief Reads user input character-by-character in raw mode.
 *        This function handles basic line editing (insertions, backspace).
 *
 * @return A dynamically allocated string containing the user input,
 *         or NULL if input is interrupted.
 */
char *termios_get_input(void)
{
    term_info_t *ti = get_shell()->_term_info;
    char c;

    reset_buffer_termios(ti);
    print_input_termios(ti, true);
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (get_shell()->_term_info->_sig_buffer_reset)
            reset_buffer_termios(ti);
        if (c == CTRL_D_VALUE && ti->_buffer_len == 0) {
            reset_buffer_termios(ti);
            print_input_termios(ti, false);
            return NULL;
        }
        if (choose_char_case(c, ti) == true)
            return my_strdup(ti->_buffer);
        ti->_buffer[ti->_buffer_len] = '\0';
        print_input_termios(ti, true);
    }
    return my_strdup(ti->_buffer);
}
