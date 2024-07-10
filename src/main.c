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

    char *alpha;
    char *alphanum;
    char *op;
    char punct;
    char *num;

    while (!buf_eof(&buffer)) {
        alpha = lexer_getalpha(&buffer);
        if (alpha[0] != '\0') {
            printf("Alpha: %s\n", alpha);
        }

        alphanum = lexer_getalphanum(&buffer);
        if (alphanum[0] != '\0') {
            printf("Alphanum: %s\n", alphanum);
        }

        op = lexer_getop(&buffer);
        if (op != '\0') {
            printf("Operator: %s\n", op);
        }

        punct = lexer_getchar(&buffer);
        if (punct != '\0') {
            printf("Punctuation: %c\n", punct);
        }

        num = lexer_getnumber(&buffer);
        if (num[0] != '\0') {
            printf("Number: %s\n", num);
        }
    }

    fclose(fd);
    return EXIT_SUCCESS;
}
