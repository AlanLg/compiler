#include "../include/buffer.h"
#include "../include/lexer.h"
#include "../include/errors.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

const char *keywords[] = {"fonction", "rien", "entier", "bool", "si", "sinon", "tantque"};

bool is_operator(char c) {
    return c == OPERATOR_PLUS || c == OPERATOR_MINUS || c == OPERATOR_MULT ||
           c == OPERATOR_DIV || c == OPERATOR_EQUAL || c == OPERATOR_NEGATION ||
           c == OPERATOR_SUP || c == OPERATOR_INF || c == OPERATOR_AND || c == OPERATOR_OR;
}

bool is_punctuation(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == ',' ||
           c == ';' || c == ':';
}

bool is_alpha(const char *str) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_digit(char c) {
    return isdigit(c);
}

char *lexer_getalphanum(buffer_t *buffer, error_list *errors) {
    char ident[256];
    size_t idx = 0;

    buf_skipblank(buffer);

    char c = buf_getchar(buffer);
    buf_lock(buffer);
    while (isalnum(c)) {
        ident[idx++] = c;
        c = buf_getchar(buffer);
    }
    buf_rollback_and_unlock(buffer, 1);
    ident[idx] = '\0';

    char *result = malloc(idx + 1);
    if (result != NULL) {
        strcpy(result, ident);
    } else {
        add_error(errors, "Erreur : Allocation de memoire echouee dans lexer_getalphanum.");
    }

    return result;
}

char *lexer_getalpha(buffer_t *buffer, error_list *errors) {
    char ident[256];
    size_t idx = 0;

    buf_skipblank(buffer);

    char c = buf_getchar(buffer);
    while (isalpha(c)) {
        ident[idx++] = c;
        c = buf_getchar(buffer);
    }

    ident[idx] = '\0';

    if (is_alpha(ident)) {
        char *result = malloc(idx + 1);
        if (result != NULL) {
            strcpy(result, ident);
        } else {
            add_error(errors, "Erreur : Allocation de memoire echouee dans lexer_getalpha.");
        }
        return result;
    } else {
        add_error(errors, "Erreur : Identifiant non reconnu dans lexer_getalpha.");
        return "";
    }
}

char *lexer_getoperator(buffer_t *buffer, error_list *errors) {
    buf_skipblank(buffer);
    buf_lock(buffer);
    char *result = malloc(3);
    if (!result) {
        buf_unlock(buffer);
        add_error(errors, "Erreur : Allocation de memoire echouee dans lexer_getoperator.");
        return NULL;
    }

    char character = buf_getchar(buffer);
    int i = 0;
    if (character == OPERATOR_PLUS || character == OPERATOR_MINUS ||
        character == OPERATOR_MULT || character == OPERATOR_DIV ||
        character == OPERATOR_EQUAL || character == OPERATOR_SUP ||
        character == OPERATOR_INF || character == OPERATOR_NEGATION ||
        character == OPERATOR_AND || character == OPERATOR_OR) {
        result[i++] = character;
    } else {
        buf_unlock(buffer);
        free(result);
        return NULL;
    }

    character = buf_getchar(buffer);
    if ((result[0] == OPERATOR_EQUAL && character == OPERATOR_EQUAL) ||
        (result[0] == OPERATOR_NEGATION && character == OPERATOR_EQUAL) ||
        (result[0] == OPERATOR_INF && character == OPERATOR_EQUAL) ||
        (result[0] == OPERATOR_SUP && character == OPERATOR_EQUAL) ||
        (result[0] == OPERATOR_PLUS && character == OPERATOR_PLUS) ||
        (result[0] == OPERATOR_MINUS && character == OPERATOR_MINUS) ||
        (result[0] == OPERATOR_AND && character == OPERATOR_AND) ||
        (result[0] == OPERATOR_OR && character == OPERATOR_OR)) {
        result[i++] = character;
    } else {
        buf_rollback(buffer, 1);
    }

    result[i] = '\0';
    buf_unlock(buffer);
    return result;
}

char lexer_getchar(buffer_t *buffer, error_list *errors) {
    buf_skipblank(buffer);

    const char c = buf_getchar(buffer);
    printf("found char %c\n", c);
    if (is_punctuation(c)) {
        return c;
    } else {
        return '\0';
    }
}

char *lexer_getnumber(buffer_t *buffer, error_list *errors) {
    static char number[256];
    size_t idx = 0;

    buf_skipblank(buffer);

    char c = buf_getchar(buffer);
    buf_lock(buffer);
    while (is_digit(c)) {
        number[idx++] = c;
        c = buf_getchar(buffer);
    }
    buf_rollback_and_unlock(buffer, 1);
    number[idx] = '\0';

    char *result = malloc((idx + 1) * sizeof(char));
    if (result != NULL) {
        strcpy(result, number);
    } else {
        add_error(errors, "Erreur : Allocation de memoire echouee dans lexer_getnumber.");
    }

    return result;
}
