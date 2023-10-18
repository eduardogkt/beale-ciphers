/*
 * biblioteca que implementa as estruturas e funcoes
 * necessarias para a codificacao e decodificacao 
 * das cifras de beale.
*/

#ifndef _LIBBEALE_COD_DEC_H_
#define _LIBBEALE_COD_DEC_H_

#include "liblista_char.h"

#ifndef TRUE
    #define TRUE 1
#endif
#ifndef FALSE
    #define FALSE 0
#endif

#define BEALE_KEY_SPACE -1    // codigo de char espaco ' '
#define BEALE_KEY_NLINE -2    // codigo de char nove linha '\n'
#define BEALE_ERROR_CHAR -3   // char nao encontrado na lista de chaves

// le a entrada dos parametros para a codificacao da cifras de beale
int leEntrada_cod(int argc, char **argv, char **msgcod, char **msgdec, 
                  char **chaves, char **livro, int *source);

// le a entrada dos parametros para a decodificacao da cifras de beale
int leEntrada_dec(int argc, char **argv, char **msgcod, char **msgdec, 
                  char **chaves, char **livro, int *source);

// retira o acento do caractere e traduz o cedilha para c
wchar_t formatarLetra(wchar_t c);

// abre os arquivos para codificacao/decodificacao
int abreArquivosBeale(FILE **file_r, char *filepath_r, FILE **file_w, char *filepath_w);

// le o arquivo codificado e o decodifica
void decodificaArquivoBeale(lista_char_t *lista, FILE *msgcod, FILE *msgdec);

// le o arquivo original e o codifica
void codificaArquivoBeale(lista_char_t *lista, FILE *msgorg, FILE *msgcod);

#endif // _LIBBEALE_COD_DEC_H_
