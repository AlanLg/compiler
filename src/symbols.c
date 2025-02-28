#include "../include/symbols.h"
#include <stdio.h>

symbol_t *sym_new(char *name, sym_type_e type, ast_t *ast_node) {
    symbol_t *symbol = (symbol_t*)malloc(sizeof(symbol_t));
    if (symbol) {
        symbol->name = name;
        symbol->type = type;
        symbol->ast_node = ast_node;
        symbol->next = NULL;
        symbol->function_table = NULL;
    }

  return symbol;
}

void sym_delete(symbol_t **sym) {
    symbol_t *current_symbol = *sym;
    symbol_t *next_symbol;

    while (current_symbol != NULL) {
        next_symbol = current_symbol->next;

        if (current_symbol->name != NULL) {
            free(current_symbol->name);
        }
        if (current_symbol->ast_node != NULL) {
            free(current_symbol->ast_node);
        }
        if (current_symbol->function_table != NULL) {
            sym_delete(&(current_symbol->function_table));
        }

        free(current_symbol);
        current_symbol = next_symbol;
    }

    *sym = NULL;
}

void sym_remove(symbol_t **table, symbol_t *sym) {
    if (table == NULL || *table == NULL || sym == NULL) {
        return;
    }

    symbol_t *current = *table;
    symbol_t *previous = NULL;

    if (current == sym) {
        *table = current->next;
        free(current->name);
        free(current->ast_node);
        free(current);
        return;
    }

    while (current != NULL && current != sym) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    if (previous != NULL) {
        previous->next = current->next;
    }

    free(current->name);
    free(current->ast_node);
    free(current);
}

void sym_add(symbol_t **table, symbol_t *sym) {
    if (*table == NULL) {
        printf("its null, adding %s\n", sym->name);
        *table = sym;
    } else {
        symbol_t *temp_symbol = *table;
        while (temp_symbol->next != NULL) {
            printf("looping, found %s\n", temp_symbol->name);
            temp_symbol = temp_symbol->next;
        }
        temp_symbol->next = sym;
    }
}

symbol_t * sym_search(symbol_t *table, char *name) {
    printf("searching for name : %s \n", name);
    symbol_t *temp = table;
    while (temp != NULL) {
        printf("checking for name: %s\n", temp->name);
        if(strcmp(temp->name, name) == 0) {
            printf("found for name: %s\n", temp->name);
            return temp;
        }
        temp = temp->next;
    }
    printf("returning null\n");
    return NULL;
}
