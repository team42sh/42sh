/*
** EPITECH PROJECT, 2025
** 42sh-ci
** File description:
** update
*/

#ifndef UPDATE_H_
    #define UPDATE_H_

    #include "core/minishell.h"
    #include "update_infos.h"


// checker.c
void check_for_updates(char *prg_path);

// version_helper.c
char *get_latest_release(void);
int is_valid_version(char *version);
int compare_versions(char *version1, char *version2);

// update_manager.c
void ask_for_update(char *prg_path, char *latest_version);

#endif /* !UPDATE_H_ */
