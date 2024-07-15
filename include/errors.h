#ifndef ERRORS_H
#define ERRORS_H

typedef struct error_node {
    char *message;
    struct error_node *next;
} error_node;

typedef struct error_list {
    error_node *head;
    int count;
} error_list;

error_list* init_errorList();

void add_error(error_list *list, const char *message);

void print_errors(const error_list *list);

// Fonction pour libérer la mémoire allouée à la liste des erreurs
void free_errorList(error_list *list);

#endif // ERRORS_H
