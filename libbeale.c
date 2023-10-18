#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include "libbeale_cod_dec.h"
#include "libbeale.h"

// set locale e seed da funcao aleatoria
void bealeInicializa() {

    setlocale(LC_ALL, "");
    srand(clock());
}

// cria e verifica a criacao de lista de cifras
lista_char_t *bealeCriaListaCifras() {
    
    lista_char_t *listaCifras;
    if (!(listaCifras = listaCria())) {
        fprintf(stderr, "Erro: nao foi possivel criar a lista de chars.\n");
        exit(BEALE_ERROR_LIST);
    }
    return listaCifras;
}

// retorna de qual arquivo as cifras deverao ser retiradas
int bealeGetSource(char *livro, char *chaves) {
    
    if (livro)  return BEALE_SOURCE_BOOK;
    if (chaves) return BEALE_SOURCE_KEYS;
    return BEALE_SOURCE_ERROR;
}

// le os argumetos de entrada e e verifica se estao corretos
void bealeLeEntrada(int argc, char **argv, char **msgcod, char **msgdec, 
                    char **chaves, char **livro, int *source, int *opcao) {

    opterr = FALSE; // desativa mansagens de erro do getopt

    *opcao = getopt(argc, argv, "ed");
    switch (*opcao) {
        // opcoes codificar e decodificar, aborta programa se ocorrer algum erro
        case 'e': 
            if (!leEntrada_cod(argc, argv, msgcod, msgdec, chaves, livro, source))
                exit(BEALE_ERROR_ENTRY);
            break;
        case 'd': 
            if (!leEntrada_dec(argc, argv, msgcod, msgdec, chaves, livro, source))
                exit(BEALE_ERROR_ENTRY);
            break;
        // entrada invalida
        default:
            fprintf(stderr, "Erro: entrada invalida, a primeira opcao deve ser -e ou -d\n");
            exit(BEALE_ERROR_ENTRY);
    }

    *source = bealeGetSource(*livro, *chaves);
    if (*source == BEALE_SOURCE_ERROR) {
        fprintf(stderr, "Erro: nao foi possivel determinar arquivo de origem.\n");
        exit(BEALE_ERROR_ENTRY);
    }
}

// garante que todas os caracteres ascii tenham pelo menos uma chave
void completaListaComAscii(lista_char_t *lista, unsigned int posicao) {

    for (wchar_t c = L' '; c <= L'~'; c++)
        listaInsere(lista, c, posicao++);
}

// gera as cifras de beale a partir do livro de cifras
void bealeGeraCifras_livro(lista_char_t *lista, char *livro_path) {
    
    FILE *file;
    if (!(file = fopen(livro_path, "r"))) {
        fprintf(stderr, "Erro: nao foi possivel abrir o livro cifra.\n");
        listaDestroi(lista);
        exit(BEALE_ERROR_FOPEN);
    }
    unsigned int posicao = 0;

    // string que recebera a palavra lida
    wchar_t *palavra = (wchar_t *) malloc(sizeof(wchar_t) * MAXSIZE);
    if (!palavra) {
        fprintf(stderr, "Erro: nao foi possivel alocar memoria.\n");
        listaDestroi(lista);
        exit(BEALE_ERROR_MALLOC);
    }
    
    // lendo palavra por palavra do arquivo e preenchendo a lista
    while (fscanf(file, "%ls", palavra) != EOF) {
        // padroniza a letra, retirando o acento e traduzindo ç para c
        wchar_t letraInicial = formatarLetra(palavra[0]);
        listaInsere(lista, letraInicial, posicao++);
    }
    free(palavra);
    fclose(file);

    // terminando de completar a lista de chars ascii
    completaListaComAscii(lista, posicao);
}

// gera as cifras de beale a partir do arquivo de chaves
void bealeGeraCifras_chaves(lista_char_t *lista, char *chaves_path) {
    
    FILE *file;
    if (!(file = fopen(chaves_path, "r"))) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo de chaves.\n");
        listaDestroi(lista);
        exit(BEALE_ERROR_FOPEN);
    }
    unsigned int posicao = 0;

    wchar_t c, buff;
    int num;
    // pegando o caractere
    fwscanf(file, L"%lc: ", &c);
    while (!feof(file)) {
        // pegando a lista de numeros e o char que vem depois
        fwscanf(file, L"%d", &num);
        buff = fgetwc(file);

        // se char apos o numero for \n, fim da lista de numeros
        while (!feof(file) && buff != L'\n') {
            listaInsere(lista, c, num);
            posicao++;
            fwscanf(file, L"%d", &num);
            buff = fgetwc(file);
        }
        listaInsere(lista, c, num);
        posicao++;

        fwscanf(file, L"%lc: ", &c);
    }
    fclose(file);

    // terminando de completar a lista de chars ascii
    completaListaComAscii(lista, posicao);
}

// gera o arquivos de chaves com as cifras de beale contidas na lista
void bealeGeraArquivoChaves(lista_char_t *lista, char *chaves_path) {
    
    FILE *arqchaves;
    // cria um novo arquivo, se arquivo ja existir, sobreescreve
    if (!(arqchaves = fopen(chaves_path, "w"))) {
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo de chaves.\n");
        listaDestroi(lista);
        exit(BEALE_ERROR_FOPEN);
    }
    // preenche o arquivo de chaves com o conteudo da lista de chars
    listaImprime(lista, arqchaves);
    fclose(arqchaves);
}

// abre os arquivos corretamente e codifica a mensagem
void bealeCodifica(lista_char_t *lista, char *msgorg_path, char *msgcod_path) {
    
    FILE *msgorg = NULL, *msgcod = NULL;

    // testa a abertura dos arquivos
    if (!abreArquivosBeale(&msgorg, msgorg_path, &msgcod, msgcod_path))
        exit(BEALE_ERROR_FOPEN);
        
    // lendo e codificando o arquivo
    codificaArquivoBeale(lista, msgorg, msgcod);

    fclose(msgorg);
    fclose(msgcod);
}

// abre os arquivos corretamente e decodifica a mensagem
void bealeDecodifica(lista_char_t *lista, char *msgcod_path, char *msgdec_path) {
    
    FILE *msgcod = NULL, *msgdec = NULL;

    // testa a abertura dos arquivos
    if (!abreArquivosBeale(&msgcod, msgcod_path, &msgdec, msgdec_path))
        exit(BEALE_ERROR_FOPEN);

    // lendo e decodificando o arquivo
    decodificaArquivoBeale(lista, msgcod, msgdec);
    
    fclose(msgcod);
    fclose(msgdec);
}
