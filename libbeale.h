/*
 * biblioteca que implementa as estruturas e funcoes
 * necessarias para leitura de entrada, criacao de 
 * arquivo de chaves e funcionamento da codificacao 
 * e decodificacao das cifras de beale.
 */

#ifndef _LIBBEALE_H_
#define _LIBBEALE_H_

#include "liblista_char.h"

#ifndef TRUE
    #define TRUE 1
#endif
#ifndef FALSE
    #define FALSE 0
#endif

#define MAXSIZE 512

#define BEALE_SOURCE_ERROR 0 // arquivo de origem invalido
#define BEALE_SOURCE_BOOK 1  // cifras criadas a partir de livro
#define BEALE_SOURCE_KEYS 2  // cifras criadas a partir de arquivo de chaves

#define BEALE_ENCODE 'e' // codificar
#define BEALE_DECODE 'd' // decodificar

#define BEALE_ERROR_MALLOC -4 // erro de alocacao de memoria
#define BEALE_ERROR_FOPEN -5  // erro ao abrir arquivo
#define BEALE_ERROR_ENTRY -6  // erro ao ler entrada do programa
#define BEALE_ERROR_LIST -7   // erro ao criar lista de cifras

// set locale e seed da funcao aleatoria
void bealeInicializa();

// cria e verifica a criacao de lista de cifras
lista_char_t *bealeCriaListaCifras();

// le os argumetos de entrada e e verifica se estao corretos
void bealeLeEntrada(int argc, char **argv, char **msgcod, char **msgdec, 
                    char **livro, char **chaves, int *source, int *opcao);

// gera as cifras de beale a partir do livro de cifras
void bealeGeraCifras_livro(lista_char_t *lista, char *livro_path);

// gera as cifras de beale a partir do arquivo de chaves
void bealeGeraCifras_chaves(lista_char_t *lista, char *chaves_path);

// gera o arquivos de chaves com as cifras de beale contidas na lista
void bealeGeraArquivoChaves(lista_char_t *lista, char *chaves_path);

// abre os arquivos corretamente e codifica a mensagem
void bealeCodifica(lista_char_t *lista, char *msgorg_path, char *msgcod_path);

// abre os arquivos corretamente e decodifica a mensagem
void bealeDecodifica(lista_char_t *lista, char *msgorg_path, char *msgdec_path);

#endif // _LIBBEALE_H_
