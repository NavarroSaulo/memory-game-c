/*
Trabalho 1 de Programação 1
Engenharia Elétrica
Alunos: Gabriela Novais, Laura Jeveaux, Saulo Navarro
Funções de bibliotecas usadas tem seu nome comentado em frente ao primeiro uso.
Linux Version
*/
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Jogo {
    int linha;
    int coluna;
    int numlinha;
    int pares;       // Numeros de pares acertados e coordenadas a serem digitadas para jogar
    int l1;          // linha 1
    int c1;          // coluna 1
    int l2;          // linha 2
    int c2;          // coluna 2
    int semente;     // Numero digitado pelo usuario
    int sementinha;  // Numero auxiliar para randomização
} propriedadesJogo;

typedef struct Status {
    bool novoJogo;  // stdboolh, novoJogo é para continuar rodando o programa enquanto o usuario nao escolher sair do menu. emPartida serve para verificar se a pessoa está no jogo da memória ou no menu
    bool emPartida;
    char tryAgain;    // tryAgain armazena o valor que serve para voltar ao menu ou nao
    int escolhaMenu;  // escolhaMenu armazena o valor do menu que o usuário for entrar, sementinha é o numero que ira ser randomizado de acordo com a semente que será scaneado pelo usuario
} statusDoJogo;

typedef struct Tabuleiro {
    int letras[16];
    int posicoesLetras[16];  // Vetor contendo as letras aleatorias e a posição em que cada letra vai ficar
    int novaPosicaoLetra;
    char tabuleiro[4][4];  // tabuleiro, mascara e matrizAcertos são respectivamente matrizes que armazenam as letras ordenadas aleatoriamente, a matriz de * que será jogada e uma matriz de 0's e 1's que verifica a posição que ja foi descoberta
    char mascara[4][4];
    char matrizAcertos[4][4];
} sTabuleiro;

void blue();
void green();
void red();
void reset();
// Acima estão as funções para mudar a cor do texto e voltar ao padrão
void mostrar_menu();                                                                    // Apenas printa as opções do menu
void printarTabuleiro(int linha, int contadorLinha, int coluna, char tabuleiro[4][4]);  // Ao receber uma matriz e suas caracteristicas, printa essa matriz formatada
bool voltarMenu(char escolha, int novoJogo);                                            // Pergunta e recebe se o usuário deseja voltar ao menu ou nao
int gerarLetras(int vetor[16], int sementeFixa, int semente);                           // Ao receber uma semente do usuário, gera 8 Letras aleatórias e guarda em um vetor duplicando as letras (pares)
int embaralharLetras(int vetorEmbaralhado[16], int vetor[16]);                          // Depois de preenchido o vetor de pares de letras aleatórias, aleatoriamente "bagunçamos" essas letras em outro vetor

int main() {
    propriedadesJogo j;
    j.numlinha = 0;
    j.pares = 0;
    j.sementinha = 1;

    statusDoJogo sJ;
    sJ.novoJogo = true;
    sJ.emPartida = false;

    sTabuleiro t;
    t.novaPosicaoLetra = 0;

    // Comando de regionalização:
    setlocale(LC_ALL, "Portuguese_Brasil");  // localeh

// Introdução ao programa
inicio:                  // label para um goto
    system("clear");     // stdlib, limpa o terminal
    if (j.pares == 8) {  // Quando a pessoa vencer o jogo ela volta aqui por um goto
        blue();
        printf("\n\tPARABENS POR VENCER O JOGO!\n");
        reset();
    }
    printf("\n\tTrabalho de Programacao\n");
    printf("\tAlunos: Gabriela Novais, Laura Jeveaux e Saulo Navarro\n");
    printf("\tTrabalho: Jogo da memoria\n\n");
    printf("\n\n\tDigite qualquer coisa para ir ao menu!\n");
    getchar();  // stdio, espera a pessoa pressionar qualquer tecla para prosseguir o codigo

    do {  // Loop dependente do estado do bool novoJogo, basicamente o programa inteiro e apos ele, pergunta se deseja voltar ao menu ou nao
        mostrar_menu();
        scanf("%d", &sJ.escolhaMenu);
        switch (sJ.escolhaMenu) {
            case 1:
                do {  // loop que quebra quando a partida começa, para nao ficar gerando toda vez.
                    printf("\n\tDigite uma semente para a geracao do tabuleiro: ");
                semente:
                    scanf("%d", &j.semente);
                    if (j.semente < 1) {
                        printf("Favor digitar uma semente positiva");
                        goto semente;
                    }
                    system("clear");

                    gerarLetras(t.letras, j.sementinha, j.semente);
                    embaralharLetras(t.posicoesLetras, t.letras);
                    // montagem do tabuleiro() de cada matriz, a mascara preenchida com *, a acertos com 0 e o tabuleiro com letras
                    for (j.linha = 0; j.linha < 4; j.linha++) {
                        for (j.coluna = 0; j.coluna < 4; j.coluna++) {
                            t.mascara[j.linha][j.coluna] = '*';
                            t.matrizAcertos[j.linha][j.coluna] = 0;
                            t.tabuleiro[j.linha][j.coluna] = t.posicoesLetras[t.novaPosicaoLetra];
                            t.novaPosicaoLetra++;  // incrementa o index do vetor posicoesLetras(posicoes aleatorias)
                        }
                    }
                    printf("\n\tVoce tem 10 segundos para memorizar o tabuleiro\n");
                    usleep(2000000);                                               // unistd, espera o parametro em ms antes de passar para a proxima linha
                    printarTabuleiro(j.linha, j.numlinha, j.coluna, t.tabuleiro);  // printar tabuleiro completo
                    usleep(10000000);
                    sJ.emPartida = true;  // pra nao ficar remontando o tabuleiro
                } while (sJ.emPartida == false);

                while (sJ.emPartida == true) {  // agora que está em partida e o tabuleiro foi montado, hora de jogar
                    printarTabuleiro(j.linha, j.numlinha, j.coluna, t.mascara);
                    printf("\n\tDigite duas coordenadas linha coluna (0 0 0 0 pra encerrar): ");
                    scanf("%d %d %d %d", &j.l1, &j.l2, &j.c1, &j.c2);
                    if ((j.l1 + j.l2 + j.c1 + j.c2) == 0) {  // Caso digite 0 0 0 0
                        goto inicio;
                    } else if (j.l1 == j.c1 && j.l2 == j.c2) {  // mesma coordenada, por exemplo 3 1 3 1
                        goto repetida;                          // goto pra qnd a pessoa bota a mesma coordenada duas vezes
                    }
                    if (t.tabuleiro[j.l1][j.l2] == t.tabuleiro[j.c1][j.c2] && t.matrizAcertos[j.l1][j.l2] != 1 && t.matrizAcertos[j.c1][j.c2] != 1) {  // Verifica se as letras são iguais e se ja nao foi acertado antes
                        green();                                                                                                                       // Muda a cor pra verde
                        t.mascara[j.l1][j.l2] = t.tabuleiro[j.l1][j.l2];                                                                               // Aquele lugar no tabuleiro mascara é revelado
                        t.mascara[j.c1][j.c2] = t.tabuleiro[j.c1][j.c2];
                        t.matrizAcertos[j.l1][j.l2] = 1;
                        t.matrizAcertos[j.c1][j.c2] = 1;
                        printarTabuleiro(j.linha, j.numlinha, j.coluna, t.mascara);
                        reset();    // Volta pra cor normal
                        j.pares++;  // Aumenta o numero de pares
                        printf("\n\tAcertou!\n");
                        usleep(1000000);
                        if (j.pares == 8) {
                            system("clear");
                            goto inicio;
                        }                                                 // Volta pro label inicio quando a pessoa vencer
                    } else {                                              // Se a pessoa errar os pares
                        red();                                            // Muda a cor pra vermelho
                        t.mascara[j.l1][j.l2] = t.tabuleiro[j.l1][j.l2];  // Muda aquele lugar pra mostrar a letra que era
                        t.mascara[j.c1][j.c2] = t.tabuleiro[j.c1][j.c2];
                        printarTabuleiro(j.linha, j.numlinha, j.coluna, t.mascara);
                        reset();
                        if (t.matrizAcertos[j.l1][j.l2] == 1) {  // Serie de verificações para ver o jeito que a pessoa errou e a coordenada volta a ser *
                        repetida:                                // label
                            printf("\n\tCoordenada repetida, tente novamente\n");
                            t.mascara[j.c1][j.c2] = '*';
                        } else if (t.matrizAcertos[j.c1][j.c2] == 1) {
                            printf("\n\tCoordenada repetida, tente novamente\n");
                            t.mascara[j.l1][j.l2] = '*';
                        } else {
                            printf("\n\tErrou, tente novamente\n");
                            t.mascara[j.l1][j.l2] = '*';
                            t.mascara[j.c1][j.c2] = '*';
                        }
                        usleep(1000000);
                    }
                }
                break;

            case 2:  // Opção 2 do menu
                system("clear");
                printf("\n\tO jogo comeca quando VOCE digita um numero\n\tque sera usado para gerar o tabuleiro do jogo, \n\tem seguida ele aparecera por alguns segundos na sua tela,\n\to objetivo e digitar dois pares ordenados que contenham \n\to mesmo caracter, assim denotando esses dois espacos\n");
                break;

            default:  // Acabar com o jogo
                sJ.novoJogo = false;
                return 0;
                break;
        }
        j.sementinha = 1, t.novaPosicaoLetra = 0;  // Reseta parametros do jogo
        voltarMenu(sJ.tryAgain, sJ.novoJogo);

    } while (sJ.novoJogo == true);
    return 0;
}

void mostrar_menu() {
    system("clear");
    printf("\n\n\tEscolha uma das opcoes abaixo:");
    printf("\n\n\t1)Novo Jogo");
    printf("\n\t2)Como jogar");
    printf("\n\t3)Sair\n\n\t :");
}

bool voltarMenu(char escolha, int novoJogo) {
    printf("\n\tDeseja voltar ao menu? (s - sim, qualquer outra coisa - nao)\n");
    scanf(" %c", &escolha);
    if (escolha != 's') {
        novoJogo = false;
        exit(0);  // stdlib, sai do programa
    } else {
        novoJogo = true;
    }
    return novoJogo;
}

int gerarLetras(int vetor[16], int sementeFixa, int semente) {
    for (int indexVetor = 0; indexVetor < 16; indexVetor += 2) {  // Loop que incrementa em 2 para gerar 2 letras iguais
        srand((sementeFixa) * (semente + 1));                     // função que gera um numero aleatorio
        int temp = (rand() % 26) + 65;                            // Cria uma variavel temporaria que vai ser um numero entre 1 e 26, somando 65 para as letras na tabela ascii
        bool exists = false;                                      // duplicado
        for (int j = 0; j < indexVetor; ++j) {
            if (vetor[j] == temp) {
                exists = true;  // duplicado
                sementeFixa++;  // Incrementa a semente pra gerar letras diferentes
                break;
            }
        }
        if (!exists) {  // Preenche o vetor //duplicado
            vetor[indexVetor] = temp;
            vetor[indexVetor + 1] = temp;
            // AABBCC

        } else {
            indexVetor -= 2;  // Força o loop de volta
        }
    }
    return vetor[16];
}

int embaralharLetras(int vetorEmbaralhado[16], int vetor[16]) {  // Consiste em basicamente o mesmo do que a função gerarLetras de acordo com a posição
    for (int pos = 0; pos < 16; pos++) {
        int numPosicao = (rand() % 16);
        bool duplicado = false;
        int contDuplicado = 0;
        for (int j = 0; j < pos; ++j) {
            if (vetorEmbaralhado[j] == vetor[numPosicao]) {
                contDuplicado++;
                if (contDuplicado == 2) {
                    duplicado = true;
                    break;
                }
            }
        }
        if (!duplicado) {
            vetorEmbaralhado[pos] = vetor[numPosicao];
        } else {
            --pos;
        }
    }
    return vetorEmbaralhado[16];
}

void printarTabuleiro(int linha, int contadorLinha, int coluna, char tabuleiro[4][4]) {
    system("clear");
    printf("    0   1   2   3\n\n");  // Colunas
    for (linha = 0; linha < 4; linha++) {
        printf("%d  ", contadorLinha);  // Linhas
        contadorLinha++;

        for (coluna = 0; coluna < 4; coluna++) {
            printf(" %c  ", tabuleiro[linha][coluna]);  // caracter da matriz
        }
        printf("\n\n");
    }
}

void red() {
    printf("\033[0;31m");
}

void green() {
    printf("\033[0;32m");
}

void blue() {
    printf("\033[0;34m");
}

void reset() {
    printf("\033[0m");
}