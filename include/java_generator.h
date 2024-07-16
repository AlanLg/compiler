#ifndef COMPILER_JAVA_GENERATOR_H
#define COMPILER_JAVA_GENERATOR_H

#include "errors.h"
#include "buffer.h"
#include "ast.h"

const char *convert_binary_op(ast_binary_e op);

void generate_java_code(ast_t *node, FILE *output);

void parse_and_generate(buffer_t *buffer, error_list *errors, const char *output_file);

#endif //COMPILER_JAVA_GENERATOR_H
