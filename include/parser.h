#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>

void parse(buffer_t *buffer);

ast_t *analyze_function(buffer_t *buffer);

symbol_t *analyze_parameters(buffer_t *buffer);

var_type_e analyze_return(buffer_t *buffer);

symbol_t *analyze_function_body(buffer_t *buffer);

#endif
