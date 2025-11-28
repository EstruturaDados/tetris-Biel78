#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILA 5
#define MAX_PILHA 3
#define MAX_HIST 20   // histórico para desfazer última jogada

// ----------------------------
// Estruturas
// ----------------------------
typedef struct {
    int elementos[MAX_FILA];
    int frente, tras, qtd;
} Fila;

typedef struct {
    int elementos[MAX_PILHA];
    int topo;
} Pilha;

typedef struct {
    Fila fila;
    Pilha pilha;
} Estado;

// ----------------------------
// Prototipos
// ----------------------------
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);

int filaCheia(Fila *f);
int filaVazia(Fila *f);
void enfileirar(Fila *f, int valor);
int desenfileirar(Fila *f);

int pilhaCheia(Pilha *p);
int pilhaVazia(Pilha *p);
void push(Pilha *p, int valor);
int pop(Pilha *p);

int gerarPeca();
void mostrarEstado(Fila f, Pilha p);

void salvarEstado(Estado hist[], int *topoHist, Fila f, Pilha p);
int desfazer(Estado hist[], int *topoHist, Fila *f, Pilha *p);

void trocarTopoFila(Fila *f, Pilha *p);
void inverterEstruturas(Fila *f, Pilha *p);

// ----------------------------
// Implementações
// ----------------------------
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = 0;
    f->qtd = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaCheia(Fila *f) {
    return f->qtd == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

void enfileirar(Fila *f, int valor) {
    if (filaCheia(f)) return;

    f->elementos[f->tras] = valor;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->qtd++;
}

int desenfileirar(Fila *f) {
    if (filaVazia(f)) return -1;

    int val = f->elementos[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->qtd--;
    return val;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, int valor) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->elementos[p->topo] = valor;
}

int pop(Pilha *p) {
    if (pilhaVazia(p)) return -1;
    int val = p->elementos[p->topo];
    p->topo--;
    return val;
}

int gerarPeca() {
    return rand() % 100; // peças de 0 a 99
}

void mostrarEstado(Fila f, Pilha p) {
    printf("\n--- ESTADO ATUAL ---\n");

    // Fila
    printf("Fila: ");
    int i = f.frente;
    for (int c = 0; c < f.qtd; c++) {
        printf("[%d] ", f.elementos[i]);
        i = (i + 1) % MAX_FILA;
    }
    printf("\n");

    // Pilha
    printf("Pilha: ");
    for (int j = 0; j <= p.topo; j++) {
        printf("[%d] ", p.elementos[j]);
    }
    printf("\n---------------------\n");
}

void salvarEstado(Estado hist[], int *topoHist, Fila f, Pilha p) {
    if (*topoHist < MAX_HIST - 1) {
        (*topoHist)++;
        hist[*topoHist].fila = f;
        hist[*topoHist].pilha = p;
    }
}

int desfazer(Estado hist[], int *topoHist, Fila *f, Pilha *p) {
    if (*topoHist < 0) return 0;
    *f = hist[*topoHist].fila;
    *p = hist[*topoHist].pilha;
    (*topoHist)--;
    return 1;
}

void trocarTopoFila(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) return;

    int temp = f->elementos[f->frente];
    f->elementos[f->frente] = p->elementos[p->topo];
    p->elementos[p->topo] = temp;
}

void inverterEstruturas(Fila *f, Pilha *p) {
    Pilha tempPilha;
    Fila tempFila;

    inicializarPilha(&tempPilha);
    inicializarFila(&tempFila);

    // Inverte fila -> pilha
    while (!filaVazia(f)) {
        push(&tempPilha, desenfileirar(f));
    }

    // Inverte pilha -> fila
    while (!pilhaVazia(p)) {
        enfileirar(&tempFila, pop(p));
    }

    *f = tempFila;
    *p = tempPilha;
}

// ----------------------------
// Função principal
// ----------------------------
int main() {
    Fila fila;
    Pilha pilha;

    Estado historico[MAX_HIST];
    int topoHist = -1;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // preenche fila inicialmente
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        mostrarEstado(fila, pilha);

        printf("\nMENU\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça (push)\n");
        printf("3 - Usar peça reservada (pop)\n");
        printf("4 - Trocar topo da pilha com a frente da fila\n");
        printf("5 - Desfazer última jogada\n");
        printf("6 - Inverter fila com pilha\n");
        printf("0 - Sair\n> ");
        scanf("%d", &opcao);

        switch (opcao) {

        case 1: { 
            salvarEstado(historico, &topoHist, fila, pilha);
            int jogada = desenfileirar(&fila);
            printf("Jogou a peça: %d\n", jogada);
            enfileirar(&fila, gerarPeca());
            break;
        }

        case 2:
            if (!pilhaCheia(&pilha)) {
                salvarEstado(historico, &topoHist, fila, pilha);
                push(&pilha, desenfileirar(&fila));
                enfileirar(&fila, gerarPeca());
            } else {
                printf("Pilha cheia!\n");
            }
            break;

        case 3:
            if (!pilhaVazia(&pilha)) {
                salvarEstado(historico, &topoHist, fila, pilha);
                printf("Usou peça: %d\n", pop(&pilha));
            } else {
                printf("Pilha vazia!\n");
            }
            break;

        case 4:
            salvarEstado(historico, &topoHist, fila, pilha);
            trocarTopoFila(&fila, &pilha);
            printf("Trocado com sucesso.\n");
            break;

        case 5:
            if (!desfazer(historico, &topoHist, &fila, &pilha)) {
                printf("Nada para desfazer.\n");
            } else {
                printf("Última jogada desfeita.\n");
            }
            break;

        case 6:
            salvarEstado(historico, &topoHist, fila, pilha);
            inverterEstruturas(&fila, &pilha);
            printf("Estruturas invertidas.\n");
            break;

        case 0:
            printf("Encerrando...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
