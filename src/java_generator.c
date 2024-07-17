#include "parser.h"

const char *convert_binary_op(ast_binary_e op) {
    switch (op) {
        case AST_ADDITION:
            return "+";
        case AST_SUBSTRACTION:
            return "-";
        case AST_MULTIPLICATION:
            return "*";
        case AST_DIVISION:
            return "/";
        case AST_EQUAL_TO:
            return "==";
        case AST_NOT_EQUAL_TO:
            return "!=";
        case AST_GREATER_THAN:
            return ">";
        case AST_LESS_THAN:
            return "<";
        case AST_GREATER_THAN_OR_EQUAL_TO:
            return ">=";
        case AST_LESS_THAN_OR_EQUAL_TO:
            return "<=";
        case AST_LOGICAL_AND:
            return "&&";
        case AST_LOGICAL_OR:
            return "||";
        default:
            return "Unknown";
    }
}

const char *convert_var_type(var_type_e type) {
    switch (type) {
        case VAR_INTEGER:
            return "int";
        case VAR_BOOL:
            return "boolean";
        case VAR_VOID:
            return "void";
        default:
            return "Unknown";
    }
}

void generate_java_code(ast_t *node, FILE *output) {
    if (!node) return;
    ast_list_t *stmts;

    switch (node->type) {
        case AST_INTEGER:
            fprintf(output, "%ld", node->integer);
            break;
        case AST_VARIABLE:
            fprintf(output, "%s %s", convert_var_type(node->var.type), node->var.name);
            break;
        case AST_BINARY:
            fprintf(output, "(");
            generate_java_code(node->binary.left, output);
            fprintf(output, " %s ", convert_binary_op(node->binary.op));
            generate_java_code(node->binary.right, output);
            fprintf(output, ")");
            break;
        case AST_ASSIGNMENT:
            generate_java_code(node->assignment.lvalue, output);
            fprintf(output, " = ");
            generate_java_code(node->assignment.rvalue, output);
            fprintf(output, ";");
            break;
        case AST_DECLARATION:
            fprintf(output, "%s %s", convert_var_type(node->declaration.lvalue->var.type),
                    node->declaration.lvalue->var.name);
            if (node->declaration.rvalue) {
                fprintf(output, " = ");
                generate_java_code(node->declaration.rvalue, output);
            }
            fprintf(output, ";");
            break;
        case AST_FUNCTION:
            fprintf(output, "%s %s(", convert_var_type(node->function.return_type), node->function.name);
            ast_list_t *param = node->function.params;
            while (param) {
                generate_java_code(param->current, output);
                if (param->next) fprintf(output, ", ");
                param = param->next;
            }
            fprintf(output, ") {\n");
            generate_java_code(node->function.stmts->current, output);
            fprintf(output, "\n}");
            break;
        case AST_COMPOUND_STATEMENT:
            stmts = node->compound_stmt.stmts;
            while (stmts) {
                fprintf(output, "    ");
                generate_java_code(stmts->current, output);
                if (stmts->current->type != AST_DECLARATION && stmts->current->type != AST_COMPOUND_STATEMENT &&
                    stmts->current->type != AST_CONDITION) {
                    fprintf(output, ";");
                }
                stmts = stmts->next;
            }
            break;
        case AST_CONDITION:
            fprintf(output, "if (");
            generate_java_code(node->branch.condition, output);
            fprintf(output, ") {\n    ");
            generate_java_code(node->branch.valid, output);
            fprintf(output, "\n    }");
            if (node->branch.invalid) {
                if (node->branch.invalid->type == AST_CONDITION) {
                    fprintf(output, " else ");
                    generate_java_code(node->branch.invalid, output);
                } else {
                    fprintf(output, " else {\n    ");
                    generate_java_code(node->branch.invalid, output);
                    fprintf(output, "\n}");
                }
            }
            break;
        default:
            fprintf(output, "Unknown");
            break;
    }
}

void parse_and_generate(buffer_t *buffer, error_list *errors, const char *output_file) {
    FILE *output = fopen(output_file, "w");
    if (!output) {
        fprintf(stderr, "Impossible d'ouvrir le fichier de sortie %s\n", output_file);
        return;
    }

    symbol_t *table = parse(buffer, errors);

    if (errors->count > 0) {
        fprintf(stderr, "Erreurs rencontrées lors de l'analyse:\n");
        print_errors(errors);
        fclose(output);
        return;
    }

    symbol_t *current_function = table;
    while (current_function) {
        generate_java_code(current_function->ast_node, output);
        fprintf(output, "\n");
        current_function = current_function->next;
    }

    fclose(output);
}
