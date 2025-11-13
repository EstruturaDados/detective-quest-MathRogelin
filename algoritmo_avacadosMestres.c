#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===================== ESTRUTURA ===================== */
/* Estrutura que representa uma sala (nó da árvore binária) */
typedef struct Sala {
    char nome[50];
    char pista[100]; // Nova: cada sala pode ter uma pista associada
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* Estrutura da árvore de pistas (BST) */
typedef struct PistaNode {
    char texto[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/* ===================== HASH (Pista -> Suspeito) ===================== */

#define HASH_SIZE 31

/* Nó da tabela hash que associa uma pista a um suspeito */
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *next;
} HashNode;

/* calculaHash()
   Função simples de hashing: soma dos códigos ASCII mod HASH_SIZE */
unsigned int calculaHash(const char *s) {
    unsigned int soma = 0;
    for (size_t i = 0; i < strlen(s); ++i) soma += (unsigned char)s[i];
    return soma % HASH_SIZE;
}

/* inicializarHash()
   Aloca e inicializa a tabela hash (array de ponteiros). */
HashNode** inicializarHash(void) {
    HashNode **tabela = (HashNode**) malloc(sizeof(HashNode*) * HASH_SIZE);
    if (tabela == NULL) {
        printf("Erro ao alocar memoria para a tabela hash.\n");
        exit(1);
    }
    for (int i = 0; i < HASH_SIZE; ++i) tabela[i] = NULL;
    return tabela;
}

/* inserirNaHash()
   Insere a associação (pista -> suspeito) na tabela hash.
   Usa encadeamento separado para tratar colisões. */
void inserirNaHash(HashNode **tabela, const char *pista, const char *suspeito) {
    if (pista == NULL || strlen(pista) == 0 || suspeito == NULL) return;
    unsigned int idx = calculaHash(pista);
    // inserção no início da lista do bucket
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) {
        printf("Erro ao alocar memoria para HashNode.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->next = tabela[idx];
    tabela[idx] = novo;
}

/* encontrarSuspeito()
   Busca na tabela hash o suspeito associado à pista dada.
   Retorna ponteiro para o nome do suspeito (string interna) ou NULL se não encontrada. */
const char* encontrarSuspeito(HashNode **tabela, const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return NULL;
    unsigned int idx = calculaHash(pista);
    HashNode *cur = tabela[idx];
    while (cur != NULL) {
        if (strcmp(cur->pista, pista) == 0) return cur->suspeito;
        cur = cur->next;
    }
    return NULL;
}

/* liberarHash()
   Libera toda a memória usada pela tabela hash. */
void liberarHash(HashNode **tabela) {
    if (tabela == NULL) return;
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode *cur = tabela[i];
        while (cur != NULL) {
            HashNode *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free(tabela);
}

/* ===================== FUNÇÕES ===================== */

/* criaSala()
   Cria dinamicamente uma nova sala com o nome especificado. */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memoria para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    if (pista != NULL)
        strcpy(novaSala->pista, pista);
    else
        strcpy(novaSala->pista, ""); // sem pista
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/* inserirPista()
   Insere uma nova pista na árvore BST em ordem alfabética. */
PistaNode* inserirPista(PistaNode *raiz, const char *texto) {
    if (texto == NULL || strlen(texto) == 0) return raiz; // ignora se for vazia

    if (raiz == NULL) {
        PistaNode *nova = (PistaNode*) malloc(sizeof(PistaNode));
        if (!nova) {
            printf("Erro ao alocar memoria para PistaNode.\n");
            exit(1);
        }
        strcpy(nova->texto, texto);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }

    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else if (strcmp(texto, raiz->texto) > 0)
        raiz->direita = inserirPista(raiz->direita, texto);

    return raiz;
}

/* exibirPistas()
   Exibe as pistas coletadas em ordem alfabética (in-order traversal). */
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->texto);
    exibirPistas(raiz->direita);
}

/* liberarBST()
   Libera toda a árvore de pistas. */
void liberarBST(PistaNode *raiz) {
    if (raiz == NULL) return;
    liberarBST(raiz->esquerda);
    liberarBST(raiz->direita);
    free(raiz);
}

/* contarPistasParaSuspeito()
   Percorre a BST e conta quantas pistas coletadas apontam para o suspeito indicado
   usando a tabela hash (pista -> suspeito). */
int contarPistasParaSuspeito(PistaNode *raiz, HashNode **tabela, const char *suspeito) {
    if (raiz == NULL) return 0;
    int contador = 0;
    // percorre esquerda
    contador += contarPistasParaSuspeito(raiz->esquerda, tabela, suspeito);
    // verifica o nó atual
    const char *s = encontrarSuspeito(tabela, raiz->texto);
    if (s != NULL && strcmp(s, suspeito) == 0) contador++;
    // percorre direita
    contador += contarPistasParaSuspeito(raiz->direita, tabela, suspeito);
    return contador;
}

/* verificarSuspeitoFinal()
   Solicita ao jogador o nome do suspeito acusado e verifica se há pelo menos
   duas pistas coletadas que apontam para esse suspeito. */
void verificarSuspeitoFinal(PistaNode *pistasColetadas, HashNode **tabela) {
    if (pistasColetadas == NULL) {
        printf("\nNenhuma pista coletada - não é possível acusar ninguém.\n");
        return;
    }

    char acusado[50];
    printf("\nQuem você acusa? (digite o nome exato do suspeito): ");
    // lê uma linha segura
    if (fgets(acusado, sizeof(acusado), stdin) == NULL) {
        printf("Erro na leitura.\n");
        return;
    }
    // remove '\n' final
    acusado[strcspn(acusado, "\r\n")] = '\0';

    if (strlen(acusado) == 0) {
        printf("Nenhum nome fornecido. Acusação cancelada.\n");
        return;
    }

    int correspondencias = contarPistasParaSuspeito(pistasColetadas, tabela, acusado);

    printf("\nPistas que apontam para %s: %d\n", acusado, correspondencias);
    if (correspondencias >= 2) {
        printf("Acusação válida! Há evidências suficientes para prender %s.\n", acusado);
    } else {
        printf("Acusação insuficiente. São necessárias pelo menos 2 pistas para uma acusação válida.\n");
    }
}

/* explorarSalasComPistas()
   Permite que o jogador navegue pela mansão interativamente.
   O jogador escolhe 'e' para esquerda, 'd' para direita ou 's' para sair.
   Cada sala visitada adiciona sua pista (se existir) à BST. */
void explorarSalasComPistas(Sala *salaAtual, PistaNode **pistas) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        // coleta automática da pista da sala
        if (strlen(salaAtual->pista) > 0) {
            printf("Você encontrou uma pista: \"%s\"\n", salaAtual->pista);
            *pistas = inserirPista(*pistas, salaAtual->pista);
        }

        printf("Escolha um caminho:\n");
        if (salaAtual->esquerda != NULL) printf(" (e) Ir para %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL) printf(" (d) Ir para %s\n", salaAtual->direita->nome);
        printf(" (s) Sair do jogo\n");
        printf(">> ");
        scanf(" %c", &escolha);
        // consome newline restante antes de futuras fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { /* limpa buffer */ }

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } 
        else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } 
        else if (escolha == 's') {
            printf("\nVocê decidiu encerrar a exploração.\n");
            break;
        } 
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

/* liberarArvore()
   Libera a memória de todas as salas da árvore. */
void liberarArvore(Sala *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}


// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

int main() {
    
    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    /* Montagem automática da mansão (árvore binária) */
    Sala *hall = criarSala("Hall de Entrada", "Pegadas misteriosas no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "Um copo quebrado no chão");
    Sala *cozinha = criarSala("Cozinha", "Uma colher suja de veneno");
    Sala *biblioteca = criarSala("Biblioteca", "Um livro rasgado sobre venenos");
    Sala *jardim = criarSala("Jardim", "");
    Sala *porao = criarSala("Porao", "Uma luva ensanguentada");
    Sala *quarto = criarSala("Quarto Principal", "Perfume forte no travesseiro");

    // Estrutura da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = porao;
    cozinha->direita = quarto;

    printf("=== DETECTIVE QUEST ===\n");
    printf("Bem-vindo à mansão misteriosa!\n");
    printf("Explore os cômodos, colete pistas e descubra o culpado.\n");

    // Inicializa a tabela hash e preenche as associações pista -> suspeito
    HashNode **tabela = inicializarHash();
    // Mapeamento estático (defina conforme sua história)
    inserirNaHash(tabela, "Pegadas misteriosas no tapete", "Herdeiro");
    inserirNaHash(tabela, "Um copo quebrado no chão", "Empregada");
    inserirNaHash(tabela, "Uma colher suja de veneno", "Chefe de Cozinha");
    inserirNaHash(tabela, "Um livro rasgado sobre venenos", "Chefe de Cozinha");
    inserirNaHash(tabela, "Uma luva ensanguentada", "Jardineiro");
    inserirNaHash(tabela, "Perfume forte no travesseiro", "Herdeiro");

    // Inicia exploração e coleta de pistas
    PistaNode *pistasColetadas = NULL;
    explorarSalasComPistas(hall, &pistasColetadas);

    // Exibe pistas coletadas em ordem alfabética
    printf("\n=== PISTAS COLETADAS ===\n");
    if (pistasColetadas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistas(pistasColetadas);

    // Fase de acusação: pede ao jogador para acusar um suspeito e verifica se há evidências
    verificarSuspeitoFinal(pistasColetadas, tabela);

    // Libera memória
    liberarArvore(hall);
    liberarBST(pistasColetadas);
    liberarHash(tabela);

    printf("\nObrigado por jogar!\n");
    return 0;
}
