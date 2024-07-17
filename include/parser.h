#ifndef PARSER_H
#define PARSER_H

#include "stack.h"
#include "buffer.h"
#include "ast.h"
#include "symbols.h"
#include "errors.h"

symbol_t *parse(buffer_t *buffer, error_list *errors);

symbol_t *analyze_function(buffer_t *buffer, error_list *errors);

ast_list_t *analyze_parameters(buffer_t *buffer, error_list *errors);

var_type_e analyze_return(buffer_t *buffer, error_list *errors);

ast_t *analyze_function_body(buffer_t *buffer, error_list *errors);

ast_t *analyze_declaration(buffer_t *buffer, error_list *errors, char *string);

ast_t *analyze_conditional(buffer_t *buffer, error_list *errors, char *string);

ast_t *analyze_loop(buffer_t *buffer, error_list *errors);

ast_t *analyze_expression(buffer_t *buffer, error_list *errors);

ast_list_t *analyze_statements(buffer_t *buffer, error_list *errors);

ast_t* ast_stack_to_ast(ast_stack_t *stack);

#endif
