/*
** EPITECH PROJECT, 2024
** built in
** File description:
** fonction
*/
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "stddef.h"
#include "built_in.h"
#include "shell.h"

void change_data(char ***tab, char *find, char *str, char *setenv)
{
    size_t i = 0;

    for (; tab[0][i] != NULL && !my_str(tab[0][i], find); i += 1);
    if ((*tab)[i] == NULL) {
        *tab = copy_tab(*tab, true, true);
        (*tab)[i + 1] = NULL;
    } else
        free((*tab)[i]);
    (*tab)[i] = malloc(sizeof(char) * (my_strlen(find) + my_strlen(str) + 2));
    *(*tab)[i] = '\0';
    my_strcat(3, (*tab)[i], find, setenv, str);
}

bool my_env(shell_setup_t *setup, char **av, size_t j)
{
    int i = 0;

    if (j == 1) {
        if (!is_alpha(av[1]))
            return false;
        if (av[2] != NULL && av[3] != NULL) {
            write(2, "setenv: Too many arguments.\n", 28);
            return false;
        }
        change_data(ENV, av[1], (av[2] == NULL) ? ("") : (av[2]), "=");
    } else {
        for (i = 0; ENV[0][i] != NULL
            && !my_str(ENV[0][i], av[j]); i += 1);
        if ((*ENV)[i] == NULL)
            return true;
        free((*ENV)[i]);
        for (; ENV[0][i] != NULL; i += 1)
            (*ENV)[i] = (*ENV)[i + 1];
    }
    return true;
}

static bool my_cd(shell_setup_t *setup, char **args, char *path, size_t i)
{
    char pwd[2][4096] = {0};

    if (args[1] != NULL && args[2] != NULL) {
        write(2, "cd: Too many arguments.\n", 24);
        return false;
    }
    for (i = 0; ENV[0][i] != NULL && !my_str(ENV[0][i], "HOME"); i += 1);
    if ((args[1] == NULL || same_str(args[1], "~")) && (*ENV)[i] != NULL)
        path = (*ENV)[i] + 5;
    for (i = 0; ENV[0][i] != NULL && !my_str(ENV[0][i], "OLDPWD"); i += 1);
    if (args[1] != NULL && same_str(args[1], "-") && (*ENV)[i] != NULL)
        path = (*ENV)[i] + 7;
    if (getcwd(pwd[0], 4096) == NULL || chdir(path) == -1) {
        change_output(args[1]);
        return false;
    } else {
        change_data(ENV, "OLDPWD", pwd[0], "=");
        change_data(ENV, "PWD", getcwd(pwd[1], 4096), "=");
    }
    return true;
}

bool built_in(int *exit_status, char *str, shell_setup_t *setup, char **args)
{
    if (same_str(str, "setenv") && args[1] != NULL)
        *exit_status = !my_env(setup, args, 1);
    if (same_str(str, "cd"))
        *exit_status = !my_cd(setup, args, args[1], 0);
    if (same_str(str, "alias"))
        *exit_status = alias(args, setup);
    if (same_str(str, "env") || same_str(str, "unsetenv")
        || same_str(str, "setenv") || same_str(str, "cd")
        || same_str(str, "alias"))
        return true;
    return false;
}
