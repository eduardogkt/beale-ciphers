/*
 * biblioteca que implementa a lista de caracteres
 * para as cifras de beale.
*/
#ifndef _LIBLISTA_CHAR_H_
#define _LIBLISTA_CHAR_H_

#include <stdio.h>
#include <wchar.h>
#include "liblista_chave.h"

typedef struct no_char {
    wchar_t c;                  // caractere inicial
    lista_chave_t *listaChaves; // lista de chaves (posicoes)
    struct no_char *prox;       // ponteiro para o proximo
} no_char_t;

typedef struct lista_char {
    no_char_t *ini;   // ponteiro para o inicio da lista
    int tamanho; // numero de elementos na lista
} lista_char_t;

// cria uma lista de chars vazia e a retorna, se falhar, retorna NULL
lista_char_t *listaCria();

// remove todos os chars da lista, libera espaco e retorna NULL
void listaDestroi(lista_char_t *lista);

// imprime a lista de chars na stream informada
void listaImprime(lista_char_t *lista, FILE *stream);

// retorna 1 se a lista de chars estiver vazia e 0 caso contrario
int listaVazia(lista_char_t *lista);

// retorna o tamanho da lista de chars
int listaTamanho(lista_char_t *lista);

// cria um no de char e o insere em ordem (tabela ascii) na lista
// se o char ja existir na lista, adiciona a posicao a lista de chaves
// retorna 0 em caso de erro e 1 caso contrario
int listaInsere(lista_char_t *lista, wchar_t c, int chave);

// insere uma nova posicao na lista de chaves do char
// retorna 0 em caso de erro e 1 caso contrario
int listaInserePosicao(lista_char_t *lista, wchar_t c, int chave);

// remove o char caso ele exista na lista
// retorna 0 em caso de erro e 1 caso contrario
int listaRemoveChar(lista_char_t *lista, wchar_t c);

// remove uma chave caso ela exista na lista de chaves do char
// retorna 0 em caso de erro e 1 caso contrario
int listaRemovePosicao(lista_char_t *lista, int chave);

// retorna o no do char se ele existir na lista e NULL caso contrario
no_char_t *listaPertence(lista_char_t *lista, wchar_t c);

#endif // _LIBLISTA_CHAR_H_
