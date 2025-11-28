#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM 5   // tamanho fixo da fila

// ----------- STRUCT DA PEÇA -----------
typedef struct {
    int id;
    char nome[2];  // "I", "O", "T", "L"
} Peca;

// ----------- FILA CIRCULAR -----------
typedef struct {
    Peca fila[TAM];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ----------- GERAR NOVA PEÇA -----------
Peca gerarPeca() {
    Peca p;
    char tipos[4][2] = {"I", "O", "T", "L"};

    p.id = rand() % 1000;  // id aleatório
    strcpy(p.nome, tipos[rand() % 4]);

    return p;
}

// ----------- INICIALIZAR FILA -----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;

    for (int i = 0; i < TAM; i++) {
        f->fila[i] = gerarPeca();
        f->fim = (f->fim + 1) % TAM;
        f->quantidade++;
    }
}

// ----------- MOSTRAR FILA -----------
void mostrarFila(Fila *f) {
    printf("\n=== FILA ATUAL ===\n");

    if (f->quantidade == 0) {
        printf("Fila vazia!\n");
        return;
    }

    int idx = f->inicio;

    for (int i = 0; i < f->quantidade; i++) {
        printf("[%d] Peça %s (ID %d)\n",
               i,
               f->fila[idx].nome,
               f->fila[idx].id);

        idx = (idx + 1) % TAM;
    }

    printf("Quantidade: %d\n", f->quantidade);
}

// ----------- DEQUEUE (JOGAR PEÇA) -----------
void jogarPeca(Fila *f) {
    if (f->quantidade == 0) {
        printf("\nFila vazia! Nada para jogar.\n");
        return;
    }

    printf("\nJogando a peça da frente: %s (ID %d)\n",
           f->fila[f->inicio].nome,
           f->fila[f->inicio].id);

    f->inicio = (f->inicio + 1) % TAM;
    f->quantidade--;
}

// ----------- ENQUEUE (INSERIR NOVA PEÇA) -----------
void inserirPeca(Fila *f) {
    if (f->quantidade == TAM) {
        printf("\nFila cheia! Não é possível adicionar.\n");
        return;
    }

    Peca nova = gerarPeca();
    f->fila[f->fim] = nova;

    printf("\nNova peça inserida: %s (ID %d)\n",
           nova.nome,
           nova.id);

    f->fim = (f->fim + 1) % TAM;
    f->quantidade++;
}

// ----------- MENU PRINCIPAL -----------
int menu() {
    int opc;

    printf("\n========== MENU ==========\n");
    printf("1 - Jogar peça (dequeue)\n");
    printf("2 - Inserir nova peça (enqueue)\n");
    printf("3 - Visualizar fila\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opc);

    return opc;
}

// ----------- PROGRAMA PRINCIPAL -----------
int main() {
    srand(time(NULL));

    Fila f;
    inicializarFila(&f);

    int opc;
    do {
        opc = menu();

        switch(opc) {
        case 1:
            jogarPeca(&f);
            mostrarFila(&f);
            break;

        case 2:
            inserirPeca(&f);
            mostrarFila(&f);
            break;

        case 3:
            mostrarFila(&f);
            break;

        case 0:
            printf("\nEncerrando...\n");
            break;

        default:
            printf("\nOpção inválida!\n");
        }

    } while (opc != 0);

    return 0;
}
