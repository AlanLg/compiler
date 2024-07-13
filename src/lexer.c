#include "../include/buffer.h"
#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

const char *keywords[] = {"fonction", "rien", "entier", "si", "sinon", "tantque"};

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

char *lexer_getalphanum(buffer_t *buffer) {
    static char ident[256];
    size_t idx = 0;

    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    while (isalnum(c)) {
        ident[idx++] = c;
        c = buf_getchar(buffer);
    }
    buf_rollback(buffer, 1);
    buf_unlock(buffer);

    ident[idx] = '\0';

    return ident;
}

char *lexer_getalpha(buffer_t *buffer) {
    static char ident[256];
    size_t idx = 0;

    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    while (isalpha(c)) {
        ident[idx++] = c;
        c = buf_getchar(buffer);
    }
    buf_rollback(buffer, 1);
    buf_unlock(buffer);

    ident[idx] = '\0';

    if (is_alpha(ident)) {
        return ident;
    } else {
        buf_rollback(buffer, idx);
        return "";
    }
}

char lexer_getoperator(buffer_t *buffer) {
    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    if (is_operator(c)) {
        buf_unlock(buffer);
        return c;
    } else {
        buf_rollback(buffer, 1);
        buf_unlock(buffer);
        return '\0';
    }
}

char lexer_getchar(buffer_t *buffer) {
    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    if (is_punctuation(c)) {
        buf_unlock(buffer);
        return c;
    } else {
        buf_rollback(buffer, 1);
        buf_unlock(buffer);
        return '\0';
    }
}

char *lexer_getnumber(buffer_t *buffer) {
    static char number[256];
    size_t idx = 0;

    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    while (is_digit(c)) {
        number[idx++] = c;
        c = buf_getchar(buffer);
    }
    buf_rollback(buffer, 1);
    buf_unlock(buffer);

    number[idx] = '\0';

    return number;
}
