/*
** EPITECH PROJECT, 2025
** 42sh-ci
** File description:
** update_manager
*/

#include "build_infos.h"

#include "core/update.h"

/**
 * @brief Download the specified version of the shell.
 *
 * @param version The version to download.
 * @return The path to the downloaded file, or NULL on failure.
 */
static char *download_version(IN char *version)
{
    char local_path[256];
    char download_url[512];
    char download_cmd[1024];
    int cmd_ret = 0;

    snprintf(local_path, sizeof(local_path), "/tmp/42sh-%s", version);
    snprintf(download_url, sizeof(download_url), "%s/%s/%s.%s",
        DOWNLOAD_URL_BASE, version, DOWNLOAD_PRG_BASE_NAME, OS_VER);
    snprintf(download_cmd, sizeof(download_cmd), "curl -# -fLo %s %s",
        local_path, download_url);
    cmd_ret = system(download_cmd);
    if (cmd_ret != 0) {
        fprintf(stderr, "Error: Failed to download %s\n", version);
        return NULL;
    }
    return strdup(local_path);
}

/**
 * @brief Replace the current shell with the downloaded version.
 *
 * @param prg_path The path to the current shell executable.
 * @param version_path The path to the downloaded version.
 * @return 0 on success, -1 on failure.
 */
static int replace_42sh_with_update(IN char *prg_path, IN char *version_path)
{
    char command[512];
    int cmd_ret = 0;

    snprintf(command, sizeof(command), "mv %s %s && chmod +x %s", version_path,
        prg_path, prg_path);
    cmd_ret = system(command);
    if (cmd_ret != 0) {
        fprintf(stderr, "Error: Failed to replace %s with %s\n",
            prg_path, version_path);
        return -1;
    }
    return 0;
}

/**
 * @brief Run the update process.
 *
 * @param prg_path The path to the current shell executable.
 * @param latest_version The latest version to update to.
 */
static void run_update(IN char *prg_path, IN char *latest_version)
{
    char *version_path = NULL;
    char *launch_cmd[] = {prg_path, NULL};

    version_path = download_version(latest_version);
    if (version_path == NULL)
        return;
    replace_42sh_with_update(prg_path, version_path);
    free(version_path);
    printf("Update completed. The shell will restart in 3 seconds...\n");
    system("sleep 3 && clear");
    execv(launch_cmd[0], launch_cmd);
}

/**
 * @brief Ask the user if they want to update the shell.
 *
 * @return 1 if the user wants to update, 0 otherwise.
 */
static int ask_for_update_prompt(void)
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

/**
 * @brief Ask the user if they want to update the shell and perform the update.
 *
 * @param prg_path The path to the current shell executable.
 * @param latest_version The latest version to update to.
 */
void ask_for_update(IN char *prg_path, IN char *latest_version)
{
    int do_update = 0;

    do_update = ask_for_update_prompt();
    if (do_update == 1) {
        printf("Updating...\n");
        run_update(prg_path, latest_version);
    }
}
