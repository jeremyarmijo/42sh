/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** main and error functions
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "main.h"
#include "built_in.h"

static void output_child(int *pipes)
{
    char buff[2] = {0};
    char last = '\0';

    close(pipes[1]);
    for (read(pipes[0], buff, 1); *buff != '\n'; read(pipes[0], buff, 1)) {
        if (!(last == '\0' && *buff == '-'))
            write(STDERR_FILENO, buff, 1);
        last = *buff;
        *buff = '\0';
    }
    if (last != '.')
        write(STDERR_FILENO, ".", 1);
    write(STDERR_FILENO, "\n", 1);
    close(pipes[0]);
    exit(0);
}

void change_output(char *str)
{
    int fd = 0;
    int new = 0;
    int cpy = dup(STDERR_FILENO);
    int child = 0;
    int pipes[2] = {0};

    pipe(pipes);
    child = fork();
    if (child == 0) {
        output_child(pipes);
    } else {
        fd = pipes[1];
        new = dup2(fd, STDERR_FILENO);
        close(pipes[0]);
        perror(str);
        close(new);
        dup2(cpy, new);
        wait(&child);
    }
}

size_t error_value(size_t error)
{
    char *tab[] = {"Abort", "Segmentation fault", "Floating exception", NULL};
    size_t sig[] = {SIGABRT, SIGSEGV, SIGFPE};
    size_t exit[] = {134, 139, 136};
    size_t i = 0;

    for (; tab[i] != NULL; i += 1) {
        if (sig[i] == WTERMSIG(error)) {
            write(2, tab[i], my_strlen(tab[i]));
            break;
        }
    }
    if (WCOREDUMP(error))
        write(2, " (core dumped)", 14);
    if (tab[i] != NULL) {
        write(2, "\n", 1);
        return exit[i];
    }
    return WEXITSTATUS(error);
}

int main(int UNUSED(ac), char UNUSED(**av), char **env)
{
    char **tab = copy_tab(env, false, false);
    int exit_status = 0;
    char *one = "unsetenv";
    char *two = "OLDPWD";
    char *args[3] = {one, two, NULL};
    shell_setup_t *setup_42sh = init_setup_shell(&tab);

    options(setup_42sh, args, one, &exit_status);
    get_it(setup_42sh, &exit_status);
    for (size_t i = 0; tab[i] != NULL; i += 1)
        free(tab[i]);
    free(tab);
    return exit_status;
}
