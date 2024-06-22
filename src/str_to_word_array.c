/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** lib functions
*/

#include <stdlib.h>
#include <stdbool.h>
#include "word_array.h"

static size_t cpy_str(char **tab, char *str, size_t len)
{
    *tab = malloc(sizeof(char) * (len + 1));
    **tab = '\0';
    my_strncat(*tab, str, len);
    return len;
}

static size_t my_word_array(char **tab, char *str)
{
    size_t len = 0;
    bool continue_ = true;
    size_t i = 0;

    for (; SEP[i] != '\0' && SEP[i] != *str; i += 1);
    if (SEP[i] != '\0') {
        for (; str[len] == SEP[i]; len += 1);
        if (str[len] != ' ' && str[len] != '\t')
            continue_ = false;
    }
    for (char *temp = str; continue_ && temp[len] != '\0'; i += 1) {
        if (SEP[i] == '\0') {
            len += 1;
            i = 0;
        }
        if (temp[len] == SEP[i] || temp[len] == ' ' || temp[len] == '\t')
            break;
    }
    return cpy_str(tab, str, len);
}

static char *parse_string(char *str)
{
    size_t i = 0;

    if (*str == ' ' || *str == '\t')
        return str + 1;
    for (; SEP[i] != '\0' && SEP[i] != *str; i += 1);
    if (SEP[i] != '\0') {
        for (; *str == SEP[i]; str += 1);
        if (*str != ' ' && *str != '\t')
            return str;
    }
    for (i = 0; *str != '\0';) {
        if (*str == SEP[i] || *str == ' ' || *str == '\t')
            break;
        str += SEP[i] == '\0';
        i = (SEP[i] == '\0') ? (0) : (i + 1);
    }
    if (*str == ' ' || *str == '\t')
        for (; *str == ' ' || *str == '\t'; str += 1);
    return str;
}

char **my_str_to_word_array(char *str)
{
    size_t compteur = 1;
    char **tab = NULL;

    for (char *temp = str; *temp != '\0'; temp = parse_string(temp)) {
        if (*temp != ' ' && *temp != '\t')
            compteur += 1;
    }
    if (compteur == 1)
        return NULL;
    tab = malloc(sizeof(char *) * compteur);
    for (compteur = 0; *str != '\0'; str = parse_string(str)) {
        if (*str != ' ' && *str != '\t')
            my_word_array(&tab[compteur], str);
        compteur += (*str != ' ' && *str != '\t');
    }
    tab[compteur] = NULL;
    return tab;
}
