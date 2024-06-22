/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** .h for shell
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tester.h"

list_t *free_list(list_t *head)
{
    for (list_t *temp = NULL; head != NULL; head = temp) {
        temp = head->next;
        free(head->arg);
        free(head);
    }
    return NULL;
}

void rm_list(list_t **head)
{
    list_t *temp = (*head)->next;

    free((*head)->arg);
    free(*head);
    *head = temp;
    if (*head != NULL)
        (*head)->prev = NULL;
}

char *search_path(char **env, char *str)
{
    char *paths = NULL;
    size_t i = 0;

    for (i = 0; env[i] != NULL && !my_str(env[i], str); i += 1);
    if (env[i] != NULL) {
        paths = malloc(sizeof(char) * (my_strlen(env[i]) - 4));
        *paths = '\0';
        my_strcat(1, paths, env[i] + 5);
    } else if (same_str("PATH", str)){
        paths = malloc(sizeof(char) * 9);
        *paths = '\0';
        my_strcat(1, paths, "/usr/bin");
    }
    return paths;
}

void get_command(list_t **head, char *UNUSED(path), bool *UNUSED(end))
{
    list_t *new_node = malloc(sizeof(list_t));

    printf("%s\n", COMMANDS[i]);
    new_node->arg = malloc(my_strlen((char *)COMMANDS[i]) + 1);
    *new_node->arg = '\0';
    my_strcat(1, new_node->arg, COMMANDS[i]);
    new_node->prev = NULL;
    new_node->next = *head;
    if (*head != NULL)
        (*head)->prev = new_node;
    *head = new_node;
    i += 1;
}
