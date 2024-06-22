/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for func comp
*/

#ifndef FUNC_COMP_H
    #define FUNC_COMP_H
    #include <stdbool.h>

typedef struct list_s list_t;

bool is_sep(char);
bool is_dir(char *);
bool find_str(char *, char);
bool my_str(char *, char *);
bool same_str(char *, char *);
void is_select(list_t *, char *, char);
list_t *free_list(list_t *);
char **my_str_to_word_array(char *);
void free_tab(char **);
void path_tab(list_t **, char **, char **, char *);

#endif
