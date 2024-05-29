#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define TAMANHO_MAXIMO_COBRA 100
#define TAMANHO_INICIAL_COBRA 5
#define TABULEIRO_LARGURA 60
#define TABULEIRO_ALTURA 20
#define FILENAME "scores.txt"
#define MAX_NOME 30

typedef struct {
    int x, y;
} Ponto;

Ponto *cobra;
int tamanhoCobra = TAMANHO_INICIAL_COBRA;
Ponto fruta;
char direcao = 'd';  // 'w' -> cima, 's' -> baixo, 'a' -> esquerda, 'd' -> direita
int pontuacao = 0;

void inicializarJogo();
void atualizarPosicaoCobra();
void gerarFruta();
int verificarColisao();
void desenharJogo();
void lidarComEntrada(int tecla);
void encerrarJogo();
void salvarPontuacao();
void mostrarPontuacoes();
void introducao();

int main() {
    int tecla = 0;

    screenInit(1);
    keyboardInit();
    timerInit(100);

    introducao();

    while (tecla != 27) { // Tecla ESC para sair
        screenClear();
        printf("Pressione qualquer tecla para iniciar...");
        screenUpdate();

        while (!keyhit()) {
            // Espera até que uma tecla seja pressionada
        }
        tecla = readch();
        if (tecla != 27) {
            inicializarJogo();
            desenharJogo();
            screenUpdate();

            while (tecla != 27 && !verificarColisao()) { // Tecla ESC para sair ou colisão
                if (keyhit()) {
                    tecla = readch();
                    lidarComEntrada(tecla);
                }

                if (timerTimeOver()) {
                    atualizarPosicaoCobra();
                    desenharJogo();
                    screenUpdate();
                }
            }
            encerrarJogo();
            salvarPontuacao();
            mostrarPontuacoes();
            printf("Pressione Enter para jogar novamente...");
            while (getchar() != '\n');
            introducao(); // Chama a introdução novamente após o jogo terminar
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}

void introducao() {
    screenClear();
    printf("Bem-vindo ao jogo da Cobrinha!\n");
    printf("Instruções:\n");
    printf("Use as teclas W, S, A, D para mover a cobra.\n");
    printf("Se bater na borda ou em si mesmo, você morre.\n");
    printf("Quando aparecer 'Game Over', pressione Enter para jogar novamente.\n");
    printf("Ao morrer, você pode inserir seu nome para salvar a pontuação.\n");
    printf("As pontuações existentes serão mostradas após o jogo.\n\n");
    printf("Pressione qualquer tecla para começar...");
    screenUpdate();
    while (!keyhit()) {
        // Espera até que uma tecla seja pressionada
    }
    readch(); // Lê a tecla pressionada para iniciar o jogo
}

void inicializarJogo() {
    srand(time(NULL));
    cobra = (Ponto *)malloc(sizeof(Ponto) * TAMANHO_MAXIMO_COBRA);
    for (int i = 0; i < tamanhoCobra; i++) {
        cobra[i].x = TABULEIRO_LARGURA / 2 - i;
        cobra[i].y = TABULEIRO_ALTURA / 2;
    }
    gerarFruta();
    pontuacao = 0;
}

void atualizarPosicaoCobra() {
    // Mover o corpo da cobra
    for (int i = tamanhoCobra - 1; i > 0; i--) {
        cobra[i] = cobra[i - 1];
    }

    // Mover a cabeça da cobra
    switch (direcao) {
        case 'w': cobra[0].y--; break;
        case 's': cobra[0].y++; break;
        case 'a': cobra[0].x--; break;
        case 'd': cobra[0].x++; break;
    }

    // Verificar se a cobra comeu a fruta
    if (cobra[0].x == fruta.x && cobra[0].y == fruta.y) {
        if (tamanhoCobra < TAMANHO_MAXIMO_COBRA) {
            tamanhoCobra++;
        }
        pontuacao++;
        gerarFruta();
    }
}

void gerarFruta() {
    fruta.x = rand() % TABULEIRO_LARGURA;
    fruta.y = rand() % TABULEIRO_ALTURA;
}

int verificarColisao() {
    // Verificar colisão com as bordas
    if (cobra[0].x < 0 || cobra[0].x >= TABULEIRO_LARGURA || cobra[0].y < 0 || cobra[0].y >= TABULEIRO_ALTURA) {
        return 1;
    }

    // Verificar colisão com o próprio corpo
    for (int i = 1; i < tamanhoCobra; i++) {
        if (cobra[0].x == cobra[i].x && cobra[0].y == cobra[i].y) {
            return 1;
        }
    }

    return 0;
}

void desenharJogo() {
    screenClear();

    // Desenhar tabuleiro com bordas
    for (int y = -1; y <= TABULEIRO_ALTURA; y++) {
        for (int x = -1; x <= TABULEIRO_LARGURA; x++) {
            if (y == -1 || y == TABULEIRO_ALTURA) {
                printf("\033[0;33m_\033[0m");  // Borda em marrom
            } else if (x == -1 || x == TABULEIRO_LARGURA) {
                printf("\033[0;33m|\033[0m");  // Borda em marrom
            } else {
                int parteCorpo = 0;
                for (int i = 0; i < tamanhoCobra; i++) {
                    if (cobra[i].x == x && cobra[i].y == y) {
                        parteCorpo = 1;
                        break;
                    }
                }

                if (parteCorpo) {
                    printf("\033[1;35mO\033[0m");  // Cobra em roxo
                } else if (fruta.x == x && fruta.y == y) {
                    printf("\033[1;32mX\033[0m");  // Fruta em verde
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }

    // Mostrar pontuação
    printf("Pontuação: %d\n", pontuacao);
}

void lidarComEntrada(int tecla) {
    switch (tecla) {
        case 'w': if (direcao != 's') direcao = 'w'; break;
        case 's': if (direcao != 'w') direcao = 's'; break;
        case 'a': if (direcao != 'd') direcao = 'a'; break;
        case 'd': if (direcao != 'a') direcao = 'd'; break;
    }
}

void encerrarJogo() {
    screenClear();
    int center_x = TABULEIRO_LARGURA / 2;
    int center_y = TABULEIRO_ALTURA / 2;

    for (int i = 0; i < center_y - 1; i++) {
        printf("\n");
    }
    for (int i = 0; i < center_x - 5; i++) {
        printf(" ");
    }
    printf("\033[1;31mGame Over!\033[0m\n");

    for (int i = 0; i < center_x - 10; i++) {
        printf(" ");
    }
    printf("Sua pontuação: %d\n", pontuacao);
}

void salvarPontuacao() {
    char nome[MAX_NOME];
    int i = 0;
    char c;

    int center_x = TABULEIRO_LARGURA / 2;

    for (int i = 0; i < center_x - 15; i++) {
        printf(" ");
    }
    printf("Digite seu nome para salvar a pontuação: ");
    screenUpdate();

    while ((c = getchar()) != '\n' && i < MAX_NOME - 1) {
        nome[i++] = c;
        putchar(c); // Exibe o caractere digitado
    }
    nome[i] = '\0'; // Termina a string

    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de pontuações");
        return;
    }

    fprintf(file, "%s %d\n", nome, pontuacao);
    fclose(file);
}

void mostrarPontuacoes() {
    char linha[50];
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de pontuações");
        return;
    }

    screenClear();
    printf("Pontuações:\n");
    while (fgets(linha, sizeof(linha), file)) {
        printf("%s", linha);
    }
    fclose(file);
    screenUpdate();
}
