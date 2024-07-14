#ifndef ERRORS_H
#define ERRORS_H

typedef struct ErrorNode {
    char *message;
    struct ErrorNode *next;
} ErrorNode;

typedef struct ErrorList {
    ErrorNode *head;
    int count;
} ErrorList;

// Fonction pour initialiser la liste des erreurs
ErrorList* initErrorList();

// Fonction pour ajouter une erreur à la liste
void addError(ErrorList *list, const char *message);

// Fonction pour afficher toutes les erreurs
void printErrors(const ErrorList *list);

// Fonction pour libérer la mémoire allouée à la liste des erreurs
void freeErrorList(ErrorList *list);

#endif // ERRORS_H
