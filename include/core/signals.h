/*
** EPITECH PROJECT, 2025
** signals.h
** File description:
** This file contains all the stuff concerning signals
*/

#ifndef SIGNALS_H
    #define SIGNALS_H

    #include <sys/types.h>
    #include <signal.h>

    #include "types.h"

/*
 * BUILTS IN STRUCTURE
 */
typedef struct signal_data_s {
    signal_t signal;
    char *message;
} signal_data_t;

/*
 * DECLARATE IN ./src/core/handlers/signal_handler.c
 */
extern const signal_data_t SIGNALS[];

/*
 * Signals handler
 */
void setup_shell_signals(void);
int send_signal_to_pid(signal_t signal, pid_t pid);
void print_signal(signal_t signal, int core_dumped);

#endif /* ifndef SIGNALS_H */
