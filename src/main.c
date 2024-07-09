#include "../include/buffer.h"
#include "../include/lexer.h"
#include "../include/utils.h"

int main(int argc, char **argv) {
    FILE *file = fopen("../test.intech", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    buffer_t *buffer = (buffer_t *) malloc(sizeof(buffer_t));
    if (!buffer) {
        perror("Failed to allocate memory for buffer");
        fclose(file);
        return 1;
    }

    buf_init(buffer, file);

    while (!buf_eof(buffer)) {
        char *op = lexer_getop(buffer);
        if (op) {
            printf("Operator: %s\n", op);
            free(op);
            continue;
        }

        long number = lexer_getnumber(buffer);
        if (number != 0 || (number == 0 && buf_getchar_rollback(buffer) == '0')) {
            printf("Number: %ld\n", number);
            continue;
        }

        char *alphanum = lexer_getalphanum(buffer);
        if (alphanum) {
            printf("Alphanum: %s\n", alphanum);
            free(alphanum);
            continue;
        }

        // Read the next character to avoid infinite loop
        buf_getchar(buffer);
    }

    fclose(file);
    free(buffer);

    return 0;
}
