/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for completion
*/

#ifndef COMPLETION_H
    #define COMPLETION_H
    #include <stdbool.h>

typedef struct list_s list_t;

bool is_sep(char);
bool is_dir(char *);
bool find_str(char *, char);
bool same_str(char *, char *);
char *supr_color(char *);
list_t *free_list(list_t *);
char **my_str_to_word_array(char *);
void free_tab(char **);
void new_tab(list_t **, list_t *, char **, char *);

#endif
