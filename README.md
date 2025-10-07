#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM 5  // tamanho fixo da fila

// Estrutura que representa uma peça do jogo
typedef struct {
    int id;
    char tipo[2];  // tipo da peça: "I", "O", "T", "L"
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[TAM];
    int frente;
    int tras;
    int qtd;
} Fila;

// ---------- Função que inicializa a fila ----------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// ---------- Função que gera uma peça automaticamente ----------
Peca gerarPeca() {
    Peca nova;
    char tipos[4][2] = {"I", "O", "T", "L"};
    nova.id = rand() % 1000;  // id aleatório
    strcpy(nova.tipo, tipos[rand() % 4]);
    return nova;
}

// ---------- Inserir uma peça no final da fila ----------
void enfileirar(Fila *f, Peca p) {
    if (f->qtd == TAM) {
        printf("\nA fila está cheia! Não é possível inserir nova peça.\n");
        return;
    }
    f->tras = (f->tras + 1) % TAM;
    f->pecas[f->tras] = p;
    f->qtd++;
}

// ---------- Remover uma peça da frente da fila ----------
Peca desenfileirar(Fila *f) {
    Peca removida = {-1, ""};
    if (f->qtd == 0) {
        printf("\nA fila está vazia! Nenhuma peça para jogar.\n");
        return removida;
    }
    removida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM;
    f->qtd--;
    return removida;
}

// ---------- Exibir o estado atual da fila ----------
void exibirFila(Fila f) {
    if (f.qtd == 0) {
        printf("\nFila vazia.\n");
        return;
    }
    printf("\nPeças futuras na fila:\n");
    for (int i = 0; i < f.qtd; i++) {
        int pos = (f.frente + i) % TAM;
        printf("Posição %d: Tipo %s | ID %d\n", i + 1, f.pecas[pos].tipo, f.pecas[pos].id);
    }
}

// ---------- Função principal ----------
int main() {
    srand(time(NULL));

    Fila fila;
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças geradas automaticamente
    for (int i = 0; i < TAM; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n===== MENU TETRIS STACK (Nível Novato) =====\n");
        printf("1 - Jogar (remover peça da frente)\n");
        printf("2 - Inserir nova peça\n");
        printf("3 - Visualizar fila\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("Peça jogada: Tipo %s | ID %d\n", jogada.tipo, jogada.id);
                break;
            }
            case 2: {
                Peca nova = gerarPeca();
                enfileirar(&fila, nova);
                printf("Nova peça inserida: Tipo %s | ID %d\n", nova.tipo, nova.id);
                break;
            }
            case 3:
                exibirFila(fila);
                break;
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}