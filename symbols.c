#include "symbols.h"

symbol_t * sym_new(char *name, sym_type_e type, ast_t *attributes) {
  symbol_t *symbol = (symbol_t*)malloc(sizeof(symbol_t));
  if (symbol) {
    symbol->name = name;
    symbol->type = type;
    symbol->attributes = attributes;
    symbol->next = NULL;
    symbol->function_table = NULL;
  }

  return symbol;
}

void sym_delete(symbol_t * sym) {

}

void sym_remove(symbol_t **table, symbol_t *sym) {

}

void sym_add(symbol_t **table, symbol_t *sym) {
    if (*table == NULL) {
        *table = sym;
    } else {
        symbol_t *temp_symbol = *table;
        while (temp_symbol->next != NULL) {
          temp_symbol = temp_symbol->next;
        }
        temp_symbol->next = temp_symbol;
    }
}

symbol_t * sym_search(symbol_t *table, char *name) {

}
