/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** Binary tree parse
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "parse.h"

static void pipe_s_print(shell_setup_t *setup, args_t *args, int *exit_status,
    int *pipes)
{
    int fd = pipes[0];
    int cpy = dup(STDIN_FILENO);
    int new = dup2(fd, STDIN_FILENO);

    close(pipes[1]);
    search_data(setup, args->next_r, exit_status);
    close(new);
    dup2(cpy, new);
}

void pipe_s(shell_setup_t *setup, args_t *args, int *exit_status)
{
    int pipes[2] = {0};
    int fd = 0;
    int new = 0;
    int child = 0;

    pipe(pipes);
    child = fork();
    if (child == 0) {
        fd = pipes[1];
        new = dup2(fd, STDOUT_FILENO);
        close(pipes[0]);
        search_data(setup, args->next_l, exit_status);
        close(new);
        exit(*exit_status);
    } else {
        wait(&child);
        *exit_status = WEXITSTATUS(child);
        pipe_s_print(setup, args, exit_status, pipes);
    }
}

void semi_column(shell_setup_t *setup, args_t *args, int *exit_status)
{
    if (args->next_l != NULL)
        search_data(setup, args->next_l, exit_status);
    if (args->next_r != NULL)
        search_data(setup, args->next_r, exit_status);
}

void operator_and(shell_setup_t *setup, args_t *args, int *exit_status)
{
    int left = 1;
    int right = 1;

    if (args->next_l != NULL)
        search_data(setup, args->next_l, &left);
    if (args->next_r != NULL && left == 0)
        search_data(setup, args->next_r, &right);
    if (args->next_r != NULL && left == 0)
        *exit_status = right;
    else if (args->next_l != NULL)
        *exit_status = left;
}

void operator_or(shell_setup_t *setup, args_t *args, int *exit_status)
{
    int left = 1;
    int right = 1;

    if (args->next_l != NULL)
        search_data(setup, args->next_l, &left);
    if (args->next_r != NULL && left != 0)
        search_data(setup, args->next_r, &right);
    if (args->next_l != NULL && left == 0)
        *exit_status = left;
    else if (args->next_r != NULL)
        *exit_status = right;
}

static char **add_color(char **tab)
{
    char **new_tab = NULL;
    size_t i = 0;

    for (; tab[i] != NULL; i += 1);
    new_tab = malloc(sizeof(char *) * (i + 2));
    *new_tab = *tab;
    new_tab[1] = malloc(sizeof("--color") + 1);
    *new_tab[1] = '\0';
    my_strcat(1, new_tab[1], "--color");
    for (i = 2; tab[i - 1] != NULL; i += 1)
        new_tab[i] = tab[i - 1];
    new_tab[i] = NULL;
    free(tab);
    return new_tab;
}

void search_data(shell_setup_t *setup, args_t *args, int *exit_status)
{
    char *paths = NULL;

    for (size_t i = 0; REDIR[i] != NULL; i += 1)
        if (same_str(REDIR[i], *args->value)) {
            (*RDRCT[i])(setup, args, exit_status);
            return;
        }
    if (!options(setup, args->value, *args->value, exit_status)) {
        if ((same_str(*args->value, "ls") || same_str(*args->value, "grep")) &&
            isatty(STDIN_FILENO))
            args->value = add_color(args->value);
        if (!same_str(*args->value, "exit")) {
            paths = search_path((*setup->env), "PATH");
            shell(setup, args->value, paths, exit_status);
            free(paths);
        }
    }
}
