#include "file_manager.h"
#include "book_data_file.h"
#include "two_three_tree.h"

#include <errno.h>

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
 * @brief Cria um novo nó no arquivo de índices.
 * 
 * Esta função é responsável por criar um novo nó no arquivo de índices.
 * 
 * @param indexFile Ponteiro para o arquivo de índices.
 * @param node Ponteiro para o nó a ser criado.
 * 
 * @return Posição do nó no arquivo de índices.
 * 
 * @note O nó é criado no final do arquivo de índices.
 */
Node23 loadNode23(FILE *indexFile, int offset){
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
void saveNode(FILE *indexFile, int offset, Node23 *node) {
    fseek(indexFile, offset, SEEK_SET);
    fwrite(node, sizeof(Node23), 1, indexFile);
}