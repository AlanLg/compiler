#include "syntax_analysis.h"

ast_t *ast_new_integer(long val) {
    ast_t *node = (*ast_t) malloc(sizeof(ast_t));
    if (node) {
        node->type = AST_INTEGER;
        node->integer = val;
    }
    return node;
}
