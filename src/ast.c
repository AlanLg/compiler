#include "../include/ast.h"

const char* var_type_strings[] = {
        "entier",
        "bool",
        "rien"
};

var_type_e get_var_type_from_string(const char* var_type_string) {
    for (int i = 0; i < sizeof(var_type_strings) / sizeof(var_type_strings[0]); ++i) {
        if (strcmp(var_type_string, var_type_strings[i]) == 0) {
            return (var_type_e)i;
        }
    }
    return UNKNOWN;
}


ast_t *ast_new_integer(long val) {
  ast_t *node = (ast_t*)malloc(sizeof(ast_t));
  if (node) {
    node->type = AST_INTEGER;
    node->integer = val;
  }
  return node;
}

ast_t *ast_new_variable(char *name, var_type_e type) {
  ast_t *variable = (ast_t*)malloc(sizeof(ast_t));
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

ast_t *ast_new_comp_stmt(ast_list_t *stmts) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_COMPOUND_STATEMENT;
        node->compound_stmt.stmts = stmts;
    }
    return node;
}

ast_t *ast_new_assignment(ast_t *lvalue, ast_t *rvalue) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_ASSIGNMENT;
        node->assignment.lvalue = lvalue;
        node->assignment.rvalue = rvalue;
    }
    return node;
}

ast_t *ast_new_declaration(ast_t *lvalue, ast_t *rvalue) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_DECLARATION;
        node->declaration.lvalue = lvalue;
        node->declaration.rvalue = rvalue;
    }
    return node;
}

ast_t *ast_new_condition(ast_t *condition, ast_t *valid, ast_t *invalid) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_CONDITION;
        node->branch.condition = condition;
        node->branch.valid = valid;
        node->branch.invalid = invalid;
    }
    return node;
}

ast_t *ast_new_loop(ast_t *condition, ast_t *stmt) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_LOOP;
        node->loop.condition = condition;
        node->loop.stmt = stmt;
    }
    return node;
}

ast_t *ast_new_return(ast_t *expr) {
    ast_t *node = (ast_t*)malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_RETURN;
        node->ret.expr = expr;
    }
    return node;
}

ast_list_t *ast_list_new_node(ast_t *elem) {
    ast_list_t *node = (ast_list_t *)malloc(sizeof(ast_t));
    if (node) {
        node->current = elem;
        node->next = NULL;
    }
    return node;
}

ast_list_t *ast_list_add(ast_list_t **list, ast_t *elem) {
    ast_list_t *new_ast_list = ast_list_new_node(elem);
    if (*list == NULL) {
        *list = new_ast_list;
    } else {
        ast_list_t *temp_ast_list = *list;
        while (temp_ast_list->next != NULL) {
          temp_ast_list = temp_ast_list->next;
        }
        temp_ast_list->next = new_ast_list;
    }
    return new_ast_list;
}

