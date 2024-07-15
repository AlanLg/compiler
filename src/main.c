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

    error_list *errors = init_errorList();
    parse(&buffer, errors);
    print_errors(errors);
    free_errorList(errors);

    fclose(fd);
    return EXIT_SUCCESS;
}
