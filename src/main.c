#include <stdio.h>
#include <stdlib.h>
#include "../include/buffer.h"
#include "../include/lexer.h"
#include "../include/parser.h"

int main() {
    FILE *fd = fopen("../test.intech", "r");
    if (!fd) {
        fprintf(stderr, "Error opening file.\n");
        return EXIT_FAILURE;
    }

    buffer_t buffer;
    buf_init(&buffer, fd);
    parse(&buffer);
    fclose(fd);
    return EXIT_SUCCESS;
}
