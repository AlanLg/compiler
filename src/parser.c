#include "../include/parser.h"
#include <stdio.h>

symbol_t *table;

/*
  Début
   répéter
   analyse du lexème
   si le premier lexème n’est pas fonction,
   arrêter et retourner une erreur
   sinon
   appeler analyse_fonction()
   ajouter l’ast_t* retourné dans la liste des fonctions
   tant que le fichier n’est pas terminé
  Fin
*/
void parse(buffer_t *buffer) {
    if (!strcmp(lexer_getalphanum(buffer), "fonction")) {
        perror("first lexer was not \"fonction\"");
        exit(1);
    }

    ast_t *result = analyze_function(buffer);

}

/*
  Début
   analyse du lexème qui sera le nom de la fonction
   appel de analyse_paramètres()
   appel de analyse_type_de_retour()
   appel de analyse_corps_de_fonction()
   création de l’AST pour la fonction “main” avec les paramètres, type de retour et corps de
  fonction
   ajout du symbole correspondant au nom de la fonction dans la table des symboles
   retourner l’AST pour la fonction “main”
  Fin
*/
ast_t *analyze_function(buffer_t *buffer) {
    char *name = lexer_getalphanum(buffer);
    symbol_t *params = analyze_parameters(buffer);
    var_type_e return_type = analyze_return(buffer);
    symbol_t *stmts = analyze_function_body(buffer);

    ast_list_t *stmts_ast = NULL;
    symbol_t *temp_symb_stmts = stmts;
    while (temp_symb_stmts != NULL) {
        ast_list_add(&stmts_ast, temp_symb_stmts->ast_node);
        temp_symb_stmts = temp_symb_stmts->next;
    }

    ast_list_t *params_ast = NULL;
    symbol_t *temp_symb_params = params;
    while (temp_symb_params != NULL) {
        ast_list_add(&params_ast, temp_symb_params->ast_node);
        temp_symb_params = temp_symb_params->next;
    }

    ast_t *ast_function = ast_new_function(name, return_type, params_ast, stmts_ast);
    symbol_t *function_symbol = sym_new(name, SYM_FUNCTION, ast_function);
    function_symbol->function_table = params;
    sym_add(&function_symbol->function_table, stmts);
    sym_add(&table, function_symbol);

    return ast_function;
}


/*
  Début
    analyse du prochain symbole de ponctuation
    si ce n’est pas ‘(‘,
    produire une erreur
    répéter indéfiniment
    analyse du prochain lexème
    si ce n’est pas un type valide,
    produire une erreur
    analyse du prochain lexème, qui sera le nom du paramètre
    vérification de l’inexistance du lexème dans la table des symboles
    si le symbole n’existe pas, l’ajouter dans la table des symboles
    créer l’AST correspondant à la variable
    ajouter l’AST à la liste des paramètres
    si le prochain symbole de ponctuation est ‘,’, continuer
    sinon si c’est ‘)’, arrêter
    retourner la liste des paramètres
  Fin
*/
symbol_t *analyze_parameters(buffer_t *buffer) {
    printf("analyzing params");
    if (buf_getchar(buffer) != '(') {
        perror("missing ( for function parameters");
        exit(1);
    }

    char comma_or_parenthesis;

    printf("locking buffer");
    buf_lock(buffer);
    comma_or_parenthesis = buf_getchar(buffer);

    printf("checking if )");
    if (comma_or_parenthesis == ')') {
        return NULL;
    }

    printf("rollback");
    buf_rollback(buffer, 1);

    symbol_t *parameter_symbols = NULL;

    printf("starting do while");
    do {
        char *parameter_type_name = lexer_getalphanum(buffer);

        printf("type: %s", parameter_type_name);

        var_type_e parameter_type = get_var_type_from_string(parameter_type_name);
        if (parameter_type == UNKNOWN) {
          perror("parameter type is not valid");
          exit(1);
        }

        char *parameter_name = lexer_getalphanum(buffer);
        ast_t *ast = ast_new_variable(parameter_name, parameter_type);

        printf("name: %s", parameter_type_name);

        sym_add(&parameter_symbols, sym_new(parameter_name, SYM_PARAM, ast));

        if (sym_search(parameter_symbols, parameter_name) != NULL) {
          perror("parameter name is already taken");
          exit(1);
        }

        comma_or_parenthesis = buf_getchar(buffer);
    } while (comma_or_parenthesis == ',');

    if (comma_or_parenthesis != ')') {
      perror("missing ) at end of parameters");
      exit(1);
    }

    return parameter_symbols;
}

var_type_e analyze_return(buffer_t *buffer) {
    if (buf_getchar(buffer) != ':') {
        perror("missing : for function return type");
        exit(1);
    }
    var_type_e return_type = get_var_type_from_string(lexer_getalphanum(buffer));
    if (return_type == UNKNOWN) {
        perror("unknown type for function return type");
        exit(1);
    }
    return return_type;
}

symbol_t *analyze_function_body(buffer_t *buffer) {
}
