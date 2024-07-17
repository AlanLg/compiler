# COMPILER

## Langages Utilisés

- **Langage Source** : Intech
- **Langage Cible** : Java
- **Langage d'Implémentation** : **C**.
  
## Fonctionnalités

- **Analyse lexicale**
- **Analyse syntaxique**
- **Gestion des erreurs**
- **Support des fonctions**
- **Support des structures de contrôle**

### Fonctionnalités Implémentées

- **Lexer** : Analyse lexicale complète pour identifier les mots-clés, les identificateurs, les nombres, et les symboles.
- **Parser** : Analyse syntaxique pour vérifier la structure des programmes, y compris les déclarations de fonctions, les paramètres, les types de retour, les corps de fonctions, les déclarations et les affectations de variables, les branches conditionnelles (`si`) et la déclaration de variable.
- **AST** : Génération de l'arbre de syntaxe abstraite pour représenter la structure du programme de manière arborescente.
- **Gestion des Erreurs** : Système complet de gestion des erreurs qui détecte et rapporte les erreurs de syntaxe et de structure, avec des messages d'erreur clairs et précis.

## Compilation et Exécution

### Utilisation de GCC

1. **Compilation** :
   ```sh
   gcc -o compiler src/main.c src/buffer.c src/lexer.c src/parser.c src/ast.c src/symbol.c src/errors.c -Iinclude

2. **Exécution** :
   ```sh
   ./compiler tests/test.intech

### Utilisation de Makefile

1. **Compilation** :
   ```sh
   make

2. **Exécution** :
   ```sh
   ./compiler tests/test.intech

## Exemple de Fichier Source

Voici un exemple de fichier source (test.intech) que vous pouvez utiliser pour tester le compilateur :

   ```plaintext
   fonction main {
       int x = 10;
       si (x > 5) {
           x = x + 1;
       }
       tantque (x < 15) {
           x = x - 1;
       }
   }
   ```


## Gestion des Erreurs

Le compilateur inclut un système de gestion des erreurs qui détecte et rapporte les erreurs de syntaxe et de structure. Les erreurs sont ajoutées à une liste et affichées à la fin de l'analyse.


## Auteurs

- **Alan LE GALL**
- **Gregory POTES GONZALEZ**
- **Jeremie KUNKELA**
- **Steven TERNEUS**





