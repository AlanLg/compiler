#include "../include/parser.h"
#include "lexer.h"
#include <string.h>

symbol_t *parse(buffer_t *buffer, error_list *errors) {
    if (strcmp(lexer_getalpha(buffer, errors), "fonction") != 0) {
        add_error(errors, "Le premier lexème devrait être \"fonction\"");
        return NULL;
    }

    symbol_t *main_function_symbol = analyze_function(buffer, errors);
    if (!main_function_symbol) {
        add_error(errors, "Erreur lors de l'analyse de la fonction principale");
        return NULL;
    }

    if (!buf_eof(buffer)) {
        add_error(errors, "Il reste du contenu inattendu après la fonction principale");
        return NULL;
    }

    return main_function_symbol;
}

symbol_t *analyze_function(buffer_t *buffer, error_list *errors) {
    char *name = lexer_getalphanum(buffer, errors);
    if (!name) {
        add_error(errors, "Nom de fonction invalide");
        return NULL;
    }

    ast_list_t *params = analyze_parameters(buffer, errors);
    if (!params) {
        add_error(errors, "Erreur lors de l'analyse des paramètres de la fonction");
        return NULL;
    }

    var_type_e return_type = analyze_return(buffer, errors);
    if (return_type == VAR_UNKNOWN) {
        add_error(errors, "Type de retour de fonction invalide");
        return NULL;
    }

    ast_t *body = analyze_function_body(buffer, errors);
    if (!body) {
        add_error(errors, "Erreur lors de l'analyse du corps de la fonction");
        return NULL;
    }


    ast_t *function_ast = ast_new_function(name, return_type, params, ast_list_new_node(body));

    symbol_t *function_symbol = sym_new(name, SYM_FUNCTION, function_ast);

    return function_symbol;
}


ast_list_t *analyze_parameters(buffer_t *buffer, error_list *errors) {
    if (lexer_getchar(buffer, errors) != '(') {
        add_error(errors, "Parenthèse ouvrante manquante pour les paramètres de la fonction");
        return NULL;
    }

    ast_list_t *parameters = NULL;

    char next = lexer_getchar(buffer, errors);
    while (next != ')') {
        if (next == ',') {
            add_error(errors, "Virgule inattendue au début des paramètres");
            return NULL;
        }

        buf_rollback(buffer, 1);

        char *param_type_name = lexer_getalpha(buffer, errors);
        if (!param_type_name) {
            add_error(errors, "Type de paramètre invalide");
            return NULL;
        }
        var_type_e param_type = get_var_type_from_string(param_type_name);
        if (param_type == VAR_UNKNOWN) {
            add_error(errors, "Type de paramètre inconnu");
            return NULL;
        }

        char *param_name = lexer_getalphanum(buffer, errors);
        if (!param_name) {
            add_error(errors, "Nom de paramètre invalide");
            return NULL;
        }

        ast_t *param_ast = ast_new_variable(param_name, param_type);
        ast_list_add(&parameters, param_ast);

        next = lexer_getchar(buffer, errors);
        if (next == ',') {
            next = lexer_getchar(buffer, errors);
            continue;
        } else if (next != ')') {
            add_error(errors, "Virgule ou parenthèse fermante attendue après le paramètre");
            return NULL;
        }
    }

    return parameters;
}

var_type_e analyze_return(buffer_t *buffer, error_list *errors) {
    if (lexer_getchar(buffer, errors) != ':') {
        add_error(errors, "Double-point manquant après le type de retour de la fonction");
        return VAR_UNKNOWN;
    }

    char *return_type_name = lexer_getalpha(buffer, errors);
    if (!return_type_name) {
        add_error(errors, "Type de retour de fonction invalide");
        return VAR_UNKNOWN;
    }

    var_type_e return_type = get_var_type_from_string(return_type_name);
    if (return_type == VAR_UNKNOWN) {
        add_error(errors, "Type de retour de fonction inconnu");
        return VAR_UNKNOWN;
    }

    return return_type;
}

ast_t *analyze_function_body(buffer_t *buffer, error_list *errors) {
    if (lexer_getchar(buffer, errors) != '{') {
        add_error(errors, "Accolade ouvrante manquante pour le corps de la fonction");
        return NULL;
    }

    ast_list_t *stmts = analyze_statements(buffer, errors);
    if (!stmts) {
        add_error(errors, "Erreur lors de l'analyse des déclarations et instructions de la fonction");
        return NULL;
    }

    ast_t *comp_stmt_ast = ast_new_comp_stmt(stmts);

    return comp_stmt_ast;
}


ast_list_t *analyze_statements(buffer_t *buffer, error_list *errors) {
    ast_list_t *statements = NULL;

    char next = lexer_getchar(buffer, errors);
    while (next != '}') {
        buf_rollback(buffer, 1);

        char *next_keyword = lexer_getalpha(buffer, errors);
        if (!next_keyword) {
            add_error(errors, "Mot-clé d'instruction invalide ou manquant");
            return NULL;
        }

        if (strcmp(next_keyword, "entier") == 0 || strcmp(next_keyword, "bool") == 0 ||
            strcmp(next_keyword, "rien") == 0) {
            ast_t *declaration_ast = analyze_declaration(buffer, errors, next_keyword);
            if (!declaration_ast) {
                add_error(errors, "Erreur lors de l'analyse de la déclaration de variable");
                return NULL;
            }
            ast_list_add(&statements, declaration_ast);
        } else if (strcmp(next_keyword, "si") == 0) {
            ast_t *conditional_ast = analyze_conditional(buffer, errors, next_keyword);
            if (!conditional_ast) {
                add_error(errors, "Erreur lors de l'analyse de l'instruction conditionnelle");
                return NULL;
            }
            ast_list_add(&statements, conditional_ast);
        } else {
            add_error(errors, "Mot-clé d'instruction invalide");
            return NULL;
        }

        next = lexer_getchar(buffer, errors);
    }

    return statements;
}


ast_t *analyze_declaration(buffer_t *buffer, error_list *errors, char *type_name) {
    if (!type_name) {
        add_error(errors, "Type de variable invalide");
        return NULL;
    }

    var_type_e type = get_var_type_from_string(type_name);
    if (type == VAR_UNKNOWN) {
        add_error(errors, "Type de variable inconnu");
        return NULL;
    }

    char *var_name = lexer_getalphanum(buffer, errors);
    if (!var_name) {
        add_error(errors, "Nom de variable invalide");
        return NULL;
    }

    ast_t *var_decl_ast = ast_new_declaration(ast_new_variable(var_name, type), NULL);

    char *next = lexer_getoperator(buffer, errors);
    if (!next) {
        buf_rollback(buffer, 1);

        if (lexer_getchar(buffer, errors) != ';') {
            add_error(errors, "Point-virgule attendu à la fin de la déclaration de variable");
            return NULL;
        }
    } else if (strcmp(next, "=") == 0) {
        ast_t *value_ast = analyze_expression(buffer, errors);
        if (!value_ast) {
            add_error(errors, "Erreur lors de l'analyse de l'expression d'initialisation de variable");
            return NULL;
        }

        ast_t *expression = value_ast;

        var_decl_ast->declaration.rvalue = expression;

        if (lexer_getchar(buffer, errors) != ';') {
            add_error(errors, "Point-virgule attendu à la fin de la déclaration de variable");
            return NULL;
        }
    } else {
        add_error(errors, "Opérateur d'assignation '=' attendu après le nom de la variable");
        return NULL;
    }

    return var_decl_ast;
}

ast_t *analyze_conditional(buffer_t *buffer, error_list *errors, char *next_keyword) {
    if (strcmp(next_keyword, "si") != 0) {
        add_error(errors, "Mot-clé \"si\" manquant pour l'instruction conditionnelle");
        return NULL;
    }

    if (lexer_getchar(buffer, errors) != '(') {
        add_error(errors, "Parenthèse ouvrante manquante pour la condition de l'instruction conditionnelle");
        return NULL;
    }

    ast_t *condition = analyze_expression(buffer, errors);
    if (!condition) {
        add_error(errors, "Erreur lors de l'analyse de la condition de l'instruction conditionnelle");
        return NULL;
    }

    if (lexer_getchar(buffer, errors) != ')') {
        add_error(errors, "Parenthèse fermante manquante après la condition de l'instruction conditionnelle");
        return NULL;
    }

    ast_t *true_stmt = analyze_function_body(buffer, errors);
    if (!true_stmt) {
        add_error(errors, "Erreur lors de l'analyse du bloc d'instructions du \"si\"");
        return NULL;
    }

    ast_t *current_conditional = ast_new_condition(condition, true_stmt, NULL);
    ast_t *last_conditional = current_conditional;

    while (true) {
        char *next_keyword = lexer_getalpha(buffer, errors);

        if (strcmp(next_keyword, "sinon") == 0) {
            char *peek_keyword = lexer_getalpha(buffer, errors);

            if (strcmp(peek_keyword, "si") == 0) {
                if (lexer_getchar(buffer, errors) != '(') {
                    add_error(errors, "Parenthèse ouvrante manquante pour la condition du \"sinon si\"");
                    return NULL;
                }
                ast_t *condition_elseif = analyze_expression(buffer, errors);
                if (!condition_elseif) {
                    add_error(errors, "Erreur lors de l'analyse de la condition du \"sinon si\"");
                    return NULL;
                }
                if (lexer_getchar(buffer, errors) != ')') {
                    add_error(errors, "Parenthèse fermante manquante après la condition du \"sinon si\"");
                    return NULL;
                }

                ast_t *true_stmt_elseif = analyze_function_body(buffer, errors);
                if (!true_stmt_elseif) {
                    add_error(errors, "Erreur lors de l'analyse du bloc d'instructions du \"sinon si\"");
                    return NULL;
                }

                last_conditional->branch.invalid = ast_new_condition(condition_elseif, true_stmt_elseif, NULL);
                last_conditional = last_conditional->branch.invalid;
            } else {
                buf_rollback(buffer, 1);
                ast_t *false_stmt = analyze_function_body(buffer, errors);
                if (!false_stmt) {
                    add_error(errors, "Erreur lors de l'analyse du bloc d'instructions du \"sinon\"");
                    return NULL;
                }

                last_conditional->branch.invalid = ast_new_condition(NULL, false_stmt, NULL);
                last_conditional = last_conditional->branch.invalid;
            }
        } else {
            buf_rollback(buffer, 1);
            break;
        }
    }

    return current_conditional;
}

/*
ast_t *analyze_loop(buffer_t *buffer, error_list *errors) {
    if (strcmp(lexer_getalpha(buffer, errors), "tantque") != 0) {
        add_error(errors, "Mot-clé \"tantque\" manquant pour l'instruction de boucle");
        return NULL;
    }

    ast_t *condition = analyze_expression(buffer, errors);
    if (!condition) {
        add_error(errors, "Erreur lors de l'analyse de la condition de l'instruction de boucle");
        return NULL;
    }

    ast_t *body = analyze_function_body(buffer, errors);
    if (!body) {
        add_error(errors, "Erreur lors de l'analyse du bloc d'instructions de la boucle");
        return NULL;
    }

    ast_t *loop_ast = ast_new_loop(condition, body);

    return loop_ast;
}
*/

ast_t *analyze_expression(buffer_t *buffer, error_list *errors) {
    string_stack_t operator_stack;
    string_stack_initialize(&operator_stack);

    ast_stack_t output_stack;
    ast_stack_initialize(&output_stack);

    while (true) {
        char number = *lexer_getnumber(buffer, errors);

        if (strcmp(&number, "\0") == 0) {
            add_error(errors, "invalid number found in expression");
            break;
        }

        int number_as_int = atoi(&number);
        ast_t *number_as_ast = ast_new_integer(number_as_int);
        ast_stack_push(&output_stack, number_as_ast);

        buf_lock(buffer);

        const char next = lexer_getchar(buffer, errors);

        buf_rollback_and_unlock(buffer, 1);
        if (next == ';' || next == ')') {
            break;
        }

        char* operator = lexer_getoperator(buffer, errors);

        if (operator == NULL) {
            add_error(errors, "invalid operator found in expression");
            break;
        }

        if (string_stack_is_empty(&operator_stack)) {
            string_stack_push(&operator_stack, operator);
        } else {
            string_stack_push(&operator_stack, operator);
            char *pop = string_stack_pop(&operator_stack);

            ast_t* binary = ast_new_binary(ast_get_binary_from_string(pop), NULL, NULL);

            ast_stack_push(&output_stack, binary);
        }
    }

    while (!string_stack_is_empty(&operator_stack)) {
        const char *pop = string_stack_pop(&operator_stack);
        ast_t* binary = ast_new_binary(ast_get_binary_from_string(pop), NULL, NULL);
        ast_stack_push(&output_stack, binary);
    }
    ast_t* stack_to_ast = ast_stack_to_ast(&output_stack);

    return stack_to_ast;
}

ast_t* ast_stack_to_ast(ast_stack_t *stack) {
    if (ast_stack_is_empty(stack)) {
        return NULL;
    }
    ast_t* current = ast_stack_pop(stack);
    if (current->type == AST_BINARY) {
        if (current->binary.right == NULL) {
            current->binary.right = ast_stack_to_ast(stack);
        }
        if (current->binary.left == NULL) {
            current->binary.left = ast_stack_to_ast(stack);
        }
    }
    return current;
}
