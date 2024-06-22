/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for history
*/

#ifndef HISTORY_H
    #define HISTORY_H
    #define ERR -1
    #define CTRL_D 4
    #define BACKSLASH 127
    #include <stdbool.h>

typedef struct list_s list_t;

bool is_sep(char);
list_t *ls_list(list_t *, char *, char *, size_t _);
void print_comp(list_t *, list_t *);
bool update_list(list_t ***, list_t *, char **, char);
void find_commands(list_t **, char *);
void cut_list(list_t **, char *);
char *supr_color(char *);

#endif
