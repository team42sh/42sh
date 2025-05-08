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
void check_for_updates(void);

// version_helper.c
char *get_latest_release(void);
int is_valid_version(char *version);
int compare_versions(char *version1, char *version2);

#endif /* !UPDATE_H_ */
