/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for shell main
*/

#ifndef MAIN_H
    #define MAIN_H

    #include <stdbool.h>
    #include "built_in.h"

    #define UNUSED __attribute__((unused))

size_t my_strlen(char *);
void get_it(shell_setup_t *, int *);
char **copy_tab(char **, bool, bool);
bool options(shell_setup_t *, char **, char *, int *);

#endif
