#include <stdio.h>
#include <stdlib.h>
#include "liblista_chave.h"

// cria um no de lista de chave e o retorna
no_chave_t *noChaveCria(int chave) {

    no_chave_t *no = (no_chave_t*) malloc(sizeof(no_chave_t));
    if (no == NULL) return NULL;

    no->chave = chave;
    no->prox = NULL;
    return no;
}

// cria uma lista de chaves vazia e a retorna, se falhar, retorna NULL
lista_chave_t *listaChaveCria() {

    lista_chave_t *lista = (lista_chave_t*) malloc(sizeof(lista_chave_t));
    if (lista == NULL) return NULL;

    lista->ini = NULL;
    lista->tamanho = 0;
    return lista;
}

// remove todas as chaves da lista, libera espaco e retorna NULL
void listaChaveDestroi(lista_chave_t *lista) {

    no_chave_t *no;
    while (!listaChaveVazia(lista)) {
        no = lista->ini;
        lista->ini = lista->ini->prox;
        free(no);
        lista->tamanho--;
    }
    free(lista);
}

// imprime a lista de chaves na stream informada, se estiver vazia, nao imprime nada
void listaChaveImprime(lista_chave_t *lista, FILE *stream) {

    if (listaChaveVazia(lista))
        return;
    
    no_chave_t *no = lista->ini;
    while (no->prox != NULL) {
        fprintf(stream, "%d ", no->chave);
        no = no->prox;
    }
    fprintf(stream, "%d\n", no->chave);
}

// retorna 1 se a lista de chaves estiver vazia e 0 caso contrario
int listaChaveVazia(lista_chave_t *lista) {

    return (lista->tamanho == 0);
}

// retorna o tamanho da lista de chaves
int listaChaveTamanho(lista_chave_t *lista) {

    return lista->tamanho;
}

// insere uma nova chave no inicio da lista
// retorna 0 em caso de erro e 1 caso contrario
int listaChaveInsere(lista_chave_t *lista, int chave) {

    no_chave_t *novoNo = noChaveCria(chave);
    if (novoNo == NULL) return 0;
    
    novoNo->prox = lista->ini;
    lista->ini = novoNo;
    lista->tamanho++;
    return 1;
}

// remove a chave caso ela exista na lista
// retorna 0 em caso de erro e 1 caso contrario
int listaChaveRemove(lista_chave_t *lista, int chave) {
    
    if (listaChaveVazia(lista))
        return 0;

    no_chave_t *anter, *atual;
    
    // a chave esta no inicio
    if (lista->ini->chave == chave) {
        atual = lista->ini; 
        lista->ini = atual->prox;
        free(atual);
        lista->tamanho--;
        return 1;
    }

    // a chave esta no meio ou nao existe
    anter = lista->ini;
    atual = lista->ini->prox;

    while (atual != NULL) {
        if (atual->chave == chave) {
            anter->prox = atual->prox;
            free(atual);
            lista->tamanho--;
            return 1;
        }
        anter = atual;
        atual = atual->prox;
    }
    return 0;  // atual eh nulo, chave não encontada
}

// retorna o no da chave se ela existir na lista e NULL caso contrario
no_chave_t *listaChavePertence(lista_chave_t *lista, int chave) {

    no_chave_t *no = lista->ini;
    while (no != NULL) {
        if (no->chave == chave)
            return no;
        no = no->prox;
    }
    return NULL; // chave nao encontrada
}
