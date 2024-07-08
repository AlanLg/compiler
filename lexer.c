#include "lexer.h"

size_t length_alphanum_word(buffer_t *buffer) {
  size_t res;
  char c;

  res = 0;
  c = buf_getchar(buffer);
  while (isalnum(c)) {
    res++;
    c = buf_getchar(buffer);
  }
  buf_rollback(buffer, res);
  return res;
}

size_t length_number_word(buffer_t *buffer) {
  size_t res;
  char c;

  res = 0;
  c = buf_getchar(buffer);
  if (c == OPERATOR_MINUS) {
    c = buf_getchar(buffer);
    if (!isdigit(c))
      return 0;
    res++;
  }
  while (isdigit(c)) {
    res++;
    c = buf_getchar(buffer);
  }
  buf_rollback(buffer, res);
  return res;
}

/**
 * Cette fonction doit retourner une chaîne de caractères si elle trouve une
 *suite de chiffres et/ou de lettres d’au moins 1 caractère. Si aucun caractère
 *lu ne correspond, le curseur doit retourner à sa position initiale Cette
 *fonction effectue un malloc pour créer la chaîne de caractères
 **/
char *lexer_getalphanum(buffer_t *buffer) {
  size_t length;
  char *res;
  char c;
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
    res = malloc(length + 1);
    if (!res) {
      buf_rollback(buffer, white_space);
      buf_unlock(buffer);
      return NULL;
    }
    i = 0;
    while (i < length) {
      c = buf_getchar(buffer);
      res[i++] = c;
    }
    res[length] = EOF;
  } else
    return NULL;

  // Libérer le buffer
  buf_unlock(buffer);

  return res;
}

/**
 * Meme principe que lexer_getalphanum, mais le curseur doit revenir à sa
 *position initiale systématiquement.
 **/
char *lexer_getalphanum_rollback(buffer_t *buffer) {
  size_t length;
  char *res;
  char c;
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
    res = malloc(length + 1);
    if (!res) {
      buf_rollback(buffer, white_space);
      buf_unlock(buffer);
      return NULL;
    }
    i = 0;
    while (i < length) {
      c = buf_getchar(buffer);
      res[i++] = c;
    }
    res[length] = EOF;
  } else
    return NULL;

  // On rollback a la position initiale
  buf_rollback(buffer, white_space + length);

  // Libérer le buffer
  buf_unlock(buffer);

  return res;
}

/**
 * Cette fonction doit retourner une chaîne de caractères qui correspond à un
 * opérateur de votre langage (unaire ou binaire typiquement)
 **/
char *lexer_getop(buffer_t *buffer) {
  char *res;
  int i;
  char c;
  size_t white_space;

  // Lock le buffer pour lire les caractères
  buf_lock(buffer);

  // Skip les whites spaces
  white_space = buf_skipblank(buffer);

  // Créer la chaîne de caractères pour l'opérateur
  res = malloc(SIZE_MAX_OPERATOR + 1);
  if (!res) {
    buf_rollback(buffer, white_space);
    buf_unlock(buffer);
    return NULL;
  }

  // Lire les caractères de l'opérateur
  c = buf_getchar(buffer);
  if (c == OPERATOR_PLUS || c == OPERATOR_MINUS || c == OPERATOR_MULT ||
      c == OPERATOR_DIV || c == OPERATOR_EQUAL || c == OPERATOR_SUP ||
      c == OPERATOR_INF) {
    i = 0;
    res[i++] = c;
  } else {
    buf_rollback_and_unlock(buffer, i + white_space);
    free(res);
    return NULL;
  }
  c = buf_getchar(buffer);
  if ((res[0] == OPERATOR_EQUAL && c == OPERATOR_EQUAL) ||
      (res[0] == OPERATOR_NEGATION && c == OPERATOR_EQUAL) ||
      (res[0] == OPERATOR_INF && c == OPERATOR_EQUAL) ||
      (res[0] == OPERATOR_SUP && c == OPERATOR_EQUAL))
    res[i++] = c;
  else
    buf_rollback(buffer, 1);

  // Finaliser la chaine de caractère de l'opérateur et libérer le buffer
  res[i] = EOF;
  buf_unlock(buffer);

  return res;
}

/**
 * Cette fonction doit lire des caractères tant que ce sont des chiffres (le
 *nombre peut commencer par le symbole “-”) Elle retourne un long qui est la
 *valeur numérique du nombre lu Appuyez-vous sur strtol()
 **/
long lexer_getnumber(buffer_t *buffer) {

  size_t length;
  char *number_str;
  char c;
  size_t i;
  long res;
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
    c = buf_getchar(buffer);
    if (c == OPERATOR_MINUS) {
      number_str[i++] = c;
      c = buf_getchar(buffer);
    }

    while (isdigit(c)) {
      number_str[i++] = c;
      c = buf_getchar(buffer);
    }

    number_str[length] = EOF;

    // Revenir au dernier caractère non numérique lu
    buf_rollback(buffer, 1);
  } else {
    buf_rollback(buffer, white_space);
    return 0;
  }

  res = strtol(number_str, NULL, 10);
  free(number_str);

  // Déverrouiller le buffer
  buf_unlock(buffer);

  return res;
}
