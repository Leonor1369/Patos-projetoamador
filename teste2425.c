#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    int inf,sup;
} Intervalo;

void ordena ( Intervalo c[], int N){
    int i, j;
    Intervalo temp;
    
    // Bubble sort para ordenar por limite inferior
    for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - i - 1; j++) {
            if (c[j].inf > c[j + 1].inf) {
                temp = c[j];
                c[j] = c[j + 1];
                c[j + 1] = temp;
            }
        }
    }
}

int cardinalidade(Intervalo c[], int N){
    if (N == 0) return 0;
    
    // Encontrar o valor máximo para dimensionar o array
    int max_val = 0;
    for (int i = 0; i < N; i++) {
        if (c[i].sup > max_val) max_val = c[i].sup;
    }
    
    // Array para marcar números presentes (inicializado com 0s)
    int aux[max_val + 1];
    for (int i = 0; i <= max_val; i++) {
        aux[i] = 0;
    }
    
    // Marcar todos os números de todos os intervalos
    for (int i = 0; i < N; i++) {
        for (int j = c[i].inf; j <= c[i].sup; j++) {
            aux[j] = 1; // Marcar como presente
        }
    }
    
    // Contar quantos estão marcados
    int total = 0;
    for (int i = 0; i <= max_val; i++) {
        if (aux[i] == 1) total++;
    }
    
    return total;
}

void camel2snake(char *id){
    int len = strlen(id);
    char temp[1000]; // Buffer temporário
    int j = 0;
    
    for (int i = 0; i < len; i++) {
        if (isupper(id[i]) && i > 0) {
            temp[j++] = '_';
            temp[j++] = id[i] + 32; // Converter para minúscula
        } else {
            temp[j++] = id[i];
        }
    }
    temp[j] = '\0';
    
    strcpy(id, temp);
}

typedef struct no {
    int valor;
    struct no *prox;
} *LInt;

LInt arrayToList (int v[], int N) {
    if (N == 0) return NULL;
    
    LInt head = malloc(sizeof(struct no));
    head->valor = v[0];
    head->prox = NULL;
    
    LInt current = head;
    
    for (int i = 1; i < N; i++) {
        current->prox = malloc(sizeof(struct no));
        current = current->prox;
        current->valor = v[i];
        current->prox = NULL;
    }
    
    return head;
}

int apagaUltimo(LInt *l, int x){
    if (*l == NULL) return -1; // Lista vazia
    
    LInt current = *l;
    LInt last_occurrence = NULL;
    LInt before_last = NULL;
    LInt prev = NULL;
    
    // Encontrar a última ocorrência
    while (current != NULL) {
        if (current->valor == x) {
            before_last = prev;
            last_occurrence = current;
        }
        prev = current;
        current = current->prox;
    }
    
    if (last_occurrence == NULL) return -1; // Elemento não encontrado
    
    // Remover o elemento
    if (before_last == NULL) {
        // É o primeiro elemento
        *l = last_occurrence->prox;
    } else {
        before_last->prox = last_occurrence->prox;
    }
    
    free(last_occurrence);
    return 0; // Sucesso
}

typedef struct nodo {
int valor;
struct nodo *esq, *dir;
} *ABin;

int encontraCaminho(ABin a, int x, int caminho[], int *tamanho) {
    if (a == NULL) return 0;
    
    caminho[*tamanho] = a->valor;
    (*tamanho)++;
    
    if (a->valor == x) return 1;
    
    if (x < a->valor) {
        if (encontraCaminho(a->esq, x, caminho, tamanho)) return 1;
    } else {
        if (encontraCaminho(a->dir, x, caminho, tamanho)) return 1;
    }
    
    (*tamanho)--;
    return 0;
}

int parentesco(ABin a, int x, int y){
    int caminho_x[1000], caminho_y[1000];
    int tamanho_x = 0, tamanho_y = 0;
    
    if (!encontraCaminho(a, x, caminho_x, &tamanho_x) || 
        !encontraCaminho(a, y, caminho_y, &tamanho_y)) {
        return -1; // Um dos valores não foi encontrado
    }
    
    // Encontrar o ancestral comum mais próximo
    int ancestral_comum = 0;
    int min_tamanho = (tamanho_x < tamanho_y) ? tamanho_x : tamanho_y;
    
    // Percorrer até encontrar diferença ou chegar ao fim
    while (ancestral_comum < min_tamanho && 
           caminho_x[ancestral_comum] == caminho_y[ancestral_comum]) {
        ancestral_comum++;
    }
    ancestral_comum--; // Voltar ao último índice comum
    
    // Calcular distância: distância de x ao ancestral + distância de y ao ancestral
    int distancia = (tamanho_x - 1 - ancestral_comum) + (tamanho_y - 1 - ancestral_comum);
    
    return distancia;
}

// Funções auxiliares para testes
void printIntervalo(Intervalo i) {
    printf("[%d..%d] ", i.inf, i.sup);
}

void printLista(LInt l) {
    while (l != NULL) {
        printf("%d ", l->valor);
        l = l->prox;
    }
    printf("\n");
}

// Função para criar nó da árvore
ABin criarNo(int valor) {
    ABin novo = malloc(sizeof(struct nodo));
    novo->valor = valor;
    novo->esq = novo->dir = NULL;
    return novo;
}

// Função para inserir na BST
ABin inserir(ABin a, int valor) {
    if (a == NULL) return criarNo(valor);
    
    if (valor < a->valor) {
        a->esq = inserir(a->esq, valor);
    } else if (valor > a->valor) {
        a->dir = inserir(a->dir, valor);
    }
    
    return a;
}

// Função de teste principal
int main() {
    printf("=== Teste das Soluções ===\n\n");
    
    // Teste 1: Ordenar intervalos
    printf("1. Teste ordenação de intervalos:\n");
    Intervalo intervalos[] = {{10, 11}, {1, 6}, {5, 8}};
    int n = 3;
    
    printf("Antes: ");
    for (int i = 0; i < n; i++) printIntervalo(intervalos[i]);
    printf("\n");
    
    ordena(intervalos, n);
    
    printf("Depois: ");
    for (int i = 0; i < n; i++) printIntervalo(intervalos[i]);
    printf("\n\n");
    
    // Teste 2: Cardinalidade
    printf("2. Teste cardinalidade:\n");
    Intervalo conj[] = {{1, 6}, {10, 11}};
    int card = cardinalidade(conj, 2);
    printf("Cardinalidade de [1..6] ∪ [10..11] = %d\n\n", card);
    
    // Teste 3: Conversão camelCase para snake_case
    printf("3. Teste conversão camelCase:\n");
    char id[] = "apagaTodosMenores";
    printf("Antes: %s\n", id);
    camel2snake(id);
    printf("Depois: %s\n\n", id);
    
    // Teste 4: Array para lista
    printf("4. Teste array para lista:\n");
    int arr[] = {1, 2, 3, 4, 5};
    LInt lista = arrayToList(arr, 5);
    printf("Lista criada: ");
    printLista(lista);
    
    // Teste 5: Apagar último
    printf("5. Teste apagar último:\n");
    printf("Lista antes: ");
    printLista(lista);
    int resultado = apagaUltimo(&lista, 3);
    printf("Resultado da remoção: %d\n", resultado);
    printf("Lista depois: ");
    printLista(lista);
    
    // Teste 6: Grau de parentesco
    printf("6. Teste grau de parentesco:\n");
    ABin arvore = NULL;
    int valores[] = {5, 3, 7, 1, 4, 6, 8};
    for (int i = 0; i < 7; i++) {
        arvore = inserir(arvore, valores[i]);
    }
    
    int grau = parentesco(arvore, 1, 8);
    printf("Grau de parentesco entre 1 e 8: %d\n", grau);
    
    return 0;
}