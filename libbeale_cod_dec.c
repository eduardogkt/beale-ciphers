#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "libbeale_cod_dec.h"

// le a entrada dos parametros para a codificacao da cifras de beale
int leEntrada_cod(int argc, char **argv, char **msgcod, char **msgdec, 
                  char **chaves, char **livro, int *source) {
    
    opterr = FALSE;
    int argcount = 0;
    int opt;
    while ((opt = getopt(argc, argv, "b:m:o:c:")) != -1) {
        switch (opt) {
            case 'b': *livro  = optarg; break;
            case 'm': *msgdec = optarg; break;
            case 'o': *msgcod = optarg; break;
            case 'c': *chaves = optarg; break;
            default:
                if (strchr("bmoc", opt)) // se for alguas das opcoes validas
					fprintf(stderr, "Erro: opcao necessita de argumento.\n");
				else
                    fprintf(stderr, "Erro: opcao de entrada invalida.\n");
				return FALSE;
        }
        if (++argcount > 4) {
            fprintf(stderr, "Erro: argumentos demais.\n");
            return FALSE;
        }
    }
    // se algum dos parametos for nulo, faltou parametros de entrada
    if (!*livro || !*msgcod  || !*msgdec || !*chaves) {
        fprintf(stderr, "Erro: muito pouco argumentos.\n");
        return FALSE;
    }
    return TRUE;
}

// le a entrada dos parametros para a decodificacao da cifras de beale
int leEntrada_dec(int argc, char **argv, char **msgcod, char **msgdec, 
                  char **chaves, char **livro, int *source) {

    opterr = FALSE;
    int argcount = 0;
    int opt;
    while ((opt = getopt(argc, argv, "b:i:o:c:")) != -1) {
        switch (opt) {
            case 'b': *livro  = optarg; break;
            case 'i': *msgcod = optarg; break;
            case 'o': *msgdec = optarg; break;
            case 'c': *chaves = optarg; break;
            default:
                if (strchr("bioc", opt)) // se for alguas das opcoes validas
					fprintf(stderr, "Erro: opcao necessita de argumento.\n");
				else
                    fprintf(stderr, "Erro: opcao de entrada invalida.\n");
				return FALSE;
        }
        if (++argcount > 3) {
            fprintf(stderr, "Erro: argumentos demais.\n");
            return FALSE;
        }
    }
    // se opcao livro e chaves forem usadas simultaneamente
    if (*livro && *chaves) {
        fprintf(stderr, "Erro: entrada invalida, use -b OU -c.\n");
        return FALSE;
    }
    // se algum dos parametos for nulo, faltou parametros de entrada
    if ((!*chaves && !*livro) || !*msgcod || !*msgdec) {
        fprintf(stderr, "Erro: muito pouco argumentos.\n");
        return FALSE;
    }
    return TRUE;
}

// abre os arquivos para codificacao/decodificacao
int abreArquivosBeale(FILE **file_r, char *filepath_r, FILE **file_w, char *filepath_w) {

    // erro ao abrir arquivo a ser lido
    if (!(*file_r = fopen(filepath_r, "r"))) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo a ser lido.\n");
        return FALSE;
    }
    // arquivo de saida ja existe
    if ((*file_w = fopen(filepath_w, "r"))) {
        fprintf(stderr, "Erro: arquivo de saida ja existe.\n");
        fclose(*file_r);
        fclose(*file_w);
        return FALSE;
    }
    // erro ao criar arquivo de saida
    if (!(*file_w = fopen(filepath_w, "w"))) {
        fprintf(stderr, "Erro: nao foi possivel criar o arquivo de saida.\n");
        return FALSE;
    }
    return TRUE;
}

// retira o acento do caractere c
wchar_t formatarLetra(wchar_t c) {

    if (wcschr(L"áàãâ", c)) return L'a';
    if (wcschr(L"ÁÀÃÂ", c)) return L'A';
    if (wcschr(L"éèẽê", c)) return L'e';
    if (wcschr(L"ÉÈẼÊ", c)) return L'E';
    if (wcschr(L"íìĩî", c)) return L'i';
    if (wcschr(L"ÍÌĨÎ", c)) return L'I';
    if (wcschr(L"óòõô", c)) return L'o';
    if (wcschr(L"ÓÒÕÔ", c)) return L'O';
    if (wcschr(L"úùũû", c)) return L'u';
    if (wcschr(L"ÚÙŨÛ", c)) return L'U';
    if (wcschr(L"çÇ",   c)) return L'c';
    return c;
}

// retorna um numero aleatorio no intervalo (min, max)
int randRange(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

// retorna uma chave aleatoria da lista de chaves do char c
int procuraChave(lista_char_t *lista, wchar_t c) {
    
    if (c == L' ') return BEALE_KEY_SPACE;
    if (c == L'\n') return BEALE_KEY_NLINE;

    no_char_t *noChar;
    // idealmente, nunca deve ocorrer pra arquivos ascii
    if (!(noChar = listaPertence(lista, c)))
        return BEALE_ERROR_CHAR;

    int tamListaChaves = noChar->listaChaves->tamanho;
    int indiceChave = randRange(1, tamListaChaves);

    // percorrendo a lista de chaves ate o indice sorteado
    no_chave_t *noChave = noChar->listaChaves->ini;
    for (int i = 1; i < indiceChave; i++)
        noChave = noChave->prox;

    return noChave->chave;
}

// procura o char correspondente aa chave
wchar_t procuraChar(lista_char_t *lista, int chave) {
    
    if (chave == BEALE_KEY_SPACE) return L' ';
    if (chave == BEALE_KEY_NLINE) return L'\n';

    no_char_t *no = lista->ini;
    while (no != NULL) {
        lista_chave_t *listaChaves = no->listaChaves;
        if (listaChavePertence(listaChaves, chave))
            return no->c;
        no = no->prox;
    }
    // chave nao encontrada
    // idealmente nunca ocorre ao decodificar textos ascii
    return BEALE_ERROR_CHAR;
}

// le o arquivo codificado e o decodifica
void decodificaArquivoBeale(lista_char_t *lista, FILE *msgcod, FILE *msgdec) {

    int chave;
    wchar_t c;
    // lendo chave por chave do arquivo codificado
    fwscanf(msgcod, L"%d", &chave);
    while(!feof(msgcod)) {
        c = procuraChar(lista, chave);
        fwprintf(msgdec, L"%lc", c);
        
        fwscanf(msgcod, L"%d ", &chave);
    }
    c = procuraChar(lista, chave);
    fwprintf(msgdec, L"%lc", c);
}

// le o arquivo original e o codifica
void codificaArquivoBeale(lista_char_t *lista, FILE *msgorg, FILE *msgcod) {

    int chave;
    wchar_t c;
    // lendo letra por letra do arquivo original
    while((c = fgetwc(msgorg)) != EOF) {
        c = formatarLetra(c);
        chave = procuraChave(lista, c);
        
        fwprintf(msgcod, L"%d ", chave);
        if (chave == BEALE_KEY_NLINE)
            fwprintf(msgcod, L"\n");
    }
}
