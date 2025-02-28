#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "utils.h"

typedef enum {
  VAR_INTEGER,
  VAR_BOOL,
  VAR_VOID,
    VAR_UNKNOWN,
} var_type_e;

typedef enum {
  AST_EQUAL_TO,
  AST_NOT_EQUAL_TO,
  AST_GREATER_THAN,
  AST_LESS_THAN,
  AST_GREATER_THAN_OR_EQUAL_TO,
  AST_LESS_THAN_OR_EQUAL_TO,
  AST_LOGICAL_AND,
  AST_LOGICAL_OR,
  AST_ADDITION,
  AST_SUBSTRACTION,
  AST_MULTIPLICATION,
  AST_DIVISION,
  AST_UNKNOWN,
} ast_binary_e;

typedef enum {
  AST_UNARY_MINUS,
  AST_INCREMENT,
  AST_DECREMENT,
} ast_unary_e;

typedef enum {
  AST_VOID,
  AST_INTEGER,
  AST_BINARY,
  AST_UNARY,
  AST_FUNCTION,
  AST_FNCALL,
  AST_VARIABLE,
  AST_CONDITION,
  AST_LOOP,
  AST_DECLARATION,
  AST_ASSIGNMENT,
  AST_COMPOUND_STATEMENT,
  AST_RETURN
} ast_node_type_e;

typedef struct ast_list_t {
    struct ast_t *current;
    struct ast_list_t *next;
} ast_list_t;

typedef struct ast_t {
  ast_node_type_e type;
  union {
    long integer;
    ast_list_t list;
    struct {
      char *name;
      var_type_e type;
    } var;

    struct {
      ast_binary_e op;
      struct ast_t *left;
      struct ast_t *right;
    } binary;

    struct {
      char op;
      struct ast_t *operand;
    } unary;

    struct {
      char *name;
      struct ast_list_t *args;
    } call;

    struct {
      char *name;
      var_type_e return_type;
      struct ast_list_t *params;
      struct ast_list_t *stmts;
    } function;

    struct {
      struct ast_list_t *stmts;
    } compound_stmt;

    struct {
      struct ast_t *lvalue;
      struct ast_t *rvalue;
    } assignment;

    struct {
      struct ast_t *lvalue;
      struct ast_t *rvalue;
    } declaration;

    struct {
      struct ast_t *condition;
      struct ast_t *valid;
      struct ast_t *invalid;
    } branch;

    struct {
      struct ast_t *condition;
      struct ast_t *stmt;
    } loop;

    struct {
      struct ast_t *expr;
    } ret;
  };
} ast_t;

ast_t *ast_new_integer(long val);
ast_t *ast_new_variable(char *name, var_type_e type);
ast_t *ast_new_binary(ast_binary_e op, ast_t *left, ast_t *right);
ast_t *ast_new_unary(ast_unary_e op, ast_t *operand);
ast_t *ast_new_function(char *name, var_type_e return_type, ast_list_t *params, ast_list_t *stmts);
ast_t *ast_new_fncall(char *name, ast_list_t *args);
ast_t *ast_new_comp_stmt(ast_list_t *stmts);
ast_t *ast_new_assignment(ast_t *lvalue, ast_t *rvalue);
ast_t *ast_new_declaration(ast_t *lvalue, ast_t *rvalue);
ast_t *ast_new_condition(ast_t *condition, ast_t *valid, ast_t *invalid);
ast_t *ast_new_loop(ast_t *condition, ast_t *stmt);
ast_t *ast_new_return(ast_t *expr);
ast_list_t *ast_list_new_node(ast_t *elem);
ast_list_t *ast_list_add(ast_list_t **list, ast_t *elem);
var_type_e get_var_type_from_string(const char* var_type_string);
ast_binary_e ast_get_binary_from_string(const char *operator);

#endif
