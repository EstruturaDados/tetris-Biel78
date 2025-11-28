#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ------------------- STRUCT DA PEÇA -------------------
typedef struct {
    int id;
    char nome[2]; // I, O, T, L
} Peca;

// ------------------- FILA CIRCULAR -------------------
typedef struct {
    Peca fila[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ------------------- PILHA LINEAR -------------------
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ------------------- GERAR NOVA PEÇA -------------------
Peca gerarPeca() {
    char tipos[4][2] = {"I", "O", "T", "L"};
    Peca p;
    p.id = rand() % 1000;
    strcpy(p.nome, tipos[rand() % 4]);
    return p;
}

// ------------------- INICIALIZAR FILA -------------------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;

    for (int i = 0; i < TAM_FILA; i++) {
        f->fila[f->fim] = gerarPeca();
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// ------------------- INICIALIZAR PILHA -------------------
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// ------------------- MOSTRAR FILA -------------------
void mostrarFila(Fila *f) {
    printf("\n=== FILA ===\n");

    if (f->quantidade == 0) {
        printf("Fila vazia!\n");
        return;
    }

    int idx = f->inicio;
    for (int i = 0; i < f->quantidade; i++) {
        printf("[%d] %s (ID %d)\n",
               i,
               f->fila[idx].nome,
               f->fila[idx].id);
        idx = (idx + 1) % TAM_FILA;
    }
}

// ------------------- MOSTRAR PILHA -------------------
void mostrarPilha(Pilha *p) {
    printf("\n=== PILHA DE RESERVA ===\n");

    if (p->topo == -1) {
        printf("Pilha vazia!\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("[%d] %s (ID %d)\n",
               i,
               p->itens[i].nome,
               p->itens[i].id);
    }
}

// ------------------- JOGAR PEÇA (DEQUEUE) -------------------
void jogarPeca(Fila *f) {
    if (f->quantidade == 0) {
        printf("\nFila vazia! Nada para jogar.\n");
        return;
    }

    printf("\nJogando peça da frente: %s (ID %d)\n",
           f->fila[f->inicio].nome,
           f->fila[f->inicio].id);

    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->quantidade--;
}

// ------------------- INSERIR PEÇA NA FILA (ENQUEUE) -------------------
void inserirPeca(Fila *f) {
    if (f->quantidade == TAM_FILA) {
        printf("\nA fila já está cheia!\n");
        return;
    }

    Peca nova = gerarPeca();
    f->fila[f->fim] = nova;

    printf("\nNova peça gerada: %s (ID %d)\n", nova.nome, nova.id);

    f->fim = (f->fim + 1) % TAM_FILA;
    f->quantidade++;
}

// ------------------- PUSH: RESERVAR PEÇA -------------------
void reservarPeca(Fila *f, Pilha *p) {
    if (p->topo == TAM_PILHA - 1) {
        printf("\nA pilha está cheia! Não é possível reservar.\n");
        return;
    }
    if (f->quantidade == 0) {
        printf("\nA fila está vazia! Nada para reservar.\n");
        return;
    }

    Peca reservada = f->fila[f->inicio];
    p->itens[++p->topo] = reservada;

    printf("\nPeça reservada: %s (ID %d)\n", reservada.nome, reservada.id);

    jogarPeca(f);   // remove da fila
    inserirPeca(f); // fila continua sempre cheia
}

// ------------------- POP: USAR PEÇA RESERVADA -------------------
void usarPecaReservada(Fila *f, Pilha *p) {
    if (p->topo == -1) {
        printf("\nNenhuma peça reservada para usar!\n");
        return;
    }

    Peca usada = p->itens[p->topo--];

    printf("\nUsando peça reservada: %s (ID %d)\n",
           usada.nome,
           usada.id);
}

// ------------------- MENU -------------------
int menu() {
    int opc;
    printf("\n========= MENU =========\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opc);
    return opc;
}

// ------------------- MAIN -------------------
int main() {
    srand(time(NULL));

    Fila f;
    Pilha p;

    inicializarFila(&f);
    inicializarPilha(&p);

    int opc;
    do {
        opc = menu();

        switch (opc) {
        case 1:
            jogarPeca(&f);
            inserirPeca(&f);
            break;

        case 2:
            reservarPeca(&f, &p);
            break;

        case 3:
            usarPecaReservada(&f, &p);
            break;

        case 0:
            printf("\nEncerrando...\n");
            break;

        default:
            printf("\nOpção inválida!\n");
        }

        // Mostrar estado geral após cada ação
        mostrarFila(&f);
        mostrarPilha(&p);

    } while (opc != 0);

    return 0;
}
