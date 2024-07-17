#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdbool.h>

#include "ast.h"

#define MAX_SIZE 10

typedef struct {
    char* arr[MAX_SIZE];
    int top;
} string_stack_t;

typedef struct {
    ast_t* arr[MAX_SIZE];
    int top;
} ast_stack_t;


void ast_stack_initialize(ast_stack_t *stack);
bool ast_stack_is_empty(ast_stack_t *stack);
bool ast_stack_is_full(ast_stack_t *stack);
void ast_stack_push(ast_stack_t *stack, ast_t* value);
ast_t* ast_stack_pop(ast_stack_t *stack);
ast_t* ast_stack_peek(ast_stack_t *stack);

void string_stack_initialize(string_stack_t *stack);
bool string_stack_is_empty(string_stack_t *stack);
bool string_stack_is_full(string_stack_t *stack);
void string_stack_push(string_stack_t *stack, char* value);
char* string_stack_pop(string_stack_t *stack);
char* string_stack_peek(string_stack_t *stack);

#endif
