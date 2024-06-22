/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for linked_list
*/

#ifndef LINKED_LIST_H
    #define LINKED_LIST_H
    #include <stdbool.h>

typedef struct list_s {
    struct list_s *next;
    struct list_s *prev;
    char *arg;
    bool is_ok;
} list_t;

enum list {
    HEAD,
    NODE,
    HEAD_COMP,
    NODE_COMP,
    COMANDS,
};

size_t my_strlen(char *);
char *my_strcat(size_t _, char *, ...);
char *my_strncat(char *, char *, size_t _);
bool same_begin(char *, char *);

#endif
