#include <stdio.h>
#include <stdlib.h>

typedef struct nodo{
    int valor;
    struct nodo *esq, *dir;
}*Abn;
  
Abn new (int raiz, Abn e, Abn d){
    Abn r= malloc(sizeof(struct nodo));
    r-> valor = raiz;
    r -> esq = e;
    r-> dir = d;
    return r;
}

Abn FromArray (int v[], int N){
    Abn r;
    int m;
    Abn e, d;
    if (N == 0) r = NULL;
    else {
        m = N/2;
        r = new (v[m], NULL, NULL);
        e = FromArray (v, m);
        d = FromArray (v+m+1, N-m-1);
        r = new (v[m], e, d);
    }
    return r;
}

Abn procura (Abn a, int c){
    Abn r;
    if (a== NULL) r = NULL;
    else if (a-> valor == c) r=a;
    else if (a ->valor > c )r = procura(a-> valor , c);
    else r = procura(a-> dir, c);

    return r;
}

Abn procura_2 (Abn a, int c){
    while(a!= NULL && a-> valor != c){
        if (a-> valor > c) a= a->esq;
        else a = a->dir;
    }
    return a;
}

int tamanho (Abn a){
    if (a == NULL) return 0;
    else return (1 + tamanho(a-> esq) + tamanho(a-> dir));
}

int max (int a, int b){
    if (a > b) return a;
    else return b;
}

int altura (Abn a){
    if (a == NULL) return 0;
    else return (1 + max (altura(a-> esq), altura(a-> dir)));
}

Abn fundo (Abn a){
    if (a == NULL) return NULL; // Se a árvore for vazia, retorna NULL
    
    // Se o nó atual não tem filhos, ele é o fundo
    if (a->esq == NULL && a->dir == NULL) {
        return a;
    }
    
    // Percorre recursivamente os filhos
    Abn fundo_esq = fundo(a->esq); // Busca na subárvore esquerda
    Abn fundo_dir = fundo(a->dir); // Busca na subárvore direita
    
    // Retorna o primeiro nó folha encontrado (priorizando a esquerda)
    return (fundo_esq != NULL) ? fundo_esq : fundo_dir;
    
}
Abn insere (Abn a, int c){
    if (a == NULL) a = new (c, NULL, NULL);
    else if (a->valor > c) a->esq= insere(a->esq, c);
    else a->dir = insere (a->dir, c);
    return a;
}

Abn insere_2 (Abn a, int c){
    Abn r = a, nova, ant = NULL;
    nova = new (c, NULL, NULL);
    while (a!= NULL){
        ant = a;
        if (a->valor > c) a= a->esq;
        else a = a->dir;
    }
    if (ant ==NULL) r = nova;
    else if (ant-> valor > c) ant -> esq = nova;
    else ant->dir = nova;

    return r;
}

Abn insere_3 (Abn a, int c){
    Abn *p; // é um indereço nde uma seta está guardada
    p= &a;
    while (*p != NULL){
        if ((*p)-> valor > c){
            p= &((*p)->esq);
        }else p= &((*p)-> dir);
    }
    *p = new (c, NULL, NULL);
    return a;
}
  
void toArray (Abn a, int v[], int N){
    
    if (a == NULL || N == 0) return 0;
    int i = 0;
    v[i++] = a->valor;
    i += toArray (a->esq, v + i, N - i);
    i += toArray (a->dir, v + i, N - i);
    return i;
}

Abn removeMaior (Abn a, Abn *rem){
    Abn tmp;
    if (a-> dir == NULL){
        tmp = a;
        a = a-> esq;
        *rem = tmp;
    }else a->dir = removeMaior (a->dir, rem);

    return a;
}

Abn remove (Abn a, int c){
    Abn tmp;
    if (a == NULL) return NULL;
    if (a-> valor >c) a->esq = remove(a-> esq, c);
    else if (a -> valor < c) a->dir = remove (a->dir, c);
    else if (a-> dir == NULL) {
        tmp = a;
        a = a-> esq;
        free(tmp);
    } else if (a-> esq == NULL){
        tmp = a;
        a= a->dir;
        free(tmp);
    }else { 
        Abn maior = NULL;
        a-> esq = removeMaior(a->esq, &maior);
        maior->esq = a->esq;
        maior->dir = a->dir;
        free(a);
        a-> maior;
    }
    
    return a;
}

int main() {
    // Criando uma árvore binária
    Abn a = NULL;

    // Testando a função insere
    printf("Inserindo valores na árvore...\n");
    a = insere(a, 10);
    a = insere(a, 5);
    a = insere(a, 15);
    a = insere(a, 3);
    a = insere(a, 7);
    a = insere(a, 12);
    a = insere(a, 18);

    // Testando a função tamanho
    printf("Tamanho da árvore: %d\n", tamanho(a));

    // Testando a função altura
    printf("Altura da árvore: %d\n", altura(a));

    // Testando a função fundo
    Abn no_fundo = fundo(a);
    if (no_fundo != NULL) {
        printf("O fundo da árvore tem o valor: %d\n", no_fundo->valor);
    } else {
        printf("A árvore está vazia.\n");
    }

    // Testando a função procura
    int valor_procurado = 7;
    Abn encontrado = procura(a, valor_procurado);
    if (encontrado != NULL) {
        printf("Valor %d encontrado na árvore.\n", valor_procurado);
    } else {
        printf("Valor %d não encontrado na árvore.\n", valor_procurado);
    }

    // Testando a função remove
    printf("Removendo o valor 5 da árvore...\n");
    a = remove(a, 5);
    printf("Tamanho da árvore após remoção: %d\n", tamanho(a));

    // Testando a função toArray
    int tamanho_array = tamanho(a);
    int *array = malloc(tamanho_array * sizeof(int));
    toArray(a, array, tamanho_array);
    printf("Árvore convertida para array: ");
    for (int i = 0; i < tamanho_array; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    free(array);

    // Testando a função FromArray
    int valores[] = {1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(valores) / sizeof(valores[0]);
    Abn nova_arvore = FromArray(valores, n);
    printf("Árvore criada a partir do array (tamanho): %d\n", tamanho(nova_arvore));

    // Testando a função insere_2
    printf("Inserindo valor 20 usando insere_2...\n");
    nova_arvore = insere_2(nova_arvore, 20);
    printf("Tamanho da nova árvore: %d\n", tamanho(nova_arvore));

    // Testando a função insere_3
    printf("Inserindo valor 25 usando insere_3...\n");
    nova_arvore = insere_3(nova_arvore, 25);
    printf("Tamanho da nova árvore: %d\n", tamanho(nova_arvore));

    // Testando a função removeMaior
    Abn maior = NULL;
    nova_arvore = removeMaior(nova_arvore, &maior);
    if (maior != NULL) {
        printf("Maior valor removido: %d\n", maior->valor);
        free(maior);
    }
    printf("Tamanho da nova árvore após remover o maior: %d\n", tamanho(nova_arvore));

    // Liberando memória
    printf("Liberando memória...\n");
    while (a != NULL) {
        a = remove(a, a->valor);
    }
    while (nova_arvore != NULL) {
        nova_arvore = remove(nova_arvore, nova_arvore->valor);
    }

    printf("Todos os testes foram concluídos.\n");
    return 0;
}