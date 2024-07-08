#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Define your buffer_t structure and related functions
typedef struct buffer_t buffer_t;

// Function prototypes for buffer operations
void buf_lock(buffer_t *buffer);
void buf_unlock(buffer_t *buffer);
char buf_getchar(buffer_t *buffer);
void buf_rollback(buffer_t *buffer, size_t count);
void buf_rollback_and_unlock(buffer_t *buffer, size_t count);
size_t buf_skipblank(buffer_t *buffer);

// Constants for operator symbols
#define OPERATOR_MINUS '-'
#define OPERATOR_PLUS '+'
#define OPERATOR_MULT '*'
#define OPERATOR_DIV '/'
#define OPERATOR_EQUAL '='
#define OPERATOR_SUP '>'
#define OPERATOR_INF '<'
#define OPERATOR_NEGATION '!'
#define SIZE_MAX_OPERATOR 2

// Function prototypes for the lexer functions
size_t length_alphanum_word(buffer_t *buffer);
size_t length_number_word(buffer_t *buffer);
char *lexer_getalphanum(buffer_t *buffer);
char *lexer_getalphanum_rollback(buffer_t *buffer);
char *lexer_getop(buffer_t *buffer);
long lexer_getnumber(buffer_t *buffer);

#endif // LEXER_H
