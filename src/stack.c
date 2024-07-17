#include "../include/stack.h"

void string_stack_initialize(string_stack_t *stack) {
    stack->top = -1;
}

bool string_stack_is_empty(string_stack_t *stack) {
    return stack->top == -1;
}

bool string_stack_is_full(string_stack_t *stack) {
    return stack->top == MAX_SIZE - 1;
}

void string_stack_push(string_stack_t *stack, char* value) {
    if (string_stack_is_full(stack)) {
        printf("Stack Overflow\n");
        return;
    }
    stack->arr[++stack->top] = strdup(value);
}

char* string_stack_pop(string_stack_t *stack) {
    if (string_stack_is_empty(stack)) {
        printf("Stack Underflow\n");
        return NULL;
    }
    char* popped = stack->arr[stack->top];
    stack->top--;
    return popped;
}

char* string_stack_peek(string_stack_t *stack) {
    if (string_stack_is_empty(stack)) {
        printf("Stack is empty\n");
        return NULL;
    }
    return stack->arr[stack->top];
}

void ast_stack_initialize(ast_stack_t *stack) {
    stack->top = -1;
}

bool ast_stack_is_empty(ast_stack_t *stack) {
    return stack->top == -1;
}

bool ast_stack_is_full(ast_stack_t *stack) {
    return stack->top == MAX_SIZE - 1;
}

void ast_stack_push(ast_stack_t *stack, ast_t* value) {
    if (ast_stack_is_full(stack)) {
        printf("Stack Overflow\n");
        return;
    }
    stack->arr[++stack->top] = value;
}

ast_t* ast_stack_pop(ast_stack_t *stack) {
    if (ast_stack_is_empty(stack)) {
        printf("Stack is empty\n");
        return NULL;
    }
    ast_t* popped = stack->arr[stack->top];
    stack->top--;
    return popped;
}

ast_t* ast_stack_peek(ast_stack_t *stack) {
    if (ast_stack_is_empty(stack)) {
        printf("Stack is empty\n");
        return NULL;
    }
    return stack->arr[stack->top];
}
