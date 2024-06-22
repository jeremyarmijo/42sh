/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for tester
*/

#ifndef TESTER_H
    #define TESTER_H
    #define UNUSED __attribute__((unused))

typedef struct list_s {
    struct list_s *next;
    struct list_s *prev;
    char *arg;
} list_t;

bool my_str(char *, char *);
char *my_strcat(size_t _, char *, ...);
bool same_str(char *, char *);
size_t my_strlen(char *);

static size_t i = 0;
static const char *COMMANDS[] = {"ls", "env > file", "grep PATH file",
    "rm file", "alias l ls", "l", "alias", "cd .. || ls", "cd - && cd include",
    "cd file && cd ..", "cd ..; cat Makefile | head", "exit", NULL};

#endif
