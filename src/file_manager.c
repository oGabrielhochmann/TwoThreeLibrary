/**
 * @file file_manager.c
 * @author Gabriel Hochmann
 * 
 * @brief Contém funções para gerenciamento de arquivos binários.
 * 
 * @see file_manager.h
 */

#include "file_manager.h"
#include "book_data_file.h"
#include "two_three_tree.h"

#include <errno.h>

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
FILE *openFile(const char *filename, const char *mode)
{

    FILE *file = fopen(filename, mode);

    if (!file)
    {
        fprintf(stderr, "Error, opening file: %s\n", filename);
        perror("Details");
        return NULL;
    }

    return file;
}

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
int closeFile(FILE **file)
{
    if (file && *file)
    {
        if (fclose(*file) == 0)
        {
            *file = NULL;
            return 0;
        }
        perror("Erro ao abrir o arquivo");
    }

    return -1;
}

/**
 * @brief Cria o cabeçalho do arquivo de índices.
 * 
 * Esta função é responsável por criar o cabeçalho do arquivo de índices.
 * 
 * @param file Ponteiro para o arquivo de índices.
 * 
 * @return Nenhum.
 * 
 * @note O cabeçalho do arquivo de índices é composto por três informações: a raiz da árvore, a primeira posição livre e a cabeça dos registros livres.
 */
void createIndexFileHeader(FILE *file)
{
    IndexFileHeader header;

    header.rootAddress = -1;       // Raiz da árvore (inicialmente vazia)
    header.firstEmptyPosition = 0; // Primeira posição livre 
    header.headEmptyPosition = -1; // Cabeça de registros livres (inicialmente sem registros livres)

    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, file);
}

/**
 * @brief Cria o cabeçalho do arquivo de dados.
 * 
 * Esta função é responsável por criar o cabeçalho do arquivo de dados.
 * 
 * @param file Ponteiro para o arquivo de dados.
 * 
 * @return Nenhum.
 * 
 * @note O cabeçalho do arquivo de dados é composto por duas informações: a primeira posição livre e a cabeça dos registros livres.
 */
void createBookDataFileHeader(FILE *file) {
    BookDataFileHeader header;

    header.firstEmptyPosition = 0; // Primeira posição livre
    header.headEmptyPosition = -1; // Cabeça de registros livres (inicialmente sem registros livres)

    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, file);
}

/**
 * @brief Lê o cabeçalho de um arquivo binário.
 * 
 * Esta função é responsável por ler o cabeçalho de um arquivo binário e armazená-lo
 * na estrutura fornecida pelo usuário.
 * 
 * @param file Ponteiro para o arquivo binário já aberto.
 * @param header Ponteiro para a estrutura onde o cabeçalho será armazenado.
 * @param headerSize Tamanho, em bytes, da estrutura do cabeçalho.
 * 
 * @return 1 se o cabeçalho foi lido com sucesso, -1 em caso de erro.
 * 
 * @note Certifique-se de que o arquivo já está aberto em modo de leitura binária antes de chamar esta função.
 *       O cabeçalho será lido a partir do início do arquivo.
 */
int readFileHeader(FILE *file, void *header, size_t headerSize) {
    fseek(file, 0, SEEK_SET);

    if (fread(header, headerSize, 1, file) != 1) {
        fprintf(stderr, "Erro ao ler o cabeçalho do arquivo.\n");
        return -1; // Erro ao ler o cabeçalho
    }

    return 1; // Sucesso
}

/**
 * @brief Salva o cabeçalho de um arquivo binário.
 * 
 * Esta função é responsável por salvar o cabeçalho de um arquivo binário.
 * 
 * @param file Ponteiro para o arquivo binário já aberto.
 * @param header Ponteiro para a estrutura do cabeçalho.
 * @param headerSize Tamanho, em bytes, da estrutura do cabeçalho.
 * 
 * 
 * @return Nenhum.
 */
void saveHeader(FILE *file, void *header, size_t headerSize)
{
    // Coloca o ponteiro do arquivo no início
    fseek(file, 0, SEEK_SET);

    // Escreve o cabeçalho no arquivo
    fwrite(header, headerSize, 1, file);
}