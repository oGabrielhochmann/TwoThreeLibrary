#ifndef INDEX_FILE_HEADER_H
#define INDEX_FILE_HEADER_H

// Cabeçalho do Arquivo de Índices (Árvore 2-3)
typedef struct {
    int rootAddress;        // Endereço (deslocamento/offset) do registro raiz no arquivo de índices.
    int firstEmptyPosition; // Posição do primeiro espaço livre no arquivo de índices
    int headEmptyPosition;  // Endereço (deslocamento/offset) do início da lista de nós (páginas) livres.
} indexFileHeader;

#endif /* INDEX_FILE_HEADER_H */