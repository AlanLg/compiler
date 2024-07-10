#ifndef LEXER_H
#define LEXER_H

#include "../include/buffer.h"
// Define operator characters
#define OPERATOR_PLUS '+'
#define OPERATOR_MINUS '-'
#define OPERATOR_MULT '*'
#define OPERATOR_DIV '/'
#define OPERATOR_EQUAL '='
#define OPERATOR_NEGATION '!'
#define OPERATOR_SUP '>'
#define OPERATOR_INF '<'
#define OPERATOR_AND '&'
#define OPERATOR_OR '|'

bool is_alphanum(char c);

bool is_alpha(char c);

bool is_operator(char c);

bool is_punctuation(char c);

bool is_digit(char c);

char *lexer_getalpha(buffer_t *buffer);

char *lexer_getalphanum(buffer_t *buffer);

char *lexer_getop(buffer_t *buffer);

char lexer_getchar(buffer_t *buffer);

char *lexer_getnumber(buffer_t *buffer);

#endif // LEXER_H
