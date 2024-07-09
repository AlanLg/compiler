#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "symbols.h"

void parse(buffer_t *buffer);
ast_t analyse_function(buffer_t *buffer);

#endif
