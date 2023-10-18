/*
 * biblioteca que implementa a lista de chaves (posicoes)
 * para as cifras de beale.
*/
#ifndef _LIBLISTA_CHAVE_H_
#define _LIBLISTA_CHAVE_H_

#include <stdio.h>

typedef struct no_chave {
    int chave;             // tamanho do vetor de chaves (posicoes)
    struct no_chave *prox; // ponteiro para o proximo
} no_chave_t;

typedef struct lista_chave {
    no_chave_t *ini; // ponteiro para o inicio da lista
    int tamanho;     // numero de elementos na lista
} lista_chave_t;

// cria uma lista de chaves vazia e a retorna, se falhar, retorna NULL
lista_chave_t *listaChaveCria();

// remove todas as chaves da lista, libera espaco e retorna NULL
void listaChaveDestroi(lista_chave_t *lista);

// imprime a lista de chaves na stream informada, se estiver vazia, nao imprime nada
void listaChaveImprime(lista_chave_t *lista, FILE *stream);

// retorna 1 se a lista de chaves estiver vazia e 0 caso contrario
int listaChaveVazia(lista_chave_t *lista);

// retorna o tamanho da lista de chaves
int listaChaveTamanho(lista_chave_t *lista);

// insere uma nova chave no inicio da lista
// retorna 0 em caso de erro e 1 caso contrario
int listaChaveInsere(lista_chave_t *lista, int chave);

// remove a chave caso ela exista na lista
// retorna 0 em caso de erro e 1 caso contrario
int listaChaveRemove(lista_chave_t *lista, int chave);

// retorna o no da chave se ela existir na lista e NULL caso contrario
no_chave_t *listaChavePertence(lista_chave_t *lista, int chave);

#endif // _LIBLISTA_CHAVE_H_
