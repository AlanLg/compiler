#include "../include/buffer.h"
#include "../include/parser.h"
#include <stdio.h>

int main(int argc, char **argv) {
    printf("testing\n");
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

    analyze_parameters(buffer);

    fclose(file);
    free(buffer);

    return 0;
}
