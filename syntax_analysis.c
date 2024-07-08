#include "syntax_analysis.h"

ast_t *ast_new_integer(long val) {
  ast_t *node = (*ast_t)malloc(sizeof(ast_t));
  if (node) {
    node->type = AST_INTEGER;
    node->integer = val;
  }
  return node;
}

ast_t *ast_new_variable(char *name, var_type_e type) {
  ast_t *variable = (*ast_t)malloc(sizeof(ast_t));
  if (variable) {
    variable->type = AST_VARIABLE;
    variable->var.name = name;
    variable->var.type = type;
  }
  return variable;
}

ast_t *ast_new_binary(ast_binary_e op, ast_t *left, ast_t *right) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_BINARY;
        node->binary.op = op;
        node->binary.left = left;
        node->binary.right = right;
    }
    return node;
}

ast_t *ast_new_unary(ast_unary_e op, ast_t *operand) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_UNARY;
        node->unary.op = op;
        node->unary.operand = operand;
    }
    return node;
}

ast_t *ast_new_function(char *name, var_type_e return_type, ast_list_t *params, ast_list_t *stmts) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_FUNCTION;
        node->function.name = name;
        node->function.return_type = return_type;
        node->function.params = params;
        node->function.stmts = stmts;
    }
    return node;
}
