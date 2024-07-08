#ifndef SYMBOLES_H
#define SYMBOLES_H

#include "syntax_analysis.h"

typedef enum {
  SYM_FUNCTION,
  SYM_VAR,
  SYM_PARAM
} sym_type_e;

typedef struct symbol_t {
  char *name;
  sym_type_e type;
  ast_t *attributes;
  struct symbol_t *function_table;
  struct symbol_t *next;
} symbol_t;

symbol_t * sym_new (char *name, sym_type_e type, ast_t *attributes);
void sym_delete (symbol_t * sym);
void sym_remove (symbol_t **table, symbol_t *sym);
void sym_add (symbol_t **table, symbol_t *sym);
symbol_t * sym_search (symbol_t *table, char *name);

#endif
