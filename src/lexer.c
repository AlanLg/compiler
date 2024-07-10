#include "../include/buffer.h"
#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool is_alphanum(char c) {
    return isalnum(c);
}

bool is_alpha(char c) {
    return isalpha(c);
}

bool is_operator(char c) {
    return c == '=' || c == '<' || c == '>' || c == '&' || c == '|';
}

bool is_punctuation(char c) {
    return ispunct(c) && !is_operator(c);
}

bool is_digit(char c) {
    return isdigit(c);
}

char *lexer_getalpha(buffer_t *buffer) {
    static char ident[256];
    size_t idx = 0;

    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    while (is_alphanum(c)) {
        ident[idx++] = c;
        c = buf_getchar(buffer);
    }
    buf_rollback(buffer, 1);
    buf_unlock(buffer);

    ident[idx] = '\0';

    return ident;
}

char *lexer_getalphanum(buffer_t *buffer) {
    static char ident[256];
    size_t idx = 0;

    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    while (is_alpha(c)) {
        ident[idx++] = c;
        c = buf_getchar(buffer);
    }
    buf_rollback(buffer, 1); // Rewind last non-alphabetic character
    buf_unlock(buffer);

    ident[idx] = '\0';

    return ident;
}

char *lexer_getop(buffer_t *buffer) {
    buf_skipblank(buffer);
    buf_lock(buffer);
    char *result = malloc(3);
    if (!result) {
        buf_unlock(buffer);
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
        buf_rollback(buffer, 1);
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

char lexer_getchar(buffer_t *buffer) {
    buf_skipblank(buffer);
    buf_lock(buffer);

    char c = buf_getchar(buffer);
    if (is_punctuation(c)) {
        buf_unlock(buffer);
        return c;
    } else {
        buf_rollback(buffer, 1); // Rewind last non-punctuation character
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
    buf_rollback(buffer, 1); // Rewind last non-digit character
    buf_unlock(buffer);

    number[idx] = '\0';

    return number;
}
