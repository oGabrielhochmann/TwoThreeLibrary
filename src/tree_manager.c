#include "tree_manager.h"
#include "two_three_tree.h"

#include <stdio.h>

// tree operations (insert, search, remove, etc)

/**
 * @brief Cria um novo nó 2-3.
 * 
 * Esta função é responsável por criar um novo nó 2-3 no arquivo de índices, buscando
 * um nó livre na lista de nós livres, ou criando um novo nó no final do arquivo.
 * 
 * @param indexFile Ponteiro para o arquivo de índices.
 * @param node Ponteiro para o nó a ser criado.
 * @param freeList Ponteiro para a lista de nós livres.
 * 
 * @return Deslocamento (offset) do nó criado no arquivo de índices.
 * 
 * @note Se um nó livre estiver disponível, ele será utilizado para criar o novo nó.
 */
int createNode23(FILE *indexFile, Node23 *node, IndexFreeNode **freeList)
{
    int nodeOffset;

    if (*freeList)
    {
        nodeOffset = (*freeList)->offset;
        *freeList = (*freeList)->next;
    }
    else
    {
        fseek(indexFile, 0, SEEK_END);
        nodeOffset = ftell(indexFile);
    }

    fseek(indexFile, nodeOffset, SEEK_SET);
    fwrite(node, sizeof(Node23), 1, indexFile);

    return nodeOffset;
}

/**
 * @brief Carrega um nó do arquivo de índices.
 *
 * Esta função é responsável por carregar um nó a partir de um arquivo de índices existente,
 * dado um deslocamento (offset) no arquivo.
 *
 * @param indexFile Ponteiro para o arquivo de índices.
 * @param offset Deslocamento (offset) do nó no arquivo de índices.
 *
 * @return Nó carregado do arquivo de índices.
 *
 * @note O nó é carregado a partir do deslocamento especificado no arquivo de índices.
 */
Node23 loadNode23(FILE *indexFile, int offset)
{
    Node23 node;

    fseek(indexFile, offset, SEEK_SET);
    fread(&node, sizeof(node), 1, indexFile);

    return node;
}

/**
 * @brief Salva um nó no arquivo de índices.
 *
 * Esta função é responsável por salvar um nó no arquivo de índices.
 *
 * @param indexFile Ponteiro para o arquivo de índices.
 * @param offset Deslocamento do nó no arquivo de índices.
 * @param node Ponteiro para o nó a ser salvo.
 *
 * @return Nenhum.
 */
void saveNode(FILE *indexFile, int offset, Node23 *node)
{
    fseek(indexFile, offset, SEEK_SET);
    fwrite(node, sizeof(Node23), 1, indexFile);
}