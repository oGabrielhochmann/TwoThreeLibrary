/**
 * @file  book_data_file.h
 * 
 * @author Gabriel Hochmann
 * 
 * @brief Contains the data structures for the data file.
 */

#ifndef BOOK_DATA_FILE_H
#define BOOK_DATA_FILE_H

/**
 * @brief Estruturas de Dados para o cabeçalho do arquivo de dados.
 * 
 * O cabeçalho do arquivo de dados contém informações sobre o arquivo de dados, como:
 * - firstEmptyPosition: Endereço (deslocamento/offset) da primeira posição livre do arquivo de dados.
 * - headEmptyPosition: Endereço (deslocamento/offset) da cabeça dos registros de dados livres.
 */
typedef struct {
    int firstEmptyPosition;
    int headEmptyPosition;
} BookDataFileHeader;

/**
 * @brief Estrutura de Dados para uma lista encadeada de registros de dados livres.
 * 
 * Cada nó da lista de registros de dados livres contém:
 * - offset: Endereço do registro livre no arquivo de dados.
 * - nextOffset: Offset para o próximo nó na lista de registros livres (ou  -1 se não houver próximo).
 */
typedef struct DataFreeNode
{
    int offset;     
    int nextOffset; 
} BookDataFreeNode;

#endif /* BOOK_DATA_FILE_H */