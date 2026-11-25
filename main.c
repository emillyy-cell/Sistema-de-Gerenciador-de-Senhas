/* Sistema de senhas de atendimento clinico
   Ordem de chamada: Urgente (U), Preferencial (P), Normal (N).
   Dentro de cada categoria: quem chegou primeiro e chamado primeiro (FIFO). */
#include <stdio.h>

#define MAX 100

char letras[3] = {'U', 'P', 'N'};   /* 0 = Urgente, 1 = Preferencial, 2 = Normal */
int fila[3][MAX];                   /* numeros das senhas de cada categoria */
int inicio[3], fim[3];              /* inicio = quantos ja foram chamados, fim = quantos foram emitidos */
int atualCat = -1, atualNum;        /* senha em atendimento (-1 = ninguem) */

/* devolve 0 (Urgente), 1 (Preferencial) ou 2 (Normal) */
int classificar(char genero, int idade, int gest, int lact, int esp, int ob) {
    if (idade > 80) return 0;                       /* Urgente vem primeiro */
    if (gest || lact || esp || ob) return 1;
    if (genero == 'F' && idade >= 60) return 1;
    if (genero == 'M' && idade >= 65) return 1;
    return 2;
}

void cadastrar() {
    char genero;
    int idade, gest, lact, esp, ob, c;

    printf("Genero (M/F): ");            scanf(" %c", &genero);
    printf("Idade: ");                   scanf("%d", &idade);
    printf("Gestante (1=sim 0=nao): ");  scanf("%d", &gest);
    printf("Lactante (1=sim 0=nao): ");  scanf("%d", &lact);
    printf("Necessidade especial (1=sim 0=nao): "); scanf("%d", &esp);
    printf("Obesidade (1=sim 0=nao): "); scanf("%d", &ob);

    if (genero == 'm') genero = 'M';
    if (genero == 'f') genero = 'F';

    c = classificar(genero, idade, gest, lact, esp, ob);
    if (fim[c] == MAX) {
        printf("Fila cheia!\n");
        return;
    }
    fim[c]++;                        /* proximo numero da categoria */
    fila[c][fim[c] - 1] = fim[c];    /* entra no final da fila */
    printf("Senha emitida: %c%03d\n", letras[c], fim[c]);
}

/* primeira fila que nao esta vazia (U, depois P, depois N); -1 se todas vazias */
int proximaFila() {
    for (int i = 0; i < 3; i++)
        if (inicio[i] < fim[i]) return i;
    return -1;
}

void consultar() {
    int c = proximaFila();
    if (c == -1) printf("Nao ha pacientes em espera.\n");
    else printf("Proxima senha: %c%03d\n", letras[c], fila[c][inicio[c]]);
}

void chamar() {
    int c = proximaFila();
    if (atualCat != -1) {
        printf("Finalize o atendimento atual primeiro.\n");
    } else if (c == -1) {
        printf("Nao ha pacientes em espera.\n");
    } else {
        atualCat = c;
        atualNum = fila[c][inicio[c]];
        inicio[c]++;                 /* sai da fila, nao volta mais */
        printf("CHAMANDO: %c%03d\n", letras[c], atualNum);
    }
}

void emAtendimento() {
    if (atualCat == -1) printf("Nenhuma senha em atendimento.\n");
    else printf("Em atendimento: %c%03d\n", letras[atualCat], atualNum);
}

void finalizar() {
    if (atualCat == -1) {
        printf("Nenhuma senha em atendimento.\n");
    } else {
        printf("Atendimento finalizado: %c%03d\n", letras[atualCat], atualNum);
        atualCat = -1;
    }
}

void painel() {
    int mostradas = 0;

    printf("\n===== PAINEL =====\n");
    if (atualCat == -1) printf("Senha chamada: ---\n");
    else printf("Senha chamada: %c%03d\n", letras[atualCat], atualNum);

    printf("Proximas: ");
    for (int c = 0; c < 3; c++)
        for (int k = inicio[c]; k < fim[c] && mostradas < 5; k++) {
            printf("%c%03d ", letras[c], fila[c][k]);
            mostradas++;
        }
    if (mostradas == 0) printf("(nenhuma)");

    printf("\nEm espera: Urgente=%d | Preferencial=%d | Normal=%d\n",
           fim[0] - inicio[0], fim[1] - inicio[1], fim[2] - inicio[2]);
    printf("==================\n");
}

int main() {
    int op;
    do {
        printf("\n1-Cadastrar  2-Proxima  3-Chamar  4-Em atendimento\n");
        printf("5-Finalizar  6-Painel   0-Sair\nOpcao: ");
        scanf("%d", &op);
        switch (op) {
            case 1: cadastrar(); break;
            case 2: consultar(); break;
            case 3: chamar(); break;
            case 4: emAtendimento(); break;
            case 5: finalizar(); break;
            case 6: painel(); break;
            case 0: printf("Encerrando.\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
    return 0;
}
