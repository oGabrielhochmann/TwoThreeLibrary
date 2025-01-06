#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

// tree operations

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
Node23 loadNode23(FILE *indexFile, int offset);

#endif /* TREE_MANAGER_H */