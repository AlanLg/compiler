#include "../include/parser.h"
#include "lexer.h"
#include <string.h>

// Fonction pour l'analyse syntaxique et la construction de l'AST
symbol_t *parse(buffer_t *buffer, error_list *errors) {
    // Vérifier la présence de la première fonction
    if (strcmp(lexer_getalpha(buffer, errors), "fonction") != 0) {
        add_error(errors, "Le premier lexème devrait être \"fonction\"");
        return NULL;
    }

    // Analyser la fonction principale
    symbol_t *main_function_symbol = analyze_function(buffer, errors);
    if (!main_function_symbol) {
        add_error(errors, "Erreur lors de l'analyse de la fonction principale");
        return NULL;
    }

    // Vérifier s'il reste du contenu non analysé
    if (!buf_eof(buffer)) {
        add_error(errors, "Il reste du contenu inattendu après la fonction principale");
        return NULL;
    }

    return main_function_symbol;
}

// Analyse d'une fonction
symbol_t *analyze_function(buffer_t *buffer, error_list *errors) {
    char *name = lexer_getalphanum(buffer, errors);
    if (!name) {
        add_error(errors, "Nom de fonction invalide");
        return NULL;
    }

    // Analyser les paramètres de la fonction
    ast_list_t *params = analyze_parameters(buffer, errors);
    if (!params) {
        add_error(errors, "Erreur lors de l'analyse des paramètres de la fonction");
        return NULL;
    }

    // Analyser le type de retour
    var_type_e return_type = analyze_return(buffer, errors);
    if (return_type == VAR_UNKNOWN) {
        add_error(errors, "Type de retour de fonction invalide");
        return NULL;
    }

    // Analyser le corps de la fonction
    ast_t *body = analyze_function_body(buffer, errors);
    if (!body) {
        add_error(errors, "Erreur lors de l'analyse du corps de la fonction");
        return NULL;
    }


    // Créer le nœud AST pour la fonction
    ast_t *function_ast = ast_new_function(name, return_type, params, ast_list_new_node(body));

    // Créer le symbole pour la fonction
    symbol_t *function_symbol = sym_new(name, SYM_FUNCTION, function_ast);

    return function_symbol;
}


// Analyse des paramètres de la fonction
ast_list_t *analyze_parameters(buffer_t *buffer, error_list *errors) {
    if (lexer_getchar(buffer, errors) != '(') {
        add_error(errors, "Parenthèse ouvrante manquante pour les paramètres de la fonction");
        return NULL;
    }

    ast_list_t *parameters = NULL;

    // Analyse des paramètres jusqu'à la parenthèse fermante
    char next = lexer_getchar(buffer, errors);
    while (next != ')') {
        // Vérifier si c'est une virgule (pas attendue au début des paramètres)
        if (next == ',') {
            add_error(errors, "Virgule inattendue au début des paramètres");
            return NULL;
        }

        // Retourner le caractère pour pouvoir le réutiliser dans la logique suivante
        buf_rollback(buffer, 1);

        // Analyser le type du paramètre
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

        // Analyser le nom du paramètre
        char *param_name = lexer_getalphanum(buffer, errors);
        if (!param_name) {
            add_error(errors, "Nom de paramètre invalide");
            return NULL;
        }

        // Créer le nœud AST pour le paramètre
        ast_t *param_ast = ast_new_variable(param_name, param_type);
        ast_list_add(&parameters, param_ast);

        // Vérifier s'il y a une virgule ou la fin des paramètres
        next = lexer_getchar(buffer, errors);
        if (next == ',') {
            // Si c'est une virgule, continuer à analyser les paramètres suivants
            next = lexer_getchar(buffer, errors);
            continue;
        } else if (next != ')') {
            add_error(errors, "Virgule ou parenthèse fermante attendue après le paramètre");
            return NULL;
        }
    }

    return parameters;
}

// Analyse du type de retour de la fonction
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

// Analyse du corps de la fonction
ast_t *analyze_function_body(buffer_t *buffer, error_list *errors) {
    if (lexer_getchar(buffer, errors) != '{') {
        add_error(errors, "Accolade ouvrante manquante pour le corps de la fonction");
        return NULL;
    }

    // Analyse des déclarations et des instructions
    ast_list_t *stmts = analyze_statements(buffer, errors);
    if (!stmts) {
        add_error(errors, "Erreur lors de l'analyse des déclarations et instructions de la fonction");
        return NULL;
    }

    // Créer le nœud AST pour le bloc de la fonction
    ast_t *comp_stmt_ast = ast_new_comp_stmt(stmts);

    return comp_stmt_ast;
}


// Analyse des déclarations et instructions dans le corps de la fonction
ast_list_t *analyze_statements(buffer_t *buffer, error_list *errors) {
    ast_list_t *statements = NULL;

    // Analyser les instructions jusqu'à la parenthèse fermante '}'
    char next = lexer_getchar(buffer, errors);
    while (next != '}') {
        // Retourner le caractère pour pouvoir le réutiliser dans la logique suivante
        buf_rollback(buffer, 1);

        // Identifier le type de l'instruction (déclaration, condition, boucle, etc.)
        char *next_keyword = lexer_getalpha(buffer, errors);
        if (!next_keyword) {
            add_error(errors, "Mot-clé d'instruction invalide ou manquant");
            return NULL;
        }

        if (strcmp(next_keyword, "entier") == 0 || strcmp(next_keyword, "bool") == 0 ||
            strcmp(next_keyword, "rien") == 0) {
            // Analyser la déclaration de variable
            ast_t *declaration_ast = analyze_declaration(buffer, errors, next_keyword);
            if (!declaration_ast) {
                add_error(errors, "Erreur lors de l'analyse de la déclaration de variable");
                return NULL;
            }
            ast_list_add(&statements, declaration_ast);
        } else if (strcmp(next_keyword, "si") == 0) {
            // Analyser l'instruction conditionnelle
            ast_t *conditional_ast = analyze_conditional(buffer, errors, next_keyword);
            if (!conditional_ast) {
                add_error(errors, "Erreur lors de l'analyse de l'instruction conditionnelle");
                return NULL;
            }
            ast_list_add(&statements, conditional_ast);
        } else if (strcmp(next_keyword, "tantque") == 0) {
            // Analyser l'instruction de boucle
            ast_t *loop_ast = analyze_loop(buffer, errors);
            if (!loop_ast) {
                add_error(errors, "Erreur lors de l'analyse de l'instruction de boucle");
                return NULL;
            }
            ast_list_add(&statements, loop_ast);
        } else {
            add_error(errors, "Mot-clé d'instruction invalide");
            return NULL;
        }

        // Avancer pour le prochain caractère après l'instruction
        next = lexer_getchar(buffer, errors);
    }

    return statements;
}


// Analyse d'une déclaration de variable
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

    // Création du nœud AST pour la déclaration de variable
    ast_t *var_decl_ast = ast_new_declaration(ast_new_variable(var_name, type), NULL);

    // Vérifier s'il y a une initialisation de variable
    char *next = lexer_getoperator(buffer, errors);
    if (!next) {
        // Pas d'initialisation, rollback pour le caractère non consommé
        buf_rollback(buffer, 1);

        // Vérifier le point-virgule à la fin de la déclaration
        if (lexer_getchar(buffer, errors) != ';') {
            add_error(errors, "Point-virgule attendu à la fin de la déclaration de variable");
            return NULL;
        }
    } else if (strcmp(next, "=") == 0) {
        // Initialisation de la variable avec une valeur
        ast_t *value_ast = analyze_expression(buffer, errors);
        if (!value_ast) {
            add_error(errors, "Erreur lors de l'analyse de l'expression d'initialisation de variable");
            return NULL;
        }

        // Créer un nœud AST pour l'assignation de variable
        ast_t *expression = value_ast;

        // Mettre à jour le nœud AST de la déclaration de variable
        var_decl_ast->declaration.rvalue = expression;

        // Vérifier le point-virgule à la fin de la déclaration
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

// Analyse d'une instruction conditionnelle (si ... sinon si ... sinon ...)
ast_t *analyze_conditional(buffer_t *buffer, error_list *errors, char *next_keyword) {
    if (strcmp(next_keyword, "si") != 0) {
        add_error(errors, "Mot-clé \"si\" manquant pour l'instruction conditionnelle");
        return NULL;
    }

    // Vérifier la parenthèse ouvrante
    if (lexer_getchar(buffer, errors) != '(') {
        add_error(errors, "Parenthèse ouvrante manquante pour la condition de l'instruction conditionnelle");
        return NULL;
    }

    // Analyser la condition
    ast_t *condition = analyze_expression(buffer, errors);
    if (!condition) {
        add_error(errors, "Erreur lors de l'analyse de la condition de l'instruction conditionnelle");
        return NULL;
    }

    // Vérifier la parenthèse fermante après la condition
    if (lexer_getchar(buffer, errors) != ')') {
        add_error(errors, "Parenthèse fermante manquante après la condition de l'instruction conditionnelle");
        return NULL;
    }

    // Analyser le bloc d'instructions dans le cas vrai
    ast_t *true_stmt = analyze_function_body(buffer, errors);
    if (!true_stmt) {
        add_error(errors, "Erreur lors de l'analyse du bloc d'instructions du \"si\"");
        return NULL;
    }

    ast_t *current_conditional = ast_new_condition(condition, true_stmt, NULL);
    ast_t *last_conditional = current_conditional;

    // Analyser les éventuels "sinon si" et "sinon"
    while (true) {
        char *next_keyword = lexer_getalpha(buffer, errors);

        if (strcmp(next_keyword, "sinon") == 0) {
            char *peek_keyword = lexer_getalpha(buffer, errors);
            buf_rollback(buffer,
                         strlen(peek_keyword)); // Revenir en arrière pour analyser le prochain mot-clé correctement

            if (strcmp(peek_keyword, "si") == 0) {
                // Analyser la condition du "sinon si"
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

                // Analyser le bloc d'instructions du "sinon si"
                ast_t *true_stmt_elseif = analyze_function_body(buffer, errors);
                if (!true_stmt_elseif) {
                    add_error(errors, "Erreur lors de l'analyse du bloc d'instructions du \"sinon si\"");
                    return NULL;
                }

                last_conditional->branch.invalid = ast_new_condition(condition_elseif, true_stmt_elseif, NULL);
                last_conditional = last_conditional->branch.invalid;
            } else {
                // Analyser le bloc d'instructions du "sinon"
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


// Analyse d'une boucle (tantque)
ast_t *analyze_loop(buffer_t *buffer, error_list *errors) {
    if (strcmp(lexer_getalpha(buffer, errors), "tantque") != 0) {
        add_error(errors, "Mot-clé \"tantque\" manquant pour l'instruction de boucle");
        return NULL;
    }

    // Analyser la condition de la boucle
    ast_t *condition = analyze_expression(buffer, errors);
    if (!condition) {
        add_error(errors, "Erreur lors de l'analyse de la condition de l'instruction de boucle");
        return NULL;
    }

    // Analyser le corps de la boucle
    ast_t *body = analyze_function_body(buffer, errors);
    if (!body) {
        add_error(errors, "Erreur lors de l'analyse du bloc d'instructions de la boucle");
        return NULL;
    }

    // Créer le nœud AST pour la boucle tantque
    ast_t *loop_ast = ast_new_loop(condition, body);

    return loop_ast;
}

// Analyse d'une expression
ast_t *analyze_expression(buffer_t *buffer, error_list *errors) {
    string_stack_t operator_stack;
    string_stack_initialize(&operator_stack);

    ast_stack_t output_stack;
    ast_stack_initialize(&output_stack);

    while(true) {
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
