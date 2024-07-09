#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdlib.h>
#include "buffer.h"

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

size_t length_alphanum_word(buffer_t *buffer);

size_t length_number_word(buffer_t *buffer);
char *lexer_getalphanum(buffer_t *buffer);
char *lexer_getop(buffer_t *buffer);
long lexer_getnumber(buffer_t *buffer);

#endif
