/*
** EPITECH PROJECT, 2025
** 42sh-ci
** File description:
** get_latest
*/

#include "build_infos.h"

#include "core/update.h"

void check_for_updates(void)
{
    char *latest_version = get_latest_release();
    char *current_ver = VERSION;

    if (latest_version == NULL) {
        fprintf(stderr, "Failed to fetch the latest version information.\n");
        return;
    }
    if (compare_versions(VERSION, latest_version) == 1) {
        printf("A new version of 42sh is available: %s\n", latest_version);
    } else {
        printf("You are using the latest version of 42sh: %s\n", current_ver);
    }
    free(latest_version);
}