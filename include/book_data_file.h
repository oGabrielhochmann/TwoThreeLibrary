/**
 * @file  book_data_file.h
 
 * @brief Define as estruturas de dados para gerenciar livros em um arquivo.
 *
 * Este arquivo de cabeçalho contém as definições das estruturas usadas para manipular os metadados
 * e o gerenciamento de espaço livre de um arquivo de dados de livros. Inclui uma estrutura para
 * metadados gerais do arquivo e outra para uma lista encadeada de espaços livres.

 * @author Gabriel Hochmann
 */

#ifndef BOOK_DATA_FILE_H
#define BOOK_DATA_FILE_H

/**
 * @brief Estrutura de cabeçalho para os metadados do arquivo de dados de livros.
 *
 * Esta estrutura armazena metadados sobre o arquivo de dados de livros, incluindo:
 * - `firstEmptyPosition`: O deslocamento da primeira posição disponível para escrita de novos dados.
 * - `headEmptyPosition`: O deslocamento da cabeça da lista encadeada de blocos de dados livres.
 */
typedef struct
{
    int firstEmptyPosition; /**< Deslocamento da primeira posição livre no arquivo de dados. */
    int headEmptyPosition;  /**< Deslocamento da cabeça da lista de blocos livres. */
} BookDataFileHeader;

/**
 * @brief Estrutura de nó para uma lista encadeada de blocos de dados livres.
 *
 * Esta estrutura representa um nó em uma lista encadeada usada para gerenciar os blocos
 * de dados livres no arquivo de dados de livros. Cada nó contém:
 * - `offset`: O deslocamento do bloco livre no arquivo de dados.
 * - `nextOffset`: O deslocamento do próximo nó na lista de blocos livres, ou -1 se é o último nó.
 */
typedef struct DataFreeNode
{
    int offset;     /**< Deslocamento do bloco livre no arquivo de dados. */
    int nextOffset; /**< Deslocamento do próximo bloco livre, ou -1 se não houver outro. */
} BookDataFreeNode;

#endif /* BOOK_DATA_FILE_H */
