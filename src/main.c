#include <stdio.h>
#include <stdlib.h>
#include "../include/buffer.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/errors.h"

int main() {
    FILE *fd = fopen("../test.intech", "r");
    if (!fd) {
        fprintf(stderr, "Error opening file.\n");
        return EXIT_FAILURE;
    }

    buffer_t buffer;
    buf_init(&buffer, fd);

    ErrorList *errors = initErrorList();
    parse(&buffer, errors);
    printErrors(errors);
    freeErrorList(errors);

    fclose(fd);
    return EXIT_SUCCESS;
}
