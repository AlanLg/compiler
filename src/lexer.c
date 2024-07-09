#include "../include/lexer.h"

size_t length_alphanum_word(buffer_t *buffer) {
    size_t result = 0;
    char character = buf_getchar(buffer);

    while (isalnum(character)) {
        result++;
        character = buf_getchar(buffer);
    }
    buf_rollback(buffer, result);
    return result;
}

size_t length_number_word(buffer_t *buffer) {
    size_t result = 0;
    char character = buf_getchar(buffer);

    if (character == OPERATOR_MINUS) {
        character = buf_getchar(buffer);
        if (!isdigit(character)) {
            return 0;
        }
        result++;
    }

    while (isdigit(character)) {
        result++;
        character = buf_getchar(buffer);
    }
    buf_rollback(buffer, result);
    return result;
}

char *lexer_getalphanum(buffer_t *buffer) {
    buf_lock(buffer);
    size_t white_space = buf_skipblank(buffer);
    size_t length = length_alphanum_word(buffer);

    if (length == 0) {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        return NULL;
    }

    char *result = malloc(length + 1);
    if (!result) {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        return NULL;
    }

    for (size_t i = 0; i < length; i++) {
        result[i] = buf_getchar(buffer);
    }
    result[length] = '\0';

    buf_unlock(buffer);
    return result;
}

char *lexer_getalphanum_rollback(buffer_t *buffer) {
    buf_lock(buffer);
    size_t white_space = buf_skipblank(buffer);
    size_t length = length_alphanum_word(buffer);

    if (length == 0) {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        return NULL;
    }

    char *result = malloc(length + 1);
    if (!result) {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        return NULL;
    }

    for (size_t i = 0; i < length; i++) {
        result[i] = buf_getchar(buffer);
    }
    result[length] = '\0';

    buf_rollback(buffer, white_space + length);
    buf_unlock(buffer);
    return result;
}

char *lexer_getop(buffer_t *buffer) {
    buf_lock(buffer);
    size_t white_space = buf_skipblank(buffer);

    char *result = malloc(SIZE_MAX_OPERATOR + 1);
    if (!result) {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        return NULL;
    }

    char character = buf_getchar(buffer);
    int i = 0;
    if (character == OPERATOR_PLUS || character == OPERATOR_MINUS ||
        character == OPERATOR_MULT || character == OPERATOR_DIV ||
        character == OPERATOR_EQUAL || character == OPERATOR_SUP ||
        character == OPERATOR_INF) {
        result[i++] = character;
    } else {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        free(result);
        return NULL;
    }

    character = buf_getchar(buffer);
    if ((result[0] == OPERATOR_EQUAL && character == OPERATOR_EQUAL) ||
        (result[0] == OPERATOR_NEGATION && character == OPERATOR_EQUAL) ||
        (result[0] == OPERATOR_INF && character == OPERATOR_EQUAL) ||
        (result[0] == OPERATOR_SUP && character == OPERATOR_EQUAL)) {
        result[i++] = character;
    } else {
        buf_rollback(buffer, 1);
    }

    result[i] = '\0';
    buf_unlock(buffer);
    return result;
}

long lexer_getnumber(buffer_t *buffer) {
    buf_lock(buffer);
    size_t white_space = buf_skipblank(buffer);
    size_t length = length_number_word(buffer);

    if (length == 0) {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        return 0;
    }

    char *number_str = malloc(length + 1);
    if (!number_str) {
        buf_rollback(buffer, white_space);
        buf_unlock(buffer);
        return 0;
    }

    size_t i = 0;
    char character = buf_getchar(buffer);
    if (character == OPERATOR_MINUS) {
        number_str[i++] = character;
        character = buf_getchar(buffer);
    }

    while (isdigit(character)) {
        number_str[i++] = character;
        character = buf_getchar(buffer);
    }
    number_str[i] = '\0';

    buf_rollback(buffer, 1);
    long result = strtol(number_str, NULL, 10);
    free(number_str);
    buf_unlock(buffer);
    return result;
}
