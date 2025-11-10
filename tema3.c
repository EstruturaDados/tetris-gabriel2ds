#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente, tras, qtd;
} Fila;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

char gerarTipo() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca(int id) {
    Peca nova;
    nova.nome = gerarTipo();
    nova.id = id;
    return nova;
}

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = p;
    f->qtd++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;
    return p;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void empilhar(Pilha *p, Peca item) {
    if (pilhaCheia(p)) return;
    p->pecas[++p->topo] = item;
}

Peca desempilhar(Pilha *p) {
    Peca item = {'-', -1};
    if (pilhaVazia(p)) return item;
    return p->pecas[p->topo--];
}

void exibirFila(Fila *f) {
    printf("Fila de peças: ");
    if (filaVazia(f)) {
        printf("(vazia)\n");
        return;
    }
    int i = f->frente;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n");
}

void trocarTopoFilaComPilha(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Impossível trocar: fila ou pilha vazia.\n");
        return;
    }
    Peca temp = f->pecas[f->frente];
    f->pecas[f->frente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
    printf("Troca simples realizada com sucesso!\n");
}

void trocarTresPrimeiros(Fila *f, Pilha *p) {
    if (f->qtd < 3 || p->topo < 2) {
        printf("Impossível trocar: é necessário ter ao menos 3 peças em ambas as estruturas.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int indexFila = (f->frente + i) % TAM_FILA;
        Peca temp = f->pecas[indexFila];
        f->pecas[indexFila] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
    }
    printf("Troca múltipla realizada com sucesso!\n");
}

int main() {
    srand(time(NULL));
    Fila fila;
    Pilha pilha;
    int id = 0, opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(id++));
    }

    do {
        printf("\n===== ESTADO ATUAL =====\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Reservar peça (mover para pilha)\n");
        printf("3 - Usar peça reservada (remover do topo da pilha)\n");
        printf("4 - Trocar peça atual (fila <-> pilha)\n");
        printf("5 - Trocar 3 primeiras da fila com pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1) {
                    printf("Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(id++));
                }
                break;
            }
            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia! Não é possível reservar mais.\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    if (reservada.id != -1) {
                        empilhar(&pilha, reservada);
                        printf("Peça [%c %d] reservada!\n", reservada.nome, reservada.id);
                        enfileirar(&fila, gerarPeca(id++));
                    }
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("Pilha vazia! Nenhuma peça para usar.\n");
                } else {
                    Peca usada = desempilhar(&pilha);
                    printf("Peça [%c %d] usada!\n", usada.nome, usada.id);
                }
                break;
            }
            case 4:
                trocarTopoFilaComPilha(&fila, &pilha);
                break;
            case 5:
                trocarTresPrimeiros(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}
