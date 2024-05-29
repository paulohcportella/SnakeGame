#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define TAMANHO_MAXIMO_COBRA 100
#define TAMANHO_INICIAL_COBRA 5
#define TABULEIRO_LARGURA 60
#define TABULEIRO_ALTURA 20
#define FILENAME "scores.txt"

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
void lidarComEntrada();
void encerrarJogo();

int main() {
    int tecla = 0;

    screenInit(1);
    keyboardInit();
    timerInit(100);

    while (tecla != 27) { // Tecla ESC para sair
        screenClear();
        printf("Pressione qualquer tecla para iniciar...");
        screenUpdate();

        if (keyhit()) {
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
            }
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
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
    Ponto proximaPosicao = cobra[0];

    switch (direcao) {
        case 'w': proximaPosicao.y -= 1; break;
        case 's': proximaPosicao.y += 1; break;
        case 'a': proximaPosicao.x -= 1; break;
        case 'd': proximaPosicao.x += 1; break;
    }

    for (int i = tamanhoCobra - 1; i > 0; i--) {
        cobra[i] = cobra[i - 1];
    }

    cobra[0] = proximaPosicao;

    if (cobra[0].x == fruta.x && cobra[0].y == fruta.y) {
        tamanhoCobra++;
        pontuacao += 10;
        gerarFruta();
    }

    // Verificar colisão da cabeça da cobra com o próprio corpo
    for (int i = 1; i < tamanhoCobra; i++) {
        if (cobra[0].x == cobra[i].x && cobra[0].y == cobra[i].y) {
            encerrarJogo();
        }
    }
}

void gerarFruta() {
    fruta.x = (rand() % (TABULEIRO_LARGURA - 2)) + 1;
    fruta.y = (rand() % (TABULEIRO_ALTURA - 2)) + 1;
}

int verificarColisao() {
    if (cobra[0].x <= 0 || cobra[0].x >= TABULEIRO_LARGURA - 1 || cobra[0].y <= 0 || cobra[0].y >= TABULEIRO_ALTURA - 1) return 1;

    return 0;
}

void desenharJogo() {
    screenClear();
    screenSetColor(GREEN, BLACK);

    for (int i = 0; i < tamanhoCobra; i++) {
        screenGotoxy(cobra[i].x, cobra[i].y);
        printf("O");
    }

    screenSetColor(RED, BLACK);
    screenGotoxy(fruta.x, fruta.y);
    printf("@");

    screenGotoxy(1, TABULEIRO_ALTURA + 1);
    printf("Pontuação: %d", pontuacao);
}

void lidarComEntrada(int tecla) {
    switch (tecla) {
        case 'w':
            if (direcao != 's') direcao = 'w';
            break;
        case 's':
            if (direcao != 'w') direcao = 's';
            break;
        case 'a':
            if (direcao != 'd') direcao = 'a';
            break;
        case 'd':
            if (direcao != 'a') direcao = 'd';
            break;
    }
}

void encerrarJogo() {
    screenClear();
    screenGotoxy(10, TABULEIRO_ALTURA / 2);
    printf("GAME OVER!");
    screenUpdate();

    free(cobra);

    // Limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
