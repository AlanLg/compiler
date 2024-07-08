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
