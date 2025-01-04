#ifndef TWO_THREE_TREE_H
#define TWO_THREE_TREE_H

#define LEFT_KEY 0
#define RIGHT_KEY 1
#define LEFT_CHILD 0
#define MIDDLE_CHILD 1
#define RIGHT_CHILD 2

typedef struct Node {
    int keys[2];      // 0: esquerda, 1: direita
    int nKeys;        // Número de chaves no nó
    int dataPointer;  // Posição do livro no arquivo de dados
    int children[3];  // Posições dos filhos no arquivo de índices (0: esquerda, 1: meio, 2: direita)
} Node23;

// Cabeçalho do Arquivo de Índices (Árvore 2-3)
typedef struct {
    int rootAddress;        // Endereço (deslocamento/offset) do registro raiz no arquivo de índices.
    int firstEmptyPosition; // Posição do primeiro espaço livre no arquivo de índices
    int headEmptyPosition;  // Endereço (deslocamento/offset) do início da lista de nós/páginas livres.
} IndexFileHeader;

// Lista de Nós (Páginas) Livres
typedef struct IndexFreeNode {
    int offset; // Endereço da página (nó) livre no arquivo de índice.
    int next;   // Ponteiro para o próximo nó na lista.
} IndexFreeNode;

#endif /* TWO_THREE_TREE_H */