#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "syntax_analysis.h"
#include "symbols.h"

ast_t parse_function(buffer_t *buffer);

#endif
