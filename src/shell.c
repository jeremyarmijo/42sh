/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** Fisrt shell
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include "built_in.h"
#include "shell.h"

char **copy_tab(char **tab, bool box_to_add, bool to_free)
{
    char **new_tab = NULL;
    size_t i = 0;

    for (; tab[i] != NULL; i += 1);
    new_tab = malloc(sizeof(char *) * (i + 1 + box_to_add));
    for (i = 0; tab[i] != NULL; i += 1) {
        new_tab[i] = malloc(sizeof(char) * (my_strlen(tab[i]) + 1));
        *new_tab[i] = '\0';
        my_strcat(1, new_tab[i], tab[i]);
        if (to_free)
            free(tab[i]);
    }
    new_tab[i] = NULL;
    if (to_free)
        free(tab);
    return new_tab;
}

bool options(shell_setup_t *setup, char **args, char *str, int *exit_status)
{
    if (same_str(str, "env") || (same_str(str, "setenv") && args[1] == NULL))
        for (int i = 0; setup->env[0][i] != NULL; i += 1) {
            write(1, (*setup->env)[i], my_strlen((*setup->env)[i]));
            write(1, "\n", 1);
        }
    if (same_str(str, "unsetenv")) {
        *exit_status = args[1] == NULL;
        if (args[1] == NULL)
            write(2, "unsetenv: Too few arguments.\n", 29);
        for (size_t i = 1; args[i] != NULL; i += 1)
            my_env(setup, args, i);
    }
    if (built_in(exit_status, str, setup, args))
        return true;
    return false;
}

static size_t fk_it(shell_setup_t *setup, char **args, char *path, int child)
{
    if (child == -2) {
        write(2, *args, my_strlen(*args));
        write(2, ": Command not found.\n", 21);
        return 1;
    } else if (child == -1)
        exit(84);
    if (child == 0) {
        execve(path, args, (*setup->env));
        if (errno == ENOEXEC) {
            write(2, path, my_strlen(path));
            write(2, ": Exec format error. Wrong Architecture.\n", 41);
        } else
            perror(*args);
        exit(1);
    } else {
        wait(&child);
        return error_value(child);
    }
}

void shell(shell_setup_t *setup, char **arg, char *paths, int *exit_status)
{
    char *temp = NULL;
    int i = -2;

    if (find_str(arg[0], '/')) {
        *exit_status = fk_it(setup, arg, *arg,
            (access(*arg, X_OK) == 0) ? (fork()) : (i));
        return;
    }
    temp = malloc(sizeof(char) * (my_strlen(paths) + my_strlen(*arg) + 2));
    for (i = 1; *paths != '\0' && i > 0; i += 1)
        if (paths[i] == ':' || paths[i] == '\0') {
            *temp = '\0';
            my_strncat(temp, paths, i);
            paths += i + (*(paths + i) != '\0');
            my_strcat(2, temp, "/", *arg);
            i = -1 * (access(temp, X_OK) == 0);
        }
    *exit_status = fk_it(setup, arg, temp, (i == 0) ? (fork()) : (-2));
    free(temp);
}

void free_alias_list(shell_setup_t *setup)
{
    alias_t *prev = setup->alias_database;
    alias_t *head = setup->alias_database;

    while (head) {
        prev = head;
        head = head->next;
        free(prev->key);
        free(prev->value);
        free(prev);
    }
    free(setup);
}

static bool execute_command(shell_setup_t *setup, list_t **head, args_t *args,
    int *exit_status)
{
    char **tab = my_str_to_word_array((*head)->arg);

    verif_alias(setup, &tab);
    if (tab != NULL) {
        args = setup_args(tab);
        if (!error_handling(args, exit_status))
            search_data(setup, args, exit_status);
        free_tree(args);
    }
    for (size_t i = 0; tab != NULL && tab[i] != NULL; i += 1)
        if (same_str(tab[i], "exit")) {
            free_tab(tab);
            return true;
        }
    if (tab != NULL)
        free_tab(tab);
    else
        rm_list(head);
    return false;
}

void get_it(shell_setup_t *setup, int *exit_status)
{
    list_t *head = NULL;
    char pwd[4096] = {0};
    char branch[4096] = {0};
    bool exit[2] = {false};

    while (!exit[0] && !exit[1]) {
        if (isatty(0)) {
            *pwd = '\0';
            *branch = '\0';
            printf("\033[1;3%cm➜  \033[36m%s %s\033[35m✗ "
                "\033[0m\0337", (*exit_status == 0) + '1',
                get_pwd((*setup->env), pwd), get_branch(branch));
        }
        get_command(&head, search_path(*setup->env, "PATH"), &exit[0]);
        exit[1] = execute_command(setup, &head, NULL, exit_status);
        if (exit[0] && isatty(STDIN_FILENO))
            write(1, "exit\n", 5);
    }
    free_list(head);
    free_alias_list(setup);
}
