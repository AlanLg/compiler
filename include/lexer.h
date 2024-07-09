#ifndef LEXER_H
#define LEXER_H

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "utils.h"

#define SIZE_MAX_OPERATOR 2

#define OPERATOR_PLUS '+'
#define OPERATOR_MINUS '-'
#define OPERATOR_MULT '*'
#define OPERATOR_DIV '/'
#define OPERATOR_EQUAL '='
#define OPERATOR_NEGATION '!'
#define OPERATOR_SUP '>'
#define OPERATOR_INF '<'

char *lexer_getalphanum(buffer_t *buffer);
char *lexer_getalphanum_rollback(buffer_t *buffer);
char *lexer_getop(buffer_t *buffer);
long lexer_getnumber(buffer_t *buffer);

#endif
