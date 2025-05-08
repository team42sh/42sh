/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** version_helper
*/

#include "core/update.h"
#include <regex.h>

const char *semver_regex = "^v[0-9]+\\.[0-9]+\\.[0-9]+(-alpha|-beta)?$";

char *get_latest_release(void)
{
    FILE *fp;
    char buffer[128] = {0};
    char *version = NULL;
    char command[256];
    size_t len = 0;

    snprintf(command, sizeof(command), "curl -s %s | jq -r '.tag_name'",
        UPDATE_VERSION_URL);
    fp = popen(command, "r");
    if (fp == NULL)
        return strdup("UNKWN");
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        version = strdup(buffer);
    } else
        version = strdup("UNKWN");
    pclose(fp);
    return version;
}

int is_valid_version(char *version)
{
    regex_t regex;
    int result;

    if (version == NULL)
        return 0;
    if (regcomp(&regex, semver_regex, REG_EXTENDED) != 0)
        return 0;
    result = regexec(&regex, version, 0, NULL, 0);
    regfree(&regex);
    return (result == 0) ? 1 : 0;
}

int compare_versions(char *version1, char *version2)
{
    int major1;
    int minor1;
    int patch1;
    int major2;
    int minor2;
    int patch2;

    if (!is_valid_version(version1) || !is_valid_version(version2))
        return 0;
    sscanf(version1, "v%d.%d.%d", &major1, &minor1, &patch1);
    sscanf(version2, "v%d.%d.%d", &major2, &minor2, &patch2);
    if (major1 != major2)
        return (major1 > major2) ? 1 : 2;
    if (minor1 != minor2)
        return (minor1 > minor2) ? 1 : 2;
    if (patch1 != patch2)
        return (patch1 > patch2) ? 1 : 2;
    return 0;
}
