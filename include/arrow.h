/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for arraow
*/

#ifndef ARROW_H
    #define ARROW_H

    #include <stdbool.h>
    #include "built_in.h"
    #include "binary_tree.h"

typedef struct pipes_s {
    char *str;
    int *exit_status;
    int cpy;
    int new;
} pipes_t;

typedef struct list_s {
    struct list_s *next;
    char *str;
} list_t;

size_t my_strlen(char *);
bool same_str(char *, char *);
void search_data(shell_setup_t *, args_t *, int *);

#endif
