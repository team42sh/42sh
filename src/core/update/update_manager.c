/*
** EPITECH PROJECT, 2025
** 42sh-ci
** File description:
** update_manager
*/

#include "build_infos.h"

#include "core/update.h"

int ask_for_update_prompt(void)
{
    char buffer[10];
    int do_update = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH,
        &get_shell()->_term_info->_original_termios);
    printf("Do you want to update? (y/n): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (buffer[0] == 'y' || buffer[0] == 'Y')
            do_update = 1;
        else
            do_update = 0;
    }
    enable_raw_mode(get_shell());
    return do_update;
}

void ask_for_update(char *prg_path, char *latest_version)
{
    int do_update = 0;

    (void)prg_path;
    (void)latest_version;
    do_update = ask_for_update_prompt();
    if (do_update == 1) {
        printf("Updating...\n");
    }
}
