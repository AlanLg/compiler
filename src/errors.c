#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"

// Fonction pour initialiser la liste des erreurs
ErrorList* initErrorList() {
    ErrorList *list = (ErrorList *)malloc(sizeof(ErrorList));
    if (list == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(1);
    }
    list->head = NULL;
    list->count = 0;
    return list;
}

// Fonction pour ajouter une erreur à la liste
void addError(ErrorList *list, const char *message) {
    ErrorNode *newNode = (ErrorNode *)malloc(sizeof(ErrorNode));
    if (newNode == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        exit(1);
    }
    newNode->message = strdup(message); // Copie du message
    newNode->next = list->head; // Insertion en tête de liste
    list->head = newNode;
    list->count++;
}

// Fonction pour afficher toutes les erreurs
void printErrors(const ErrorList *list) {
    ErrorNode *current = list->head;
    if (current == NULL) {
        printf("Aucune erreur trouvée.\n");
        return;
    }
    printf("Liste des erreurs (%d):\n", list->count);
    while (current != NULL) {
        printf("- %s\n", current->message);
        current = current->next;
    }
}

// Fonction pour libérer la mémoire allouée à la liste des erreurs
void freeErrorList(ErrorList *list) {
    ErrorNode *current = list->head;
    while (current != NULL) {
        ErrorNode *temp = current;
        current = current->next;
        free(temp->message); // Libération du message
        free(temp); // Libération du nœud
    }
    free(list); // Libération de la structure de la liste
}
