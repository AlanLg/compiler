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
            ast_t *conditional_ast = analyze_conditional(buffer, errors);
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
        ast_t *assign_ast = ast_new_assignment(ast_new_variable(var_name, type), value_ast);

        // Mettre à jour le nœud AST de la déclaration de variable
        var_decl_ast->declaration.rvalue = assign_ast;

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
ast_t *analyze_conditional(buffer_t *buffer, error_list *errors) {
    if (strcmp(lexer_getalpha(buffer, errors), "si") != 0) {
        add_error(errors, "Mot-clé \"si\" manquant pour l'instruction conditionnelle");
        return NULL;
    }

    // Analyser la condition
    ast_t *condition = analyze_expression(buffer, errors);
    if (!condition) {
        add_error(errors, "Erreur lors de l'analyse de la condition de l'instruction conditionnelle");
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
            // Analyser le bloc d'instructions du "sinon"
            ast_t *false_stmt = analyze_function_body(buffer, errors);
            if (!false_stmt) {
                add_error(errors, "Erreur lors de l'analyse du bloc d'instructions du \"sinon\"");
                return NULL;
            }

            last_conditional->branch.invalid = ast_new_condition(NULL, false_stmt, NULL);
            last_conditional = last_conditional->branch.invalid;
        } else if (strcmp(next_keyword, "sinon") == 0 && strcmp(lexer_getalpha(buffer, errors), "si") == 0) {
            // Analyser la condition du "sinon si"
            ast_t *condition_elseif = analyze_expression(buffer, errors);
            if (!condition_elseif) {
                add_error(errors, "Erreur lors de l'analyse de la condition du \"sinon si\"");
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
            buf_rollback(buffer, strlen(next_keyword));
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
    // Ici, vous devrez implémenter l'analyse des expressions comme les opérations arithmétiques, les appels de fonctions, etc.
    // Cette partie dépendra de la syntaxe et de la sémantique précises de votre langage Intech.
    // Vous pouvez utiliser ast_new_binary, ast_new_unary, ast_new_variable, etc., pour créer les nœuds AST appropriés.
    // Pour l'instant, je vais laisser cette partie comme étant une fonction factice.

    // Exemple factice :
    ast_t *dummy_expr = ast_new_integer(42);
    return dummy_expr;
}

