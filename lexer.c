#include "lexer.h"

size_t length_alphanum_word(buffer_t *buffer) {
  size_t result;
  char character;

  result = 0;
  character = buf_getchar(buffer);
  while (isalnum(character)) {
    result++;
    character = buf_getchar(buffer);
  }
  buf_rollback(buffer, result);
  return result;
}

size_t length_number_word(buffer_t *buffer) {
  size_t result;
  char character;

  result = 0;
  character = buf_getchar(buffer);
  if (character == OPERATOR_MINUS) {
    character = buf_getchar(buffer);
    if (!isdigit(character))
      return 0;
    result++;
  }
  while (isdigit(character)) {
    result++;
    character = buf_getchar(buffer);
  }
  buf_rollback(buffer, result);
  return result;
}

/**
 * Cette fonction doit retourner une chaîne de caractères si elle trouve une
 * suite de chiffres et/ou de lettres d’au moins 1 caractère. Si aucun caractère
 * lu ne correspond, le curseur doit retourner à sa position initiale Cette
 * fonction effectue un malloc pour créer la chaîne de caractères
 **/
char *lexer_getalphanum(buffer_t *buffer) {
  char *result;
  char character;

  // Lock le buffer pour lire les caractères
  buf_lock(buffer);

  // Skip les whites spaces
  size_t white_space = buf_skipblank(buffer);

  // On lit chaque caractère jusqu'à atteindre un caractère non alphanumérique
  // pour déterminer la taille de la chaine de caractère
  size_t length = length_alphanum_word(buffer);

  // On crée la chaîne de caractère
  if (length) {
    result = malloc(length + 1);
    if (!result) {
      buf_rollback(buffer, white_space);
      buf_unlock(buffer);
      return NULL;
    }
    size_t i = 0;
    while (i < length) {
      character = buf_getchar(buffer);
      result[i++] = character;
    }
    result[length] = EOF;
  } else
    return NULL;

  // Libérer le buffer
  buf_unlock(buffer);

  return result;
}

/**
 * Meme principe que lexer_getalphanum, mais le curseur doit revenir à sa
 *position initiale systématiquement.
 **/
char *lexer_getalphanum_rollback(buffer_t *buffer) {
  size_t length;
  char *result;
  char character;
  size_t i;
  size_t white_space;

  // Lock le buffer pour lire les caractères
  buf_lock(buffer);

  // Skip les whites spaces
  white_space = buf_skipblank(buffer);

  // On lit chaque caractère jusqu'à atteindre un caractère non alphanumérique
  // pour déterminer la taille de la chaine de caractère
  length = length_alphanum_word(buffer);

  // On crée la chaîne de caractère
  if (length) {
    result = malloc(length + 1);
    if (!result) {
      buf_rollback(buffer, white_space);
      buf_unlock(buffer);
      return NULL;
    }
    i = 0;
    while (i < length) {
      character = buf_getchar(buffer);
      result[i++] = character;
    }
    result[length] = EOF;
  } else
    return NULL;

  // On rollback a la position initiale
  buf_rollback(buffer, white_space + length);

  // Libérer le buffer
  buf_unlock(buffer);

  return result;
}

/**
 * Cette fonction doit retourner une chaîne de caractères qui correspond à un
 * opérateur de votre langage (unaire ou binaire typiquement)
 **/
char *lexer_getop(buffer_t *buffer) {
  char *result;
  int i;
  char character;
  size_t white_space;

  // Lock le buffer pour lire les caractères
  buf_lock(buffer);

  // Skip les whites spaces
  white_space = buf_skipblank(buffer);

  // Créer la chaîne de caractères pour l'opérateur
  result = malloc(SIZE_MAX_OPERATOR + 1);
  if (!result) {
    buf_rollback(buffer, white_space);
    buf_unlock(buffer);
    return NULL;
  }

  // Lire les caractères de l'opérateur
  character = buf_getchar(buffer);
  if (character == OPERATOR_PLUS || character == OPERATOR_MINUS ||
      character == OPERATOR_MULT || character == OPERATOR_DIV ||
      character == OPERATOR_EQUAL || character == OPERATOR_SUP ||
      character == OPERATOR_INF) {
    i = 0;
    result[i++] = character;
  } else {
    buf_rollback_and_unlock(buffer, i + white_space);
    free(result);
    return NULL;
  }
  character = buf_getchar(buffer);
  if ((result[0] == OPERATOR_EQUAL && character == OPERATOR_EQUAL) ||
      (result[0] == OPERATOR_NEGATION && character == OPERATOR_EQUAL) ||
      (result[0] == OPERATOR_INF && character == OPERATOR_EQUAL) ||
      (result[0] == OPERATOR_SUP && character == OPERATOR_EQUAL))
    result[i++] = character;
  else
    buf_rollback(buffer, 1);

  // Finaliser la chaine de caractère de l'opérateur et libérer le buffer
  result[i] = EOF;
  buf_unlock(buffer);

  return result;
}

/**
 * Cette fonction doit lire des caractères tant que ce sont des chiffres (le
 *nombre peut commencer par le symbole “-”) Elle retourne un long qui est la
 *valeur numérique du nombre lu Appuyez-vous sur strtol()
 **/
long lexer_getnumber(buffer_t *buffer) {

  size_t length;
  char *number_str;
  char character;
  size_t i;
  long result;
  size_t white_space;

  // Lock le buffer pour lire les caractères
  buf_lock(buffer);

  // Skip les whites spaces
  white_space = buf_skipblank(buffer);

  // On lit chaque caractère jusqu'à atteindre la fin du nombre
  length = length_number_word(buffer);

  if (length) {
    number_str = malloc(length + 1);
    if (!number_str) {
      buf_rollback(buffer, white_space);
      buf_unlock(buffer);
      return 0;
    }

    i = 0;

    // Vérifier si le nombre est négatif
    character = buf_getchar(buffer);
    if (character == OPERATOR_MINUS) {
      number_str[i++] = character;
      character = buf_getchar(buffer);
    }

    while (isdigit(character)) {
      number_str[i++] = character;
      character = buf_getchar(buffer);
    }

    number_str[length] = EOF;

    // Revenir au dernier caractère non numérique lu
    buf_rollback(buffer, 1);
  } else {
    buf_rollback(buffer, white_space);
    return 0;
  }

  result = strtol(number_str, NULL, 10);
  free(number_str);

  // Déverrouiller le buffer
  buf_unlock(buffer);

  return result;
}
