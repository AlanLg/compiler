#include <stdio.h>
#include <stdlib.h>
#include "../include/buffer.h"
#include "lexer.h"

int main() {
    FILE *fd = fopen("../test.intech", "r");
    if (!fd) {
        fprintf(stderr, "Error opening file.\n");
        return EXIT_FAILURE;
    }

    buffer_t buffer;
    buf_init(&buffer, fd);

    char *token;
    char op;
    char punct;
    char *num;

    while (!buf_eof(&buffer)) {
        token = lexer_getalpha(&buffer);
        if (token[0] != '\0') {
            printf("Alpha: %s\n", token);
            continue;
        }

        // Then check for numbers
        num = lexer_getnumber(&buffer);
        if (num[0] != '\0') {
            printf("Number: %s\n", num);
            continue;
        }

        // Then check for operators
        op = lexer_getoperator(&buffer);
        if (op != '\0') {
            printf("Operator: %c\n", op);
            continue;
        }

        // Then check for punctuation
        punct = lexer_getchar(&buffer);
        if (punct != '\0') {
            printf("Punctuation: %c\n", punct);
            continue;
        }

        // Finally check for alphanumeric tokens
        token = lexer_getalphanum(&buffer);
        if (token[0] != '\0') {
            printf("AlphaNum: %s\n", token);
        }
    }

    fclose(fd);
    return EXIT_SUCCESS;
}
