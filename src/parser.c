#include "../include/parser.h"

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
      return ast_new_function(lexer_getalphanum(buffer), analyze_return(buffer), analyze_parameters(buffer),
                              analyze_function_body(buffer));
}

ast_list_t *analyze_parameters(buffer_t *buffer) {

}

var_type_e analyze_return(buffer_t *buffer) {

}

ast_list_t *analyze_function_body(buffer_t *buffer) {

}
