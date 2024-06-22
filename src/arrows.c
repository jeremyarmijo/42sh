/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** Fisrt shell
*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "arrow.h"

static void print_tree(args_t *args)
{
    if (args->next_l != NULL)
        print_tree(args->next_l);
    printf(" %s", *(args->value));
    if (args->next_r != NULL)
        print_tree(args->next_r);
}

static void print_background(int number, args_t *args, int *exit_status)
{
    if (*exit_status == 0) {
        printf("[%d]  + Done\t\t\t    ", number);
        print_tree(args->next_l);
        printf("\n");
        return;
    }
    if (*exit_status == 139) {
        printf("[%d]  + Segmentation fault\t    ", number);
        print_tree(args);
        printf(" (core dumped)\n");
    } else if (*exit_status == 136) {
        printf("[%d]  + Floating exception\t    ", number);
        print_tree(args);
        printf(" (core dumped)\n");
    } else {
        printf("[%d]  + Exit %d\t\t\t    ", number, *exit_status);
        print_tree(args->next_l);
        printf("\n");
    }
}

static void run_background(shell_setup_t *setup, args_t *args,
    int *exit_status, int number)
{
    pid_t pgid = 0;
    pid_t pid = fork();

    if (pid == 0){
        search_data(setup, args->next_l, exit_status);
        print_background(number, args, exit_status);
        exit(*exit_status);
    }
    if (pid > 0){
        pgid = getpgid(pid);
        tcsetpgrp(STDIN_FILENO, pgid);
        kill(-pgid, SIGCONT);
        waitpid(pid, NULL, WNOHANG);
        if (args->next_r != NULL)
            search_data(setup, args->next_r, exit_status);
    }
}

void background(shell_setup_t *setup, args_t *args, int *exit_status)
{
    static int number = 1;

    printf("[%d] %d\n", number, getpid());
    run_background(setup, args, exit_status, number);
    if (number > 9){
        number = 1;
    } else {
        ++number;
    }
}

static void arrow_2l_print(list_t *head, int *pipes, int *exit_status)
{
    list_t *node = NULL;
    int fd = pipes[1];
    int new = dup2(fd, STDOUT_FILENO);

    close(pipes[0]);
    for (list_t *node_t = head; node_t->next != NULL; node_t = node) {
        node = node_t->next;
        write(STDOUT_FILENO, node_t->str, my_strlen(node_t->str));
        write(STDOUT_FILENO, "\n", 1);
        free(node_t->str);
        free(node_t);
    }
    free(node->str);
    free(node);
    close(new);
    exit(*exit_status);
}

static void arrow_2l_suite(shell_setup_t *setup,
    args_t *args, int *exit_status, list_t *head)
{
    int pipes[2] = {0};
    int child = 0;
    int fd = 0;
    int cpy = dup(STDIN_FILENO);
    int new = 0;

    pipe(pipes);
    child = fork();
    if (child == 0)
        arrow_2l_print(head, pipes, exit_status);
    else {
        wait(&child);
        *exit_status = WEXITSTATUS(child);
        fd = pipes[0];
        new = dup2(fd, STDIN_FILENO);
        close(pipes[1]);
        search_data(setup, args->next_l, exit_status);
        close(new);
        dup2(cpy, new);
    }
}

void arrow_2l(shell_setup_t *setup, args_t *args, int *exit_status)
{
    list_t *head = malloc(sizeof(list_t));
    list_t *node = head;
    size_t len = 0;

    head->next = NULL;
    head->str = NULL;
    for (ssize_t i = getline(&node->str, &len, stdin); i != -1;
        i = getline(&node->str, &len, stdin)) {
        if (i == -1)
            return;
        node->str[i - 1] = '\0';
        if (same_str(node->str, *args->next_r->value))
            break;
        node->next = malloc(sizeof(list_t));
        node = node->next;
        node->str = NULL;
        node->next = NULL;
    }
    arrow_2l_suite(setup, args, exit_status, head);
}

void arrow_l(shell_setup_t *setup, args_t *args, int *exit_status)
{
    int fd = open(*args->next_r->value, O_RDONLY);
    int cpy = dup(STDIN_FILENO);
    int new = 0;

    if (fd == -1) {
        perror(*args->next_r->value);
        return;
    }
    new = dup2(fd, STDIN_FILENO);
    search_data(setup, args->next_l, exit_status);
    close(new);
    dup2(cpy, new);
}

void arrow_2r(shell_setup_t *setup, args_t *args, int *exit_status)
{
    int fd = open(*args->next_r->value, O_CREAT | O_APPEND | O_WRONLY, 0644);
    int cpy = dup(STDOUT_FILENO);
    int new = dup2(fd, STDOUT_FILENO);

    search_data(setup, args->next_l, exit_status);
    close(new);
    dup2(cpy, new);
}

void arrow_r(shell_setup_t *setup, args_t *args, int *exit_status)
{
    int fd = open(*args->next_r->value, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    int cpy = dup(STDOUT_FILENO);
    int new = dup2(fd, STDOUT_FILENO);

    search_data(setup, args->next_l, exit_status);
    close(new);
    dup2(cpy, new);
}
