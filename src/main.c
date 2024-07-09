#include "../include/buffer.h"
#include "../include/lexer.h"
#include "../include/utils.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
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
    }

    fclose(file);
    free(buffer);

    return 0;
}
