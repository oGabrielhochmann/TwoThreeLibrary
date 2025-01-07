/**
 * @file two_three_tree.h
 *
 * @brief Contém a estrutura de dados para uma Árvore 2-3.
 *
 * Este arquivo contém a estrutura de dados para uma Árvore 2-3, que é utilizada para indexar os registros de dados.

 * @author Gabriel Hochmann
 */

#ifndef TWO_THREE_TREE_H
#define TWO_THREE_TREE_H

/**
 * @brief Estrutura de Dados para um Nó de uma Árvore 2-3.
 *
 * Cada nó de uma Árvore 2-3 contém:
 * - nKeys: Número de chaves no nó.
 * - left_key: Chave da esquerda.
 * - right_key: Chave da direita.
 * - leftBook: Posição do livro (dado) da esquerda.
 * - rightBook: Posição do livro (dado) da direita.
 * - left_child: Filho da esquerda.
 * - middle_child: Filho do meio.
 * - right_child: Filho da direita.
 */
typedef struct Node
{
    int nKeys;

    int left_key;
    int right_key;

    int leftBook;
    int rightBook;

    int left_child;
    int middle_child;
    int right_child;
} Node23;

/**
 * @brief Estrutura de Dados para o cabeçalho do arquivo de índices.
 *
 * O cabeçalho do arquivo de índices contém informações sobre o arquivo de índices, como:
 * - rootAddress: Endereço (deslocamento/offset) do registro raiz no arquivo de índices.
 * - firstEmptyPosition: Posição do primeiro espaço livre no arquivo de índices.
 * - headEmptyPosition: Endereço (deslocamento/offset) do início da lista de nós/páginas livres.
 *
 * O cabeçalho do arquivo de índices é armazenado no início do arquivo de índices.
 */
typedef struct
{
    int rootAddress;        // Endereço (deslocamento/offset) do registro raiz no arquivo de índices.
    int firstEmptyPosition; // Posição do primeiro espaço livre no arquivo de índices
    int headEmptyPosition;  // Endereço (deslocamento/offset) do início da lista de nós/páginas livres.
} IndexFileHeader;

/**
 * @brief Estrutura de Dados para uma lista encadeada de nós/páginas livres.
 *
 * Cada nó da lista de nós/páginas livres contém:
 * - offset: Endereço da página (nó) livre no arquivo de índice.
 * - next: Ponteiro para o próximo nó na lista de nós/páginas livres.
 *
 * Essa lista é utilizada para gerenciar os espaços livres no arquivo de índice.
 *
 * A lista de nós/páginas livres é armazenada no início do arquivo de índice.
 */
typedef struct IndexFreeNode
{
    int offset; // Endereço da página (nó) livre no arquivo de índice.
    int next;   // Ponteiro para o próximo nó na lista.
} IndexFreeNode;

#endif /* TWO_THREE_TREE_H */
