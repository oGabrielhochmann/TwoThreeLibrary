#ifndef TWO_THREE_TREE_H
#define TWO_THREE_TREE_H
typedef struct Node {
    int nKeys;        // Número de chaves no nó

    int left_key;      // Chave da esquerda
    int right_key;     // Chave da direita

    int leftBook;      // Posição do livro (dado) da esquerda
    int rightBook;     // Posição do livro (dado) da direita

    int left_child;    // Filho da esquerda
    int middle_child;  // Filho do meio
    int right_child;   // Filho da direita
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