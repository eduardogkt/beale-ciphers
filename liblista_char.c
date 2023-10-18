#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "liblista_chave.h"
#include "liblista_char.h"

// cria um no de lista de char e o retorna
no_char_t *noCria(wchar_t c, int chave) {

    no_char_t *no = (no_char_t*) malloc(sizeof(no_char_t));
    if (no == NULL) return NULL;

    no->c = c;
    no->listaChaves = listaChaveCria();
    // criando um novo no de char
    listaChaveInsere(no->listaChaves, chave);
    no->prox = NULL;
    return no;
}

// cria uma lista de chars vazia e a retorna, se falhar, retorna NULL
lista_char_t *listaCria() {

    lista_char_t *lista = (lista_char_t*) malloc(sizeof(lista_char_t));
    if (lista == NULL) return NULL;

    lista->ini = NULL;
    lista->tamanho = 0;
    return lista;
}

// remove todos os chars da lista, libera espaco e retorna NULL
void listaDestroi(lista_char_t *lista) {

    no_char_t *no;
    while (!listaVazia(lista)) {
        no = lista->ini;
        lista->ini = lista->ini->prox;
        listaChaveDestroi(no->listaChaves);
        free(no);
        lista->tamanho--;
    }
    free(lista);
}

// imprime a lista de chars na stream informada
void listaImprime(lista_char_t *lista, FILE *stream) {

    if (listaVazia(lista))
        return;
    
    no_char_t *no = lista->ini;
    while (no != NULL) {
        fprintf(stream, "%lc: ", no->c);
        // imprmindo a lista de chaves do char
        listaChaveImprime(no->listaChaves, stream); 
        no = no->prox;
    }
}

// retorna 1 se a lista de chars estiver vazia e 0 caso contrario
int listaVazia(lista_char_t *lista) {

    return (lista->tamanho == 0);
}

// retorna o tamanho da lista de chars
int listaTamanho(lista_char_t *lista) {

    return lista->tamanho;
}

// cria um no de char e o insere em ordem (tabela ascii) na lista
// se o char ja existir na lista, adiciona a posicao a lista de chaves
// retorna 0 em caso de erro e 1 caso contrario
int listaInsere(lista_char_t *lista, wchar_t c, int chave) {
    
    no_char_t *no, *novoNo;

    // ja existe um no com esse char
    if (listaPertence(lista, c)) {
        listaInserePosicao(lista, c, chave);
        return 1;
    }

    novoNo = noCria(c, chave);
    if (novoNo == NULL) return 0;

    if (listaVazia(lista)) {
        lista->ini = novoNo;
        lista->tamanho++;
        return 1;
    }
    // é o menor que o início da lista
    if (novoNo->c <= lista->ini->c) { 
        novoNo->prox = lista->ini;
        lista->ini = novoNo;
    }
    else { // esta no meio ou no final
        no = lista->ini;
        while (no->prox != NULL && no->prox->c < novoNo->c)
            no = no->prox;
        
        novoNo->prox = no->prox;
        no->prox = novoNo;    
    }
    lista->tamanho++;
    return 1;
}

// insere uma nova posicao na lista de chaves do char
// retorna 0 em caso de erro e 1 caso contrario
int listaInserePosicao(lista_char_t *lista, wchar_t c, int chave) {
    
    no_char_t *no = lista->ini;
    while (no != NULL) {
        if (no->c == c) {
            // insirindo a chave na lista de chaves do char
            listaChaveInsere(no->listaChaves, chave);
            return 1;
        }
        no = no->prox;
    }
    return 0; // char nao encontrado
}

// remove o char caso ele exista na lista
// retorna 0 em caso de erro e 1 caso contrario
int listaRemoveChar(lista_char_t *lista, wchar_t c) {
    
    if (listaVazia(lista))
        return 0;

    no_char_t *anter, *atual;
    
    // o char esta no inicio
    if (lista->ini->c == c) {
        atual = lista->ini; 
        lista->ini = atual->prox;
        listaChaveDestroi(atual->listaChaves);
        free(atual);
        lista->tamanho--;
        return 1;
    }

    // o char esta no meio ou nao existe
    anter = lista->ini;
    atual = lista->ini->prox;

    while (atual != NULL) {
        if (atual->c == c) {
            anter->prox = atual->prox;
            listaChaveDestroi(atual->listaChaves);
            free(atual);
            lista->tamanho--;
            return 1;
        }
        anter = atual;
        atual = atual->prox;
    }
    return 0;  // atual eh nulo, char não encontado
}

// remove uma chave caso ela exista na lista de chaves do char
// retorna 0 em caso de erro e 1 caso contrario
int listaRemovePosicao(lista_char_t *lista, int chave) {

    no_char_t *no = lista->ini;
    while (no != NULL) {
        if (listaChaveRemove(no->listaChaves, chave)) {
            if (listaChaveVazia(no->listaChaves))
                listaRemoveChar(lista, no->c);
            return 1;
        }
        no = no->prox;
    }
    return 0; // elemento nao encontrado
}

// retorna o no do char se ele existir na lista e NULL caso contrario
no_char_t *listaPertence(lista_char_t *lista, wchar_t c) {

    no_char_t *no = lista->ini;
    while (no != NULL) {
        if (no->c == c)
            return no;
        no = no->prox;
    }
    return NULL; // char nao encontrado
}
