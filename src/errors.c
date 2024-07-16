#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/errors.h"

error_list* init_errorList() {
    error_list *list = (error_list *)malloc(sizeof(error_list));
    if (list == NULL) {
        fprintf(stderr, "Erreur d'allocation de memoire.\n");
    }
    list->head = NULL;
    list->count = 0;
    return list;
}

void add_error(error_list *list, const char *message) {
    error_node *newNode = (error_node *)malloc(sizeof(error_node));
    if (newNode == NULL) {
        fprintf(stderr, "Erreur d'allocation de memoire.\n");
    }
    newNode->message = strdup(message);
    newNode->next = list->head;
    list->head = newNode;
    list->count++;
}

void print_errors(const error_list *list) {
    error_node *current = list->head;
    if (current == NULL) {
        printf("Aucune erreur trouvee.\n");
        return;
    }
    printf("Liste des erreurs (%d):\n", list->count);
    while (current != NULL) {
        printf("- %s\n", current->message);
        current = current->next;
    }
}

void free_errorList(error_list *list) {
    error_node *current = list->head;
    while (current != NULL) {
        error_node *temp = current;
        current = current->next;
        free(temp->message);
        free(temp);
    }
    free(list);
}
