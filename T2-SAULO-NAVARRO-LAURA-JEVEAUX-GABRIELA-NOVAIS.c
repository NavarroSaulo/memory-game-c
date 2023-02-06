#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Jogo {
    int linha;
    int coluna;
    int numlinha;
    int pares;
    int l1;
    int c1;
    int l2;
    int c2;
    int semente;
    int sementinha;
    int namePos;
} tPropriedadesJogo;

typedef struct Status {
    bool novoJogo;
    bool emPartida;
    char tryAgain;
    int escolhaMenu;
} tStatusDoJogo;

typedef struct Tabuleiro {
    int tamanhoTab;
    int *letras;
    int *posicoesLetras;
    int novaPosicaoLetra;
    char **tabuleiro;
    char **mascara;
    char **matrizAcertos;
} tsTabuleiro;

typedef struct Jogadores {
    char *jogadores;
} tJogadores;

typedef struct Pontuacao {
    int *pontos;
} tPontuacao;

void blue();
void green();
void red();
void reset();

void mostrar_menu();
bool voltarMenu(char escolha, int novoJogo);
void gerarLetras(int *vetor, int sementeFixa, int semente, int tamanhoTab);
void embaralharLetras(int *vetorEmbaralhado, int *vetor, int tamanho);
void printarTabuleiro(int linha, int contadorLinha, int coluna, int tamanho, char **tabuleiro);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("\nA sua execucao foi invalida, voce precisa colocar mais argumentos na sua tentativa.\n");
        printf("\nTente novamente... Use 'algo.txt' 'tamanho do tabuleiro' e nomes\n");
        exit(1);
    }
    tPropriedadesJogo *j = malloc(sizeof(tPropriedadesJogo));
    j->sementinha = 1;
    j->numlinha = 1;  //
    j->pares = 0;     //
    j->namePos = 3;

    tStatusDoJogo *sJ = malloc(sizeof(tStatusDoJogo));
    sJ->novoJogo = true;
    sJ->emPartida = false;

    tsTabuleiro *t = malloc(sizeof(tsTabuleiro));
    if (atoi(argv[2]) % 2 == 0) {
        t->tamanhoTab = (atoi(argv[2]));
        printf("\e[H\e[2J");  // stdlib, limpa o terminal
    } else {
        t->tamanhoTab = (atoi(argv[2]) + 1);
        printf("\e[H\e[2J");  // stdlib, limpa o terminal
        blue();
        printf("\n\tO Tabuleiro tem que ser par! Aumentamos o tamanho em 1\n");
        reset();
    }

    tJogadores *nomes = malloc((argc - 2) * sizeof(tJogadores));
    if (nomes == NULL) {
        printf("Falha na alocação.\n");
        return 1;
    }
    for (int i = 3; i < argc; i++) {
        nomes[i - 3].jogadores = malloc(strlen(argv[i]) + 1);
        if (nomes[i - 3].jogadores == NULL) {
            printf("Falha na alocação.\n");
            return 1;
        }
        strcpy(nomes[i - 3].jogadores, argv[i]);
    }

    tPontuacao *p = malloc(sizeof(tPontuacao));
    if (p == NULL) {
        printf("Falha na alocação.\n");
        return 1;
    }
    p->pontos = malloc((argc - 2) * sizeof(int));
    if (p->pontos == NULL) {
        printf("Falha na alocação.\n");
        return 1;
    }
    for (int i = 0; i < argc - 2; i++) {
        p->pontos[i] = 0;
    }

    t->letras = (int *)malloc(t->tamanhoTab * t->tamanhoTab * sizeof(int));
    t->posicoesLetras = (int *)malloc(t->tamanhoTab * t->tamanhoTab * sizeof(int));
    t->novaPosicaoLetra = 0;
    t->tabuleiro = (char **)malloc(t->tamanhoTab * sizeof(char *));
    t->mascara = (char **)malloc(t->tamanhoTab * sizeof(char *));
    t->matrizAcertos = (char **)malloc(t->tamanhoTab * sizeof(char *));

    for (int i = 0; i < t->tamanhoTab; i++) {
        t->tabuleiro[i] = (char *)malloc(t->tamanhoTab * sizeof(char));
        t->mascara[i] = (char *)malloc(t->tamanhoTab * sizeof(char));
        t->matrizAcertos[i] = (char *)malloc(t->tamanhoTab * sizeof(char));
    }

    char nome_result[100] = "Result_";
    FILE *arq;
    FILE *result;

    printf("\n\tTrabalho de Programacao\n");
    printf("\tAlunos: Gabriela Novais, Laura Jeveaux e Saulo Navarro\n");
    printf("\tTrabalho: Jogo da memoria\n\n");
    printf("\n\n\tDigite qualquer coisa para ir ao menu!\n");
    getchar();  // stdio, espera a pessoa pressionar qualquer tecla para prosseguir o codigo

    do {  // Loop dependente do estado do bool novoJogo, basicamente o programa inteiro e apos ele, pergunta se deseja voltar ao menu ou nao
        mostrar_menu();
        scanf("%d", &sJ->escolhaMenu);
        switch (sJ->escolhaMenu) {
            case 1:
                do {  // loop que quebra quando a partida começa, para nao ficar gerando toda vez.
                    printf("\n\tDigite uma semente para a geracao do tabuleiro: ");
                    do {
                        scanf("%d", &j->semente);
                        if (j->semente < 1) {
                            printf("Favor digitar uma semente positiva");
                        }
                    } while (j->semente < 1);
                    printf("\e[H\e[2J");

                    gerarLetras(t->letras, j->sementinha, j->semente, t->tamanhoTab);
                    embaralharLetras(t->posicoesLetras, t->letras, t->tamanhoTab);

                    //  montagem do tabuleiro() de cada matriz, a mascara preenchida com *, a acertos com 0 e o tabuleiro com letras
                    for (j->linha = 0; j->linha < t->tamanhoTab; j->linha++) {
                        for (j->coluna = 0; j->coluna < t->tamanhoTab; j->coluna++) {
                            t->mascara[j->linha][j->coluna] = '*';
                            t->matrizAcertos[j->linha][j->coluna] = 0;
                            t->tabuleiro[j->linha][j->coluna] = t->posicoesLetras[t->novaPosicaoLetra];
                            t->novaPosicaoLetra++;
                        }
                    }

                    arq = fopen(argv[1], "w");  // Chamada para abrir e escrever no arquivo
                    if (arq == NULL) {
                        printf("Erro ao abrir!\n");
                        exit(1);
                    } else {
                        fprintf(arq, "Tabuleiro:\n", stdout);
                        for (j->linha = 0; j->linha < t->tamanhoTab; j->linha++) {
                            for (j->coluna = 0; j->coluna < t->tamanhoTab; j->coluna++) {
                                fprintf(arq, "  %c  ", t->tabuleiro[j->linha][j->coluna]);
                            }
                            fprintf(arq, "\n\n", stdout);
                        }
                    }
                    fclose(arq);

                    printf("\n\tVoce tem 10 segundos para memorizar o tabuleiro\n");
                    usleep(2000000);                                                                  // unistd, espera o parametro em ms antes de passar para a proxima linha
                    printarTabuleiro(j->linha, j->numlinha, j->coluna, t->tamanhoTab, t->tabuleiro);  // printar tabuleiro completo
                    usleep(10000000);
                    sJ->emPartida = true;  // pra nao ficar remontando o tabuleiro
                } while (sJ->emPartida == false);

                while (sJ->emPartida == true) {  // agora que está em partida e o tabuleiro foi montado, hora de jogar
                    for (int i = 3; i < argc; i++) {
                        printarTabuleiro(j->linha, j->numlinha, j->coluna, t->tamanhoTab, t->mascara);
                        do {
                            printf("\n\t %s Digite duas coordenadas linha coluna (0 0 0 0 pra encerrar): ", argv[i]);
                            scanf("%d %d %d %d", &j->l1, &j->l2, &j->c1, &j->c2);
                            if ((j->l1 == 0) && (j->l2 == 0) && (j->c1 == 0) && (j->c2 == 0)) {  // Caso digite 0 0 0 0
                                sJ->emPartida = false;
                                result = fopen(strcat(nome_result, argv[1]), "w");
                                fprintf(result, "Resultado do Tabuleiro:\n");
                                for (j->linha = 0; j->linha < t->tamanhoTab; j->linha++) {
                                    for (j->coluna = 0; j->coluna < t->tamanhoTab; j->coluna++) {
                                        if (t->matrizAcertos[j->linha][j->coluna] == 1) {
                                            fprintf(result, "  %c  ", t->tabuleiro[j->linha][j->coluna]);
                                        } else {
                                            fprintf(result, "  *  ", stdout);  // Printa vazio se não houver um par
                                        }
                                    }
                                    fprintf(result, "\n\n", stdout);
                                }
                                fclose(result);
                                return 0;
                            } else if (j->l1 == j->c1 && j->l2 == j->c2) {  // mesma coordenada, por exemplo 3 1 3 1
                                red();
                                printf("\n\tCoordenada Repetida\n");
                                reset();
                            }

                        } while ((j->l1 == j->c1) && (j->l2 == j->c2));

                        if (t->tabuleiro[j->l1 - 1][j->l2 - 1] == t->tabuleiro[j->c1 - 1][j->c2 - 1] && t->matrizAcertos[j->l1 - 1][j->l2 - 1] != 1 && t->matrizAcertos[j->c1 - 1][j->c2 - 1] != 1) {  // Verifica se as letras são iguais e se ja nao foi acertado antes
                            green();
                            t->mascara[j->l1 - 1][j->l2 - 1] = t->tabuleiro[j->l1 - 1][j->l2 - 1];  // Aquele lugar no tabuleiro mascara é revelado
                            t->mascara[j->c1 - 1][j->c2 - 1] = t->tabuleiro[j->c1 - 1][j->c2 - 1];
                            t->matrizAcertos[j->l1 - 1][j->l2 - 1] = 1;
                            t->matrizAcertos[j->c1 - 1][j->c2 - 1] = 1;
                            printarTabuleiro(j->linha, j->numlinha, j->coluna, t->tamanhoTab, t->mascara);
                            j->pares++;  // Aumenta o numero de pares
                            printf("\n\tAcertou! %c e %c\n", t->tabuleiro[j->l1 - 1][j->l2 - 1], t->tabuleiro[j->l1 - 1][j->l2 - 1]);
                            printf("\n\tPontuacao:\n");
                            p->pontos[i - 3]++;
                            for (int i = 3; i < argc; i++) {
                                printf("\n\t %s: ", argv[i]);
                                printf("%d", p->pontos[i - 3]);
                            }
                            usleep(2000000);
                            reset();  // Volta pra cor normal
                            usleep(1000000);
                            if (j->pares == ((t->tamanhoTab * t->tamanhoTab) / 2)) {
                                printf("\e[H\e[2J");
                                blue();
                                printf("\n\tPARABENS POR VENCER O JOGO!\n");
                                reset();
                                usleep(5000000);
                                sJ->emPartida = false;
                                return 0;
                            }
                        } else {  // Se a pessoa errar os pares
                            red();
                            // Muda a cor pra vermelho
                            t->mascara[j->l1 - 1][j->l2 - 1] = t->tabuleiro[j->l1 - 1][j->l2 - 1];  // Muda aquele lugar pra mostrar a letra que era
                            t->mascara[j->c1 - 1][j->c2 - 1] = t->tabuleiro[j->c1 - 1][j->c2 - 1];
                            printarTabuleiro(j->linha, j->numlinha, j->coluna, t->tamanhoTab, t->mascara);

                            if (t->matrizAcertos[j->l1 - 1][j->l2 - 1] == 1) {  // Serie de verificações para ver o jeito que a pessoa errou e a coordenada volta a ser *                                // label
                                printf("\n\tCoordenada repetida, tente novamente\n");
                                for (int i = 3; i < argc; i++) {
                                    printf("\n\t %s: ", argv[i]);
                                    printf("%d", p->pontos[i - 3]);
                                }
                                t->mascara[j->c1 - 1][j->c2 - 1] = '*';
                            } else if (t->matrizAcertos[j->c1 - 1][j->c2 - 1] == 1) {
                                printf("\n\tCoordenada repetida, tente novamente\n");
                                for (int i = 3; i < argc; i++) {
                                    printf("\n\t %s: ", argv[i]);
                                    printf("%d", p->pontos[i - 3]);
                                }
                                t->mascara[j->l1 - 1][j->l2 - 1] = '*';
                            } else {
                                printf("\n\tErrou, tente novamente %c e %c\n", t->tabuleiro[j->l1 - 1][j->l2 - 1], t->tabuleiro[j->c1 - 1][j->c2 - 1]);
                                for (int i = 3; i < argc; i++) {
                                    printf("\n\t %s: ", argv[i]);
                                    printf("%d", p->pontos[i - 3]);
                                }
                                t->mascara[j->l1 - 1][j->l2 - 1] = '*';
                                t->mascara[j->c1 - 1][j->c2 - 1] = '*';
                            }
                            reset();
                            usleep(2000000);
                        }
                    }
                }
                result = fopen(strcat(nome_result, argv[1]), "w");
                fprintf(result, "Resultado do Tabuleiro:\n");
                for (j->linha = 0; j->linha < t->tamanhoTab; j->linha++) {
                    for (j->coluna = 0; j->coluna < t->tamanhoTab; j->coluna++) {
                        if (t->matrizAcertos[j->linha][j->coluna] == 1) {
                            fprintf(result, "  %c  ", t->tabuleiro[j->linha][j->coluna]);
                        } else {
                            fprintf(result, "  *  ", stdout);  // Printa vazio se não houver um par
                        }
                    }
                    fprintf(result, "\n\n", stdout);
                }
                fclose(result);

            case 2:  // Opção 2 do menu
                printf("\e[H\e[2J");
                printf("\n\tO jogo comeca quando VOCE digita um numero\n\tque sera usado para gerar o tabuleiro do jogo, \n\tem seguida ele aparecera por alguns segundos na sua tela,\n\to objetivo e digitar dois pares ordenados que contenham \n\to mesmo caracter, assim denotando esses dois espacos\n");
                break;

            default:  // Acabar com o jogo
                sJ->novoJogo = false;
                return 0;
                break;
        }
        j->sementinha = 1, t->novaPosicaoLetra = 0;  // Reseta parametros do jogo
        voltarMenu(sJ->tryAgain, sJ->novoJogo);
    } while (sJ->novoJogo == true);

    free(j);
    free(sJ);
    free(t->letras);
    free(t->matrizAcertos);
    free(t->posicoesLetras);
    free(t->tabuleiro);
    free(t->mascara);
    free(t);
    free(p);
    return 0;
}

void mostrar_menu() {
    printf("\e[H\e[2J");
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

void gerarLetras(int *vetor, int sementeFixa, int semente, int tamanhoTab) {
    for (int indexVetor = 0; indexVetor < ((tamanhoTab) * (tamanhoTab)); indexVetor += 2) {
        srand((sementeFixa) * (semente + 1));
        int temp = (rand() % 26) + 65;
        bool exists = false;
        for (int j = 0; j < indexVetor; ++j) {
            if (vetor[j] == temp) {
                exists = true;
                sementeFixa++;
                break;
            }
        }
        if (!exists) {
            vetor[indexVetor] = temp;
            vetor[indexVetor + 1] = temp;
        } else {
            indexVetor -= 2;
        }
    }
}

void embaralharLetras(int *vetorEmbaralhado, int *vetor, int tamanho) {  // Consiste em basicamente o mesmo do que a função gerarLetras de acordo com a posição
    for (int pos = 0; pos < (tamanho) * (tamanho); pos++) {
        int numPosicao = (rand() % ((tamanho) * (tamanho)));
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
}

void printarTabuleiro(int linha, int contadorLinha, int coluna, int tamanho, char **tabuleiro) {
    printf("\e[H\e[2J");
    printf("    ");
    for (coluna = 0; coluna < tamanho; coluna++) {
        printf("%d   ", coluna + 1);
    }
    printf("\n\n");
    for (linha = 0; linha < tamanho; linha++) {
        printf("%d  ", contadorLinha);  // Linhas
        contadorLinha++;

        for (coluna = 0; coluna < tamanho; coluna++) {
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
