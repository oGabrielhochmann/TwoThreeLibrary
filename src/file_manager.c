/**
 * @file file_manager.c
 * @author Gabriel Hochmann
 *
 * @brief Contém funções para o gerenciamento de arquivos binários, incluindo leitura,
 *        escrita e manipulação de dados relacionados a livros e árvores 2-3.
 *
 * Este arquivo implementa funções para realizar operações de gerenciamento de arquivos binários.
 * As funções são responsáveis por ler e escrever dados em arquivos binários que contêm informações
 * sobre livros, além de interagir com uma árvore 2-3 para o armazenamento e recuperação eficiente
 * de dados.
 *
 * @see file_manager.h
 * @see book_data_file.h
 * @see two_three_tree.h
 */

#include "book_data_file.h"
#include "two_three_tree.h"
#include "book.h"
#include "file_manager.h"
#include "book_manager.h"
#include "utils.h"

#include <errno.h>

/**
 * @brief Verifica se o arquivo está aberto corretamente.
 *
 * @param[in] arquivo Ponteiro para o arquivo a ser verificado.
 * @param[in] nomeArquivo Nome do arquivo (usado para a mensagem de erro).
 *
 * @return Retorna `1` se o arquivo estiver aberto corretamente; caso contrário, retorna `0`.
 *
 * @pre O ponteiro `arquivo` deve ser válido e ter sido previamente aberto com uma função como `fopen()`.
 *      Caso o ponteiro seja `NULL`, o arquivo não foi aberto corretamente e a função retornará `0`.
 *
 * @post Se o ponteiro `arquivo` for válido, a função retorna `1`, indicando que o arquivo foi aberto corretamente.
 *       Caso contrário, retorna `0` e imprime uma mensagem de erro para `stderr`, indicando qual arquivo não foi aberto corretamente.
 *
 * @details Esta função é útil para validar se qualquer tipo de arquivo foi aberto corretamente antes de realizar operações de leitura ou escrita.
 *          A validação é essencial para evitar falhas de execução ao tentar acessar um arquivo inexistente ou mal aberto.
 */
static int verifyFile(FILE *arquivo, const char *nomeArquivo)
{
    if (arquivo == NULL)
    {
        fprintf(stderr, "Erro: O arquivo '%s' não está aberto corretamente!\n", nomeArquivo);
        return 0;
    }
    return 1;
}

/**
 * @brief Abre um arquivo no modo especificado.
 *
 * Esta função é responsável por abrir um arquivo no modo especificado e verificar se a operação foi bem-sucedida.
 * Caso ocorra um erro ao abrir o arquivo, uma mensagem de erro será exibida.
 *
 * @pre `filename` não deve ser NULL.
 * @pre `mode` não deve ser NULL.
 *
 * @post O arquivo é aberto no modo especificado e um ponteiro para o arquivo é retornado,
 *       ou NULL se ocorrer um erro.
 *
 * @param filename Nome do arquivo a ser aberto.
 * @param mode Modo de abertura do arquivo (ex: "r", "w", "rb", "wb", etc.).
 *
 * @return Ponteiro para o arquivo aberto, ou NULL se ocorrer um erro. Em caso de erro,
 *         uma mensagem será exibida no stderr com detalhes sobre o erro.
 */
FILE *openFile(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);

    if (!validarArquivo(filename, "Arquivo"))
    {
        return NULL;
    }

    return file;
}

/**
 * @brief Fecha um arquivo previamente aberto e libera o ponteiro.
 *
 * Esta função é responsável por fechar um arquivo previamente aberto e definir o ponteiro do arquivo como NULL.
 * Ela verifica se o fechamento foi bem-sucedido e, caso contrário, exibe uma mensagem de erro.
 *
 * @pre `file` não deve ser NULL e o ponteiro `*file` não deve ser NULL.
 *
 * @post O arquivo é fechado e o ponteiro para o arquivo é definido como NULL.
 *
 * @param file Ponteiro duplo para o arquivo a ser fechado.
 *
 * @return 0 se o arquivo foi fechado com sucesso, -1 caso contrário.
 *
 * @note Se ocorrer um erro ao fechar o arquivo, uma mensagem de erro será exibida com detalhes do erro.
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
        perror("Erro ao fechar o arquivo");
    }

    return -1;
}

/**
 * @brief Cria o cabeçalho do arquivo de índices.
 *
 * Esta função é responsável por inicializar e criar o cabeçalho de um arquivo de índices. O cabeçalho contém informações essenciais
 * para a organização do arquivo, como a raiz da árvore de índices, a primeira posição livre disponível e a referência para a cabeça dos registros livres.
 * Após criar o cabeçalho, ele é escrito no início do arquivo.
 *
 * @param file Ponteiro para o arquivo de índices a ser modificado. O arquivo deve estar aberto em modo de escrita.
 *
 * @return Nenhum.
 *
 * @note O cabeçalho do arquivo de índices é composto por três informações:
 *       - A raiz da árvore (`rootAddress`), inicialmente definida como -1 (indicando que a árvore está vazia).
 *       - A primeira posição livre (`firstEmptyPosition`), inicialmente definida como 0.
 *       - A cabeça de registros livres (`headEmptyPosition`), inicialmente definida como -1 (sem registros livres).
 *
 * @warning A função assume que o arquivo foi aberto corretamente e está pronto para gravação. Não realiza verificação de erros na abertura do arquivo.
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
 * Esta função é responsável por inicializar e criar o cabeçalho de um arquivo de dados. O cabeçalho contém informações essenciais
 * para o gerenciamento dos dados no arquivo, como a primeira posição livre e a referência para a cabeça dos registros livres.
 * Após a criação do cabeçalho, ele é escrito no início do arquivo.
 *
 * @param file Ponteiro para o arquivo de dados a ser modificado. O arquivo deve estar aberto em modo de escrita.
 *
 * @return Nenhum.
 *
 * @note O cabeçalho do arquivo de dados é composto por duas informações:
 *       - A primeira posição livre (`firstEmptyPosition`), inicialmente definida como 0.
 *       - A cabeça de registros livres (`headEmptyPosition`), inicialmente definida como -1 (sem registros livres).
 *
 * @warning A função assume que o arquivo foi aberto corretamente e está pronto para gravação. Não realiza verificação de erros na abertura do arquivo.
 */
void createBookDataFileHeader(FILE *file)
{
    BookDataFileHeader header;

    header.firstEmptyPosition = 0; // Primeira posição livre
    header.headEmptyPosition = -1; // Cabeça de registros livres (inicialmente sem registros livres)

    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(header), 1, file);
}

/**
 * @brief Lê o cabeçalho de um arquivo binário e armazena na estrutura fornecida.
 *
 * Esta função é responsável por ler o cabeçalho de um arquivo binário e copiá-lo para a estrutura fornecida pelo usuário.
 * A leitura ocorre a partir do início do arquivo e armazena os dados do cabeçalho na estrutura de memória fornecida.
 *
 * @param file Ponteiro para o arquivo binário já aberto. O arquivo deve estar aberto em modo de leitura binária.
 * @param header Ponteiro para a estrutura onde o cabeçalho será armazenado. A estrutura deve ser alocada antes da chamada da função.
 * @param headerSize Tamanho, em bytes, da estrutura do cabeçalho, fornecido para garantir a leitura correta do cabeçalho.
 *
 * @return 1 se o cabeçalho foi lido com sucesso, -1 em caso de erro.
 *
 * @note A função assume que o arquivo foi aberto corretamente em modo de leitura binária antes de ser chamada.
 *       A leitura será feita a partir do início do arquivo.
 *
 * @warning Se ocorrer um erro ao ler o cabeçalho, uma mensagem de erro será exibida.
 */
int readFileHeader(FILE *file, void *header, size_t headerSize)
{
    fseek(file, 0, SEEK_SET);

    if (fread(header, headerSize, 1, file) != 1)
    {
        fprintf(stderr, "Erro ao ler o cabeçalho do arquivo.\n");
        return -1; // Erro ao ler o cabeçalho
    }

    return 1; // Sucesso
}

/**
 * @brief Salva o cabeçalho de um arquivo binário.
 *
 * Esta função é responsável por escrever um cabeçalho em um arquivo binário. Ela coloca o ponteiro do arquivo no início e
 * grava o cabeçalho fornecido na posição inicial do arquivo.
 *
 * @param file Ponteiro para o arquivo binário já aberto. O arquivo deve estar aberto em modo de escrita binária.
 * @param header Ponteiro para a estrutura que contém o cabeçalho a ser salvo no arquivo.
 * @param headerSize Tamanho, em bytes, da estrutura do cabeçalho. Esse valor é usado para garantir que a quantidade
 *                   correta de dados seja escrita no arquivo.
 *
 * @return Nenhum.
 *
 * @note A função assume que o arquivo foi aberto corretamente em modo de escrita binária antes de ser chamada.
 *       O cabeçalho será escrito a partir do início do arquivo.
 *
 * @warning Não há verificação de erros durante a escrita do cabeçalho. É importante garantir que o arquivo esteja
 *          corretamente aberto e pronto para gravação antes de chamar a função.
 */
void saveHeader(FILE *file, void *header, size_t headerSize)
{
    // Coloca o ponteiro do arquivo no início
    fseek(file, 0, SEEK_SET);

    // Escreve o cabeçalho no arquivo
    fwrite(header, headerSize, 1, file);
}

void loadTextFile(const char *textFilename, const char *indexFilename, const char *dataFilename)
{
    FILE *textFile = openFile(textFilename, "r");
    FILE *indexFile = openFile(indexFilename, "wb");
    FILE *dataFile = openFile(dataFilename, "wb");

    if (!verifyFile(textFile, "Arquivo texto") || !verifyFile(indexFile, "Arquivo de Indices") || !verifyFile(dataFile, "Arquivo de Dados"))
    {
        closeFile(&textFile);
        closeFile(&indexFile);
        closeFile(&dataFile);
        return;
    }

    Book book;
    char line[1024];

    while (fgets(line, sizeof(line), textFile))
    {
        // Remove a nova linha do final da string, se existir
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) > sizeof(line) - 1)
        {
            fprintf(stderr, "Erro: Linha muito longa no arquivo de texto.\n");
            break;
        }

        // Processa a linha e extrai dados do livro
        extractBookFromLine(line, &book);

        // Adiciona o livro ao arquivo binário
        addBook(dataFile, indexFile, &book);
    }

    closeFile(&textFile);
    closeFile(&indexFile);
    closeFile(&dataFile);

    printf("Arquivo carregado com sucesso!\n");
}
