#include <stdio.h>
#include <stdlib.h>
#include "../include/buffer.h"
#include "../include/lexer.h"
#include "java_generator.h"

int main() {
    const char *input_file = "../fichier_entrant.intech";
    const char *output_file = "../fichier_sortant.java";

    FILE *input = fopen(input_file, "r");
    if (!input) {
        fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée %s\n", input_file);
        return EXIT_FAILURE;
    }

    buffer_t buffer;
    buf_init(&buffer, input);

    error_list *errors = init_errorList();

    parse_and_generate(&buffer, errors, output_file);

    if (errors->count > 0) {
        fprintf(stderr, "Erreurs rencontrées lors de l'analyse:\n");
        print_errors(errors);
        fclose(input);
        free_errorList(errors);
        return EXIT_FAILURE;
    }

    fclose(input);
    free_errorList(errors);

    printf("Transpilation terminée avec succès. Code Java généré dans %s\n", output_file);
    return EXIT_SUCCESS;
}

