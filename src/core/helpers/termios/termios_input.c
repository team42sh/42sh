/*
** EPITECH PROJECT, 2025
** src/core/helpers/termios/termios_input
** File description:
** This file contains functions for getting the input
*/

#include "core/minishell.h"

/**
* @brief DEBUG function to print the cursor at the top of the screen.
*        Printing :
*         - Cursor start
*         - Cursor pos
*         - Max width and height of the terminal configuration
*
* @param ti             Term information structure
*/
static void debug_termios_info(IN term_info_t *ti)
{
    struct winsize ws = get_screen_info();

    if (_DEBUG_MODE_ != 1 || ti == NULL)
        return;
    set_cursor_position(1, 1);
    my_printf("Cursor Start (%d, %d)\033[K", ti->_cursor_start_pos[POS_X],
        ti->_cursor_start_pos[POS_Y]);
    set_cursor_position(2, 0);
    my_printf("Cursor Pos (%d, %d)\033[K", ti->_cursor_pos[POS_X],
        ti->_cursor_pos[POS_Y]);
    set_cursor_position(3, 0);
    my_printf("Max width : %d  Max Height : %d\033[K", ws.ws_col, ws.ws_row);
    set_cursor_position(4, 0);
    my_printf("\033[33;1m");
    for (int i = 0; i < ws.ws_col; i++)
        write(1, "-", 1);
    my_printf(RESET_COLOR);
    set_cursor_position(ti->_cursor_pos[POS_Y], ti->_cursor_pos[POS_X]);
}

/**
 * @brief Handles the deletion of a character using backspace.
 *        Moves the cursor one position to the left and removes the character
 *        at that position, shifting the rest of the buffer.
 *
 * @param ti            Term information structure
 */
static void handle_backspace(OUT term_info_t *ti)
{
    struct winsize ws = get_screen_info();

    if (ti == NULL || ti->_cursor_index <= 0)
        return;
    (ti->_cursor_index)--;
    (ti->_buffer_len)--;
    memmove(&ti->_buffer[ti->_cursor_index],
        &ti->_buffer[ti->_cursor_index + 1],
        ti->_buffer_len - ti->_cursor_index + 1);
    ti->_cursor_pos[POS_X]--;
    if (ti->_cursor_pos[POS_X] < 1) {
        ti->_cursor_pos[POS_X] = ws.ws_col - 1;
        ti->_cursor_pos[POS_Y]--;
        if (ti->_cursor_pos[POS_Y] < ti->_cursor_start_pos[POS_Y]) {
            ti->_cursor_pos[POS_Y] = ti->_cursor_start_pos[POS_Y];
            ti->_cursor_pos[POS_X] = ti->_cursor_start_pos[POS_X];
        }
    }
}

/**
 * @brief Inserts a character into the buffer at the current cursor position.
 *        Shifts the content of the buffer to the right and
 *        add the new character.
 *
 * @param ti            Term information structure
 * @param c             The character to insert
 */
static void handle_character(OUT term_info_t *ti, IN char c)
{
    struct winsize ws = get_screen_info();

    if (ti == NULL || ti->_buffer_len >= BUFFER_TERMIOS_SIZE)
        return;
    memmove(&ti->_buffer[ti->_cursor_index + 1],
        &ti->_buffer[ti->_cursor_index],
        ti->_buffer_len - ti->_cursor_index);
    ti->_buffer[ti->_cursor_index] = c;
    ti->_cursor_index++;
    ti->_buffer_len++;
    ti->_cursor_pos[POS_X]++;
    if (ti->_cursor_pos[POS_X] > ws.ws_col) {
        ti->_cursor_pos[POS_X] = 2;
        ti->_cursor_pos[POS_Y]++;
        if (ti->_cursor_pos[POS_Y] > ws.ws_row)
            ti->_cursor_pos[POS_Y] = ws.ws_row;
    }
}

/**
 * @brief Handles the escape key press in the terminal.
 *        This function is called when the escape key (`ESC`) is detected.
 *        It updates the `term_info_t` structure accordingly, potentially
 *        resetting or modifying the terminal state.
 *
 * @param ti            A pointer to the term_info_t structure
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
        handle_left_arrow(ti);
    if (sequence[1] == 'C' && ti->_cursor_index < ti->_buffer_len)
        handle_right_arrow(ti);
    if (sequence[1] == 'A')
        handle_history_up(ti);
    if (sequence[1] == 'B')
        handle_history_down(ti);
}

/**
* @brief Handle shortcut if there is a shorcut, return NULL if there is no
*        shorcuts.
*
* @param c              The character
* @param ti             Term information structure
*/
static bool handle_shortcuts(IN char c, OUT term_info_t *ti)
{
    if (ti == NULL)
        return false;
    if (c == CTRL_A_VALUE)
        handle_ctrl_a(ti);
    if (c == CTRL_E_VALUE)
        handle_ctrl_e(ti);
    if (c == CTRL_K_VALUE)
        handle_ctrl_k(ti);
    if (c == CTRL_Y_VALUE)
        handle_ctrl_y(ti);
    if (c == CTRL_A_VALUE || c == CTRL_K_VALUE || c == CTRL_E_VALUE ||
        c == CTRL_Y_VALUE)
        return true;
    return false;
}

/**
* @brief Handle the ENTER character. If you press ENTER then we reset cursor
*        and return true.
*
* @param ti             Term information structure
*
* @return TRUE means we strdup the buffer and we just finished the input.
*/
static bool handle_enter(OUT term_info_t *ti)
{
    if (ti == NULL)
        return false;
    handle_ctrl_e(ti);
    set_cursor_position(ti->_cursor_pos[POS_Y], ti->_cursor_pos[POS_X]);
    write(1, "\n", 1);
    return true;
}

/**
 * @brief Handles special input characters like TAB, ENTER, and BACKSPACE.
 *        Performs the appropriate operation based on the character
 *        read from stdin.
 *
 * @param c             The character to interpret
 * @param ti            Term information structure
 *
 * @return true if input is complete (exemple: ENTER pressed).
 */
static bool choose_char_case(IN char c, OUT term_info_t *ti)
{
    if (ti == NULL)
        return false;
    if (c == AUTO_COMPLETE_CHAR || c == CTRL_D_VALUE)
        return false;
    if (c == ENTER_CHAR)
        return handle_enter(ti);
    if (c == ESCAPE_CHAR) {
        handle_escape(ti);
        return false;
    }
    if (c == BACKSPACE_VALUE || c == CTRL_H_VALUE)
        handle_backspace(ti);
    if (handle_shortcuts(c, ti))
        return false;
    if (c != ESCAPE_CHAR && c != ENTER_CHAR && c != AUTO_COMPLETE_CHAR &&
        c != CTRL_D_VALUE && c != BACKSPACE_VALUE && c != CTRL_H_VALUE)
        handle_character(ti, c);
    return false;
}

/**
 * @brief Resets the input buffer and its variables.
 *        Called when a signal (exemple SIGINT) requests to clear the buffer.
 *
 * @param ti            Term information structure
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

    setup_new_prompt(ti);
    debug_termios_info(ti);
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (get_shell()->_term_info->_sig_buffer_reset)
            reset_buffer_termios(ti);
        if (c == CTRL_D_VALUE && ti->_buffer_len == 0) {
            reset_buffer_termios(ti);
            return NULL;
        }
        if (choose_char_case(c, ti) == true)
            return my_strdup(ti->_buffer);
        ti->_buffer[ti->_buffer_len] = '\0';
        print_multiline_buffer(ti);
        debug_termios_info(ti);
    }
    return my_strdup(ti->_buffer);
}
