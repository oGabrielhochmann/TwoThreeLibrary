#ifndef INDEX_FREE_LIST_H
#define INDEX_FREE_LIST_H

// Lista de Nós (Páginas) Livres
typedef struct IndexFreeNode {
    int offset;                 // Endereço da página (nó) livre no arquivo de índice.
    struct IndexFreeNode *next;  // Ponteiro para o próximo nó na lista.
};

#endif /* INDEX_FREE_LIST_H */