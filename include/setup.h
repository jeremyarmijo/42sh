/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for setup tree
*/

#ifndef SETUP_H
    #define SETUP_H

    #include <stdbool.h>
    #include "built_in.h"

typedef struct list_crea_s {
    char *car;
    int i;
    size_t n;
    size_t index;
} list_crea_t;

static void setup_l(char **, args_t **, int *, size_t);
static void setup_r(char **, args_t **, int *, size_t);
bool same_str(char *, char *);
void change_data(char ***, char *, char *, char *);

#endif
