# Projeto 2 Programação II (CI1002)
Eduardo Gabriel Kenzo Tanaka (GRR20211791)

## Cifras de Beale
 Programa para codificação e decodificação de textos utilizando as [Cifras de
Beale](https://en.wikipedia.org/wiki/Beale_ciphers). Para a criptografar são utilizados dois arquivos: o livro cifra
utilizado como base para gerar as cifras e o arquivo de chaves que possui
todas as relações letra inicial/chave. 

---

## Codificação e Decodificação
 O programa foi testado para codificação de arquivos em caracteres ascii e com
caracteres utilizados na língua portuguesa (letras acentudas e ç).
 Caso o arquivo codificado não esteja em *ascii*, a sua decodificação resultará 
em um equivalente em *ascii*. Por exemplo, ao codificar e depois decodificar a 
frase *"maçã, romã, maracujá"* resultará na frase *"maca, roma, maracuja"*.
 O diretorio `/livros` contem os livros cifra e arquivos utilizados nos teste.

## Entrada

Codificar, criando um arquivo de chaves
```
./beale -e -b livro-cifra -m msg-original -o msg-codificada -c arquivo-chaves
```

Decodificar por meio de livro cifra
```
./beale -d -b livro-cifra -i msg-codificada -o msg-decodificada
```

Decodificar por meio de arquivo de chaves
```
./beale -d -c arquivo-chaves -i msg-codificada -o msg-decodificada
```

 Há duas opções de entrada: `-e`, para codificação e `-d` para decodificação.
 Estas opções precisam ser as primeiras indicadas, caso contrario, uma
mensagem de erro será mostrada e o programa abortado.
 Nas opções deve ser indicado o caminho para o arquivo, caso não esteja no
diretório corrente.
 Ao chamar o programa com opção `-e`, se o arquivo de chaves especificado na 
opção `-c` ja existir, o arquivo será sobreescrito.

## Estruturação do programa
 O programa é estruturado em uma lista de listas. Cada nó da lista principal 
possui um char de letra inicial e um ponteir para a lista de posições das
paravras que começam com o char.
```
 char -> lista posicoes
  |
 char -> lista posicoes
  |
 char -> lista posicoes
```
 As funções e estruras foram implementadas utilizando *wide char* para
suportar entradas de codificacao de textos contendo cedilha e letras
acentudadas utilizadas na língua portuguesa.
 As funções principais estão agrupadas no biblioteca `libbeale.h`, a qual
possui todas as funções utilizadas no programa principal.
 Chaves para casos especiais são utilizadas: 
 ```
-1 (espaço ' ');
-2 (nova linha '\n');
-3 (sem chave correspondente na list '?')
```
 O programa foi feito de forma que todos os caracteres da tabela *ascii*
possuam pelo menos uma chave correspondente na lista de cifras, então para
codificação e decodificacao de arquivos neste formato, sempre deverá existir
pelo menos uma chave na lista.
