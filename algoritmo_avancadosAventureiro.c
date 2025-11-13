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
    
    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

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

    // Inicia exploração e coleta de pistas
    PistaNode *pistasColetadas = NULL;
    explorarSalasComPistas(hall, &pistasColetadas);

    // Exibe pistas coletadas em ordem alfabética
    printf("\n=== PISTAS COLETADAS ===\n");
    if (pistasColetadas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistas(pistasColetadas);

    // Libera memória
    liberarArvore(hall);
    liberarBST(pistasColetadas);

    printf("\nObrigado por jogar!\n");
    return 0;
}

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
