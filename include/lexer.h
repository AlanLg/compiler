#ifndef LEXER_H
#define LEXER_H

#include "../include/buffer.h"

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

bool is_operator(char c);
bool is_punctuation(char c);

bool is_alpha(const char *str);
bool is_digit(char c);

char *lexer_getalphanum(buffer_t *buffer);

char *lexer_getalpha(buffer_t *buffer);

char *lexer_getoperator(buffer_t *buffer);

char lexer_getchar(buffer_t *buffer);

char *lexer_getnumber(buffer_t *buffer);

#endif // LEXER_H
