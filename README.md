# COMPILER


## Description

Ce projet est un compilateur simple conçu pour analyser et exécuter un langage de programmation spécifique. Le compilateur lit un fichier source, analyse sa syntaxe et sa structure, et génère un arbre de syntaxe abstraite (AST) pour représenter le programme. Ce projet comprend les composants suivants :

- **Lexer** : Analyse lexicale pour découper le texte source en tokens.
- **Parser** : Analyse syntaxique pour vérifier la structure grammaticale du programme.
- **AST (Abstract Syntax Tree)** : Représentation arborescente des instructions du programme.
- **Buffer** : Gestion du tampon pour la lecture efficace des fichiers.


## Langages Utilisés

- **Langage Source** : Intech
- **Langage Cible** : Java
- **Langage d'Implémentation** : **C**.
  

## Fonctionnalités

- **Analyse lexicale** : Transformation du texte source en tokens.
- **Analyse syntaxique** : Validation de la structure du programme et génération de l'AST.
- **Gestion des erreurs** : Détection et rapport des erreurs de syntaxe et de structure.
- **Support des fonctions** : Analyse des déclarations et des corps de fonctions.
- **Support des structures de contrôle** : Analyse des branches conditionnelles et des boucles.

### Fonctionnalités Implémentées

- **Lexer** : Analyse lexicale complète pour identifier les mots-clés, les identificateurs, les nombres, et les symboles.
- **Parser** : Analyse syntaxique pour vérifier la structure des programmes, y compris les déclarations de fonctions, les paramètres, les types de retour, les corps de fonctions, les déclarations et les affectations de variables, les branches conditionnelles (`si`) et les boucles (`tantque`).
- **AST** : Génération de l'arbre de syntaxe abstraite pour représenter la structure du programme de manière arborescente.
- **Gestion des Erreurs** : Système complet de gestion des erreurs qui détecte et rapporte les erreurs de syntaxe et de structure, avec des messages d'erreur clairs et précis.
  

## Structure du Projet

Le projet est organisé de la manière suivante :

```
project_root/
│
├── include/ # Contient les fichiers d'en-tête
│ ├── ast.h
│ ├── buffer.h
│ ├── lexer.h
│ ├── parser.h
│ ├── symbols.h
│ └── errors.h
│
├── src/ # Contient les fichiers source
│ ├── main.c
│ ├── ast.c
│ ├── buffer.c
│ ├── lexer.c
│ ├── parser.c
│ ├── symbols.c
│ └── errors.c
│
├── tests/ # Contient les fichiers de test
│ └── test.intech
│
└── Makefile # Script de compilation Makefile
```

## Prérequis

- Un compilateur C (par exemple, `gcc`).
- CMake (optionnel, si vous utilisez un fichier CMake pour la compilation).


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





