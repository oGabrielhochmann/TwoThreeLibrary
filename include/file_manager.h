#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>

//  This includes opening, reading, writing, and managing free spaces in the binary files.

/**
 * @brief Abre um arquivo no modo especificado.
 *
 * Esta função é responsável por abrir um arquivo no modo especificado e verificar se a operação foi bem-sucedida.
 *
 * @pre `filename` não deve ser NULL.
 * @pre `mode` não deve ser NULL.
 *
 * @post O arquivo é aberto no modo especificado e um ponteiro para o arquivo é retornado.
 *
 * @param filename Nome do arquivo a ser aberto.
 * @param mode Modo de abertura do arquivo.
 *
 * @return Ponteiro para o arquivo aberto, ou NULL se ocorrer um erro.
 */
FILE *openFile(const char *filename, const char *mode);

/**
 * @brief Fecha um arquivo previamente aberto.
 *
 * Esta função é responsável por fechar um arquivo previamente aberto e verificar se a operação foi bem-sucedida.
 *
 * @pre `file` não deve ser NULL.
 * @pre `*file` não deve ser NULL.
 *
 * @post O arquivo é fechado e o ponteiro para o arquivo é definido como NULL.
 *
 * @param file Ponteiro para o arquivo a ser fechado.
 *
 * @return 0 se o arquivo foi fechado com sucesso, -1 caso contrário.
 *
 * @note Se ocorrer um erro ao fechar o arquivo, uma mensagem de erro será exibida.
 */
int closeFile(FILE **file);

#endif /* FILE_MANAGER_H */