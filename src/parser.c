#include "../include/parser.h"

symbol_t **table;

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
    ast_list_t *params = analyze_parameters(buffer);
    var_type_e return_type = analyze_return(buffer);
    ast_list_t *stmts = analyze_function_body(buffer);
    ast_t *ast_function = ast_new_function(name, return_type, params, stmts);
    symbol_t *function_symbol = sym_new(name, SYM_FUNCTION, ast_function);
    sym_add(table, function_symbol);
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
ast_list_t *analyze_parameters(buffer_t *buffer) {
    if (buf_getchar(buffer) != '(') {
        perror("missing ( for function parameters");
        exit(1);
    }
    if (lexer_getalphanum(buffer))
        return ast_list_new_node(ast_new_variable(lexer_getalphanum(buffer), VAR_INTEGER));
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

ast_list_t *analyze_function_body(buffer_t *buffer) {
}
