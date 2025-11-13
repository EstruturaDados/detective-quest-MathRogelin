#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===================== ESTRUTURA ===================== */
/* Estrutura que representa uma sala (nó da árvore binária) */
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* ===================== FUNÇÕES ===================== */

/* criaSala()
   Cria dinamicamente uma nova sala com o nome especificado. */
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memoria para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

/* explorarSalas()
   Permite que o jogador navegue pela mansão interativamente.
   O jogador escolhe 'e' para esquerda, 'd' para direita ou 's' para sair. */
void explorarSalas(Sala *salaAtual) {
    char escolha;

    while (1) {
        printf("\nVocê está em: %s\n", salaAtual->nome);

        // Caso o cômodo não tenha saídas, fim da exploração
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Fim da exploração.\n");
            break;
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
            printf("Você decidiu encerrar a exploração.\n");
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

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    /* Montagem automática da mansão (árvore binária) */
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porao");
    Sala *quarto = criarSala("Quarto Principal");

    // Estrutura da árvore
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->esquerda = porao;
    cozinha->direita = quarto;

    printf("=== DETECTIVE QUEST ===\n");
    printf("Bem-vindo à mansão misteriosa!\n");
    printf("Explore os cômodos e descubra o caminho.\n");

    // Inicia exploração
    explorarSalas(hall);

    // Libera memória
    liberarArvore(hall);

    printf("\nObrigado por jogar!\n");
    return 0;
}

// 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

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
