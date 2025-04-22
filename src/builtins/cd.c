/*
** EPITECH PROJECT, 2025
** src/commands/cd
** File description:
** CD COMMAND
*/

#include "core/minishell.h"
#include "core/types.h"

/*
 * Get the current working dir using getcwd function, and add it to env var.
 * Check first if the env PWD is existing if no then strduping the key.
 * If yes then just passed the key as static name.
 */
static void refresh_pwd_variables(void)
{
    char *pwd = getcwd(NULL, 0);

    if (pwd == NULL)
        return;
    if (env_search("PWD") == NULL)
        add_env(my_strdup("PWD"), pwd);
    else
        add_env("PWD", pwd);
    get_shell()->vars->pwd_var = my_strdup(pwd);
}

/*
 * Set the variable in the shell OLDPWD to the variable PWD.
 * OLDPWD = PWD.
 */
static void set_oldpwd_to_pwd(void)
{
    char *current_folder = get_shell()->vars->pwd_var;

    if (current_folder == NULL)
        return;
    free_null_check(get_shell()->vars->oldpwd_var);
    get_shell()->vars->oldpwd_var = my_strdup(current_folder);
    free_null_check(current_folder);
}

/*
 * Intermediate function for changing directory.
 * This function is called in pretty all functions in this file.
 */
static int change_directory(char *path)
{
    int chdir_status = 0;

    if (is_file_exist(path) == false)
        return print_err("%s: No such file or directory.\n", path);
    if (is_file_type(path, S_IFDIR) == false)
        return print_err("%s: Not a directory.\n", path);
    if (access(path, X_OK) != 0)
        return print_err("%s: Permission denied.\n", path);
    set_oldpwd_to_pwd();
    chdir_status = chdir(path);
    if (!chdir_status)
        refresh_pwd_variables();
    return OK_OUTPUT;
}

/*
 * Get the HOME variable and go to this location.
 * If the variable is not found then just return 1.
 */
static exitcode_t change_pwd_to_home(void)
{
    char *home_folder = NULL;

    home_folder = get_shell()->vars->home_var;
    if (home_folder == NULL)
        return print_err("cd: No home directory.\n");
    return change_directory(home_folder);
}

/*
 * Get the home after the ~, for exemple cd ~toto will go to the home of toto.
 * This function use the /etc/passwd file to access the right home path.
 */
static exitcode_t change_pwd_to_home_custom(char *arg)
{
    char *username = &arg[1];
    passwd_t *passwd = get_passwd_from_user(username);
    int result_cd = 0;

    if (passwd == NULL)
        return print_err("Unknown user: %s.\n", username);
    result_cd = change_directory(passwd->home);
    free_passwd(passwd);
    return result_cd;
}

/*
 * Get the OLDPWD variable and go to this location.
 * If the variable is not found then just return 1 with showing an error.
 * This variable shouldn't be NULL since when you start the shell you set
 * the OLDPWD value to PWD.
 */
static int change_pwd_to_last(void)
{
    char *oldpwd_temp = get_shell()->vars->oldpwd_var;
    int result_cd = 0;

    if (oldpwd_temp == NULL)
        return print_err(": No such file or directory.\n");
    oldpwd_temp = my_strdup(get_shell()->vars->oldpwd_var);
    result_cd = change_directory(oldpwd_temp);
    free(oldpwd_temp);
    return result_cd;
}

/*
 * CD COMMAND - BUILT-IN
 * This command will change the working dir.
 * Possible cases :
 * - Nothing ==> Go to home home folder
 * - '~' ==> Go to home folder
 * - '~<username>' ==> Go to the home folder of the username
 * - '-' ==> Go to last folder
 * - Normal case such as '../' './folder' 'folder' '/tmp'
 * Errors cases :
 * - Not a directory
 * - Not a existing
 */
exitcode_t cd_command(char **argv)
{
    if (argv == NULL)
        return ERROR_OUTPUT;
    if (argv[1] != NULL && argv[2] != NULL)
        return print_err("cd: Too many arguments.\n");
    if (argv[1] == NULL || !my_strcmp(argv[1], "~"))
        return change_pwd_to_home();
    if (argv[1][0] == '~' && argv[1][1] != '\0' && argv[1][1] != '~')
        return change_pwd_to_home_custom(argv[1]);
    if (!my_strcmp(argv[1], "-"))
        return change_pwd_to_last();
    return change_directory(argv[1]);
}
