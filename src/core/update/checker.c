/*
** EPITECH PROJECT, 2025
** 42sh-ci
** File description:
** get_latest
*/

#include "build_infos.h"

#include "core/update.h"

/**
 * @brief Check for updates of the shell.
 *
 * @param prg_path The path to the shell executable.
 */
void check_for_updates(IN char *prg_path)
{
    char *latest_version = get_latest_release();
    char *current_ver = VERSION;

    if (latest_version == NULL || !is_valid_version(latest_version)) {
        fprintf(stderr, "Failed to fetch the latest version information.\n");
        return;
    }
    if (compare_versions(VERSION, latest_version) == 2) {
        printf("A new version of 42sh is available: %s\n", latest_version);
        if (ALLOW_AUTO_UPDATE)
            ask_for_update(prg_path, latest_version);
    } else {
        printf("You are using the latest version of 42sh: %s\n", current_ver);
    }
    free(latest_version);
}
