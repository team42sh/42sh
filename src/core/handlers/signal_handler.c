/*
** EPITECH PROJECT, 2025
** signal_handler.c
** File description:
** This file will do some actions dependant of the signal received
*/

#include "core/minishell.h"

/*
 * Declaration of all SIGNALS data.
 * (extern variable from signals.h).
 */
const signal_data_t SIGNALS[] = {
    {SIGSEGV, "Segmentation fault"},
    {SIGFPE, "Floating exception"},
    {SIGINT, ""},
    {SIGILL, "Illegal instruction"},
    {SIGTERM, "Terminated"},
    {SIGKILL, "Killed"},
    {SIGABRT, "Abort"},
    {SIGBUS, "Bus error"},
    {-1, NULL}
};

/*
 * Show the appropriate message when a signal occured using waitpid().
 * Theses are the signals handled :
 * - Segmentation fault
 * - Interruption
 * - Terminate
 * - Kill
 * - Abort
 * Show the message when a core dumped occured using waitpid().
 */
void print_signal(signal_t signal, int core_dumped)
{
    for (int i = 0; SIGNALS[i].message != NULL; i++) {
        if (SIGNALS[i].signal != signal)
            continue;
        print_err(SIGNALS[i].message);
        if (core_dumped)
            print_err(" (core dumped)\n");
        else
            print_err("\n");
        return;
    }
}

/*
 * Send a signal signal to the pid pid_t. Using kill function.
 */
int send_signal_to_pid(signal_t signal, pid_t pid)
{
    return kill(pid, signal);
}

/*
 * Function for handling every type of signal passed in the function signal.
 * Currently supporting :
 * - Ctrl + C : SIGINT
 * - Ctrl + D : ?
 */
void handle_signal(signal_t signal)
{
    shell_t *shell = get_shell();
    term_info_t *ti = shell->_term_info;

    if (signal == SIGINT) {
        ti->_sig_buffer_reset = true;
        if (shell->current_child_pid == -1) {
            handle_ctrl_e(ti);
            set_cursor_position(ti->_cursor_pos[POS_Y],
                ti->_cursor_pos[POS_X]);
            write(1, "\n", 1);
            print_shell_prompt();
            get_cursor_position(&ti->_cursor_pos[POS_Y],
                &shell->_term_info->_cursor_pos[POS_X]);
            get_cursor_position(&ti->_cursor_start_pos[POS_Y],
                &ti->_cursor_start_pos[POS_X]);
            shell->last_exit_code = 1;
            return;
        }
        send_signal_to_pid(SIGINT, shell->current_child_pid);
    }
}

/*
 * Setup the pointer of function for signals like Ctrl + D or Ctrl + C.
 * Currently supporting :
 * - Ctrl + C : SIGINT
 */
void setup_shell_signals(void)
{
    signal(SIGINT, &handle_signal);
}
