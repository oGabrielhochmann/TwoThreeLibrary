/**
 * @file book_manager.c

 * @brief Manages book-related logic in a library or book management system.
 *
 * This file contains functions responsible for handling various operations related to books.
 * These operations include searching for books by author, calculating the total number of books,
 * adding and removing books from the system, and possibly other book management tasks.
 *
 * The functions in this file interact with a data structure holding book information and
 * perform tasks necessary for maintaining and updating the collection of books.
 *
 * @author Gabriel Hochmann
 *
 * @see book_manager.h
 */

#include "book_data_file.h"
#include "two_three_tree.h"

#include "book_manager.h"
#include "tree_manager.h"
#include "utils.h"

#include <stdio.h>

/**
 * @brief Processa uma linha de texto e extrai os dados do livro.
 *
 * @param[in] linha Linha do arquivo de texto contendo os dados do livro.
 * @param[out] livro Ponteiro para a estrutura `Livro` onde os dados extraídos serão armazenados.
 *
 * @pre A linha de texto deve estar formatada corretamente, com os campos separados por ponto e vírgula (`;`).
 *      O ponteiro para a estrutura `Livro` deve ser válido e alocado corretamente.
 *
 * @post A estrutura `Livro` apontada por `livro` será preenchida com os dados extraídos da linha de texto.
 *       Os campos da estrutura serão atualizados de acordo com os valores encontrados na linha.
 *
 * @details A função espera que a linha de texto contenha os dados do livro na seguinte ordem:
 * - Código do livro (inteiro)
 * - Título do livro (string)
 * - Aauthor do livro (string)
 * - Editora do livro (string)
 * - Edição do livro (inteiro)
 * - Ano de publicação (inteiro)
 * - Preço (decimal)
 * - Estoque (inteiro)
 * - Posição (inteiro) - Novo campo adicionado
 * - Preço (decimal) - Novo campo adicionado
 *
 * @note A função assume que todos os campos estão presentes na linha e no formato correto. Se algum campo estiver ausente ou mal formatado,
 * o comportamento pode ser imprevisível. Certifique-se de que o arquivo de texto está corretamente formatado.
 */
static void extractBookFromLine(const char *linha, Book *livro)
{
    if (linha == NULL || livro == NULL)
        return;

    char buffer[1024];
    strncpy(buffer, linha, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0'; // Garante terminação segura

    char *token = strtok(buffer, ";");

    if (token)
        livro->code = atoi(token);

    token = strtok(NULL, ";");
    if (token)
    {
        trimWhitespace(token);
        snprintf(livro->title, sizeof(livro->title), "%s", token);
    }

    token = strtok(NULL, ";");
    if (token)
    {
        trimWhitespace(token);
        snprintf(livro->author, sizeof(livro->author), "%s", token);
    }

    token = strtok(NULL, ";");
    if (token)
    {
        trimWhitespace(token);
        snprintf(livro->publisher, sizeof(livro->publisher), "%s", token);
    }

    token = strtok(NULL, ";");
    if (token)
        livro->edition = atoi(token);

    token = strtok(NULL, ";");
    if (token)
        livro->year = atoi(token);

    token = strtok(NULL, ";");
    if (token)
    {
        formatDecimalSeparator(token);
        livro->price = atof(token);
    }

    token = strtok(NULL, ";");
    if (token)
        livro->stock_quantity = atoi(token);
}

/**
 * @brief Retorna o offset do livro no arquivo de dados com base no código fornecido.
 *
 * @param[in] indexFile Ponteiro para o arquivo de índices onde a árvore 2-3 está armazenada.
 * @param[in] key O código do livro a ser verificado.
 *
 * @return Retorna o offset do livro no arquivo de dados se encontrado, `-1` caso contrário.
 *
 * @details Esta função utiliza a busca na árvore 2-3 (função `twoThreeTreeSearch`) para procurar
 *          o código do livro no arquivo de índices. Caso o livro seja encontrado, retorna o offset
 *          correspondente no arquivo de dados. Caso contrário, retorna `-1`.
 */
static long getBookOffset(FILE *indexFile, int key)
{
    // Realiza a busca na árvore 2-3 para o código do livro
    long offset = twoThreeTreeSearch(indexFile, key);

    // Verifica se a busca retornou um offset válido
    if (offset != -1)
    {
        return offset; // Retorna o offset do livro encontrado
    }
    return -1; // Retorna -1 se o livro não for encontrado
}

void addBookAux(FILE *dataFile, FILE *indexFile, const Book *book)
{
    // Verifica se o livro já existe no índice
    if (getBookOffset(indexFile, book->code) != -1)
    {
        printf("Erro: Livro com o código %d já existe no índice.\n", book->code);
        return; // Livro já existe, não adiciona novamente
    }

    // Cabeçalho do arquivo de dados
    BookDataFileHeader dataHeader;

    // Lê o cabeçalho do arquivo de dados usando a função readFileHeader
    if (readFileHeader(dataFile, &dataHeader, sizeof(BookDataFileHeader)) != 1)
    {
        return; // Se falhar ao ler o cabeçalho, retorna
    }

    // Cabeçalho do arquivo de índice
    IndexFileHeader indexHeader;

    // Lê o cabeçalho do arquivo de índice usando a função readFileHeader
    if (readFileHeader(indexFile, &indexHeader, sizeof(IndexFileHeader)) != 1)
    {
        return; // Se falhar ao ler o cabeçalho, retorna
    }

    long offset; // A posição onde o livro será armazenado no arquivo de dados

    // Verifica se há uma posição livre para reutilizar no arquivo de dados
    if (dataHeader.headEmptyPosition != -1)
    {
        // Reutiliza a posição livre
        offset = dataHeader.headEmptyPosition;

        // Lê a próxima posição livre da lista encadeada de espaços livres
        BookDataFreeNode freeNode;
        if (fseek(dataFile, sizeof(BookDataFileHeader) + offset * sizeof(Book), SEEK_SET) != 0)
        {
            perror("Erro ao posicionar o ponteiro no arquivo de dados");
            return;
        }
        if (fread(&freeNode, sizeof(BookDataFreeNode), 1, dataFile) != 1)
        {
            perror("Erro ao ler o próximo nó livre");
            return;
        }

        // Atualiza a posição livre no cabeçalho
        dataHeader.headEmptyPosition = freeNode.nextOffset;

        // Atualiza o cabeçalho no arquivo de dados
        if (fseek(dataFile, 0, SEEK_SET) != 0)
        {
            perror("Erro ao posicionar o ponteiro no arquivo de dados");
            return;
        }

        if (fwrite(&dataHeader, sizeof(BookDataFileHeader), 1, dataFile) != 1)
        {
            perror("Erro ao atualizar o cabeçalho do arquivo de dados");
            return;
        }
    }
    else
    {
        // Caso não haja registros livres, adicione no final do arquivo de dados
        fseek(dataFile, 0, SEEK_END);
        offset = ftell(dataFile) / sizeof(Book); // Calcula a posição com base no tamanho do livro
    }

    // Adiciona o livro na posição calculada
    fseek(dataFile, sizeof(BookDataFileHeader) + offset * sizeof(Book), SEEK_SET);
    if (fwrite(book, sizeof(Book), 1, dataFile) != 1)
    {
        perror("Erro ao adicionar o livro no arquivo de dados");
        return;
    }

    // Atualiza o índice com o código do livro e o novo offset
    insertKey(indexFile, book->code, offset, &indexHeader);

    // Atualiza o cabeçalho do arquivo de índice com a posição do topo
    if (offset >= indexHeader.rootAddress)
    {
        indexHeader.rootAddress = offset + 1;
    }

    // Atualiza o cabeçalho no arquivo de índice
    fseek(indexFile, 0, SEEK_SET);
    if (fwrite(&indexHeader, sizeof(IndexFileHeader), 1, indexFile) != 1)
    {
        perror("Erro ao atualizar o cabeçalho do arquivo de índice");
        return;
    }

    printf("Livro adicionado com sucesso!\n");
}

/**
 * @brief Coleta dados de um livro do usuário e os adiciona ao arquivo.
 *
 * @details Esta função solicita ao usuário que insira os detalhes de um livro, incluindo código, título, autor, editora, edição, ano, preço e estoque.
 *          A função valida os dados inseridos pelo usuário, ajusta o formato do preço, e adiciona o livro ao arquivo especificado.
 *          Caso o usuário insira um preço com vírgula, a função ajusta o formato para usar ponto como separador decimal.
 *          Se o preço inserido for inválido ou qualquer outro dado for inserido de forma incorreta, a função solicita a reentrada dos dados.
 *
 * @param fp Ponteiro para o arquivo onde o livro será adicionado. O arquivo deve estar aberto no modo de escrita.
 * @param cabecalho Ponteiro para o cabeçalho do arquivo. Deve ser um ponteiro válido para um objeto `Cabecalho` que será atualizado conforme necessário.
 *
 * @pre O ponteiro `fp` deve ser um ponteiro válido para um arquivo aberto no modo de escrita.
 * @pre O ponteiro `cabecalho` deve ser um ponteiro válido para um objeto `Cabecalho` que esteja corretamente inicializado e acessível.
 *
 * @post Os dados do livro inseridos pelo usuário são lidos e processados. Se o preço foi inserido com uma vírgula, ele será convertido para o formato com ponto.
 *       O livro é adicionado ao arquivo através da função `adicionarLivro`. Uma mensagem de sucesso é exibida ao final do processo.
 *
 * @note A função usa `fgets` para ler strings e garantir que não ocorram estouros de buffer. Além disso, é feita a limpeza do buffer de entrada após cada leitura
 *       com `scanf` para evitar problemas com entradas residuais.
 */
void registerBook(FILE *dataFile, FILE *indexFile)
{
    Book livro;
    char precoStr[20];
    int entradaValida;

    // Coletar dados do livro
    printf("Digite os dados do livro:\n");

    // Código
    do
    {
        printf("Codigo: ");
        entradaValida = scanf("%d", &livro.code);
        while (getchar() != '\n')
            ; // Limpar buffer de entrada
    } while (entradaValida != 1);

    // Título
    printf("Titulo: ");
    if (fgets(livro.title, sizeof(livro.title), stdin) == NULL)
    {
        printf("Erro ao ler o título.\n");
        return;
    }
    livro.title[strcspn(livro.title, "\n")] = '\0'; // Remover newline

    // Autor
    printf("Autor: ");
    if (fgets(livro.author, sizeof(livro.author), stdin) == NULL)
    {
        printf("Erro ao ler o autor.\n");
        return;
    }
    livro.author[strcspn(livro.author, "\n")] = '\0'; // Remover newline

    // Editora
    printf("Editora: ");
    if (fgets(livro.publisher, sizeof(livro.publisher), stdin) == NULL)
    {
        printf("Erro ao ler a editora.\n");
        return;
    }
    livro.publisher[strcspn(livro.publisher, "\n")] = '\0'; // Remover newline

    // Edição
    do
    {
        printf("Edicao: ");
        entradaValida = scanf("%d", &livro.edition);
        while (getchar() != '\n')
            ; // Limpar buffer de entrada
    } while (entradaValida != 1);

    // Ano
    do
    {
        printf("Ano: ");
        entradaValida = scanf("%d", &livro.year);
        while (getchar() != '\n')
            ; // Limpar buffer de entrada
    } while (entradaValida != 1);

    // Preço
    do
    {
        printf("Preco: ");
        if (fgets(precoStr, sizeof(precoStr), stdin) == NULL)
        {
            printf("Erro ao ler o preço.\n");
            return;
        }
        formatDecimalSeparator(precoStr); // Ajusta o formato do preço
        livro.price = strtof(precoStr, NULL);

        // Verifica se a conversão foi bem-sucedida e se o valor é positivo
        if (livro.price <= 0)
        {
            printf("Valor de preco invalido. Tente novamente.\n");
            entradaValida = 0;
        }
        else
        {
            entradaValida = 1;
        }
    } while (entradaValida != 1);

    // Estoque
    do
    {
        printf("Estoque: ");
        entradaValida = scanf("%d", &livro.stock_quantity);
        while (getchar() != '\n')
            ; // Limpar buffer de entrada
    } while (entradaValida != 1);

    // Adiciona o livro ao arquivo
    addBook(dataFile, indexFile, &livro);

    printf("Livro adicionado com sucesso.\n");
}

/**
 * @brief Busca e exibe livros de um autor específico em um arquivo.
 *
 * @details Esta função percorre um arquivo de livros e imprime os títulos dos livros escritos por um autor específico. A busca é feita de forma insensível a maiúsculas e minúsculas. Se nenhum livro do autor for encontrado, uma mensagem é exibida.
 *
 * @param fp Ponteiro para o arquivo onde os livros estão armazenados. O arquivo deve estar aberto no modo de leitura.
 * @param autor Nome do autor pelo qual os livros devem ser buscados. A comparação é feita de forma insensível a maiúsculas e minúsculas.
 *
 * @pre O ponteiro `fp` deve ser um ponteiro válido para um arquivo aberto no modo de leitura.
 * @pre O ponteiro `autor` deve ser uma string válida que representa o nome do autor a ser buscado.
 *
 * @post Todos os livros do autor especificado são exibidos, ou uma mensagem é mostrada se nenhum livro for encontrado.
 */
void searchByAuthor(FILE *dataFile, const char *autor)
{
    Book livro;
    long posicao = sizeof(BookDataFileHeader); // A posição inicial do primeiro livro é após o cabeçalho
    int encontrado = 0;

    if (!dataFile || !autor)
    {
        fprintf(stderr, "Erro: ponteiro de arquivo ou autor invalido.\n");
        return;
    }

    // Percorre todos os livros no arquivo
    while (fseek(dataFile, posicao, SEEK_SET) == 0 && fread(&livro, sizeof(Book), 1, dataFile) == 1)
    {
        if (comparaCaseInsensitive(livro.author, autor) == 0)
        {
            // Livro encontrado
            printf("Titulo: %s\n", livro.title);
            encontrado = 1;
        }
        posicao += sizeof(Book); // Move para a próxima posição do livro
    }

    if (!encontrado)
    {
        printf("Nenhum livro encontrado para o autor \"%s\".\n", autor);
    }
}

void showBookInfo(FILE *dataFile, int offset)
{
    if (!dataFile)
    {
        printf("Erro: arquivo dataFile não está aberto.\n");
        return;
    }

    // Calcula a posição do livro no arquivo
    long position = sizeof(BookDataFileHeader) + offset * sizeof(Book);

    // Navega até a posição do livro
    if (fseek(dataFile, position, SEEK_SET) != 0)
    {
        printf("Erro: falha ao buscar a posição do livro no arquivo.\n");
        return;
    }

    // Lê os dados do livro
    Book livro;
    if (fread(&livro, sizeof(Book), 1, dataFile) != 1)
    {
        printf("Erro: falha ao ler os dados do livro.\n");
        return;
    }

    // Imprime os dados do livro
    printf("\n=== Dados do Livro ===\n");
    printf("Código: %d\n", livro.code);
    printf("Título: %s\n", livro.title);
    printf("Autor: %s\n", livro.author);
    printf("Editora: %s\n", livro.publisher);
    printf("Edição: %d\n", livro.edition);
    printf("Ano: %d\n", livro.year);
    printf("Preço: %.2f\n", livro.price);
    printf("Estoque: %d\n", livro.stock_quantity);
}

/**
 * @brief Busca e imprime informações de um livro com um título específico.
 *
 * @details Esta função procura por um livro com o título fornecido no arquivo de dados.
 * A busca é realizada de forma insensível a maiúsculas e minúsculas. Se um livro com o título
 * correspondente for encontrado, suas informações são exibidas. Caso contrário, uma mensagem
 * informando que nenhum livro foi encontrado é exibida.
 *
 * @pre O arquivo deve estar aberto e posicionado corretamente. O ponteiro `fp` deve apontar para um arquivo
 *       binário que contém registros de livros, e o título deve ser uma string não-nula.
 *
 * @post O conteúdo do arquivo não é modificado. Se um livro com o título especificado for encontrado,
 *       suas informações são exibidas na saída padrão. Caso contrário, uma mensagem indicando que o livro
 *       não foi encontrado é exibida.
 *
 * @param fp Ponteiro para o arquivo onde os livros estão armazenados.
 * @param titulo O título do livro a ser pesquisado. Deve ser uma string não-nula.
 */
void searchByTitle(FILE *filename, const char *titulo)
{
    Book livro;
    long posicao = sizeof(BookDataFileHeader); // Posição inicial após o cabeçalho
    int encontrado = 0;

    // Verifica se o ponteiro do arquivo é válido
    if (!filename || !titulo)
    {
        fprintf(stderr, "Arquivo invalido ou titulo nulo.\n");
        return;
    }

    // Percorre todos os livros no arquivo
    while (fseek(filename, posicao, SEEK_SET) == 0 && fread(&livro, sizeof(Book), 1, filename) == 1)
    {
        // Compara o título do livro com o título fornecido
        if (comparaCaseInsensitive(livro.title, titulo) == 0)
        {
            // Livro encontrado, imprime as informações
            showBookInfo(filename, posicao);
            encontrado = 1;
            break; // Se você quiser parar após encontrar o primeiro livro com o título, use break
        }
        posicao += sizeof(Book); // Move para a próxima posição do livro
    }

    if (!encontrado)
    {
        printf("Nenhum livro encontrado com o titulo \"%s\".\n", titulo);
    }
}

/**
 * @brief Imprime os dados de um livro com base em seu código.
 *
 * @details Esta função utiliza o código do livro fornecido para procurar a posição (offset) do livro no arquivo de dados.
 *          Se o livro for encontrado, seus dados são impressos no console. Caso contrário, uma mensagem indicando que o livro não foi encontrado é exibida.
 *
 * @param arquivo Ponteiro para o arquivo onde os livros estão armazenados. O arquivo deve estar aberto no modo de leitura.
 * @param codigo O código do livro a ser pesquisado. Deve ser um valor inteiro que identifique unicamente o livro.
 *
 * @pre O arquivo deve estar aberto e acessível. O ponteiro de arquivo deve ser válido e o arquivo deve estar posicionado no início dos dados dos livros.
 * @pre O código do livro deve ser um valor válido para a pesquisa.
 *
 * @post Se o livro com o código especificado for encontrado, seus dados serão impressos no console.
 * @post Se o livro não for encontrado, uma mensagem informando que o livro não foi encontrado será exibida.
 * @post O ponteiro do arquivo será reposicionado para o início dos dados dos livros após a execução da função.
 *
 * @return Nenhum valor é retornado. A função imprime diretamente os dados no console ou uma mensagem de erro.
 */
void printBookData(FILE *arquivo, int codigo)
{
    // Verifica se o arquivo está aberto
    if (!arquivo)
    {
        printf("Erro: arquivo nao esta aberto.\n");
        return;
    }

    // Obtém o offset do livro no arquivo de dados
    int offset = getBookOffset(arquivo, codigo);

    // Verifica se o livro foi encontrado (offset válido)
    if (offset == -1)
    {
        printf("Livro com código %d não encontrado.\n", codigo);
        return;
    }

    // Imprime os dados do livro encontrado
    showBookInfo(arquivo, offset);

    // Retorna ao início do arquivo
    fseek(arquivo, sizeof(BookDataFileHeader), SEEK_SET);
}

/**
 * @brief Lista todos os livros armazenados no arquivo.
 *
 * @details Esta função percorre todos os registros de livros no arquivo, a partir da posição logo após o cabeçalho, e imprime os detalhes de cada livro que não está marcado como removido.
 *          A função formata a saída em uma tabela para facilitar a visualização.
 *
 * @param arquivo Ponteiro para o arquivo onde os livros estão armazenados. O arquivo deve estar aberto no modo de leitura.
 *
 * @pre O arquivo deve estar aberto e acessível. O ponteiro de arquivo deve ser válido e o arquivo deve estar posicionado no início dos dados dos livros.
 *
 * @post A lista de livros é impressa no console em formato tabular.
 * @post Livros com código marcado como -1 (removidos) não são exibidos.
 * @post O ponteiro do arquivo será reposicionado para o início dos dados dos livros após a execução da função.
 */
void listarTodosLivros(FILE *arquivo)
{
    // Verifica se o arquivo está aberto
    if (!arquivo)
    {
        printf("Erro: arquivo não está aberto.\n");
        return;
    }

    Book livro;

    // Move o ponteiro do arquivo para o início da lista de livros
    if (fseek(arquivo, sizeof(BookDataFileHeader), SEEK_SET) != 0)
    {
        printf("Erro: falha ao mover ponteiro para o início dos dados dos livros.\n");
        return;
    }

    // Imprime o cabeçalho da tabela
    printf("--------------------------------------------------------------------------------------------\n");
    printf("| Codigo | Titulo                              | Autor                          | Estoque |\n");
    printf("--------------------------------------------------------------------------------------------\n");

    // Percorre todos os registros de livros
    int count = 0; // Contador de livros exibidos
    while (fread(&livro, sizeof(Book), 1, arquivo) == 1)
    {
        // Verifica se o livro está marcado como removido
        if (livro.code != -1)
        {
            // Imprime os detalhes do livro em formato tabular
            printf("| %-6d | %-35.35s | %-30.30s | %-7d |\n",
                   livro.code, livro.title, livro.author, livro.stock_quantity);
            count++;
        }
    }

    // Se nenhum livro válido for encontrado
    if (count == 0)
    {
        printf("| Nenhum livro encontrado.\n");
    }

    // Imprime o rodapé da tabela
    printf("--------------------------------------------------------------------------------------------\n");

    // Retorna ao início do arquivo
    if (fseek(arquivo, sizeof(BookDataFileHeader), SEEK_SET) != 0)
    {
        printf("Erro: falha ao reposicionar ponteiro ao início dos dados.\n");
    }
}

/**
 * @brief Calcula o total de livros registrados no arquivo de índices utilizando a árvore 2-3.
 *
 * Esta função é responsável por calcular o número total de livros registrados na árvore 2-3 armazenada no arquivo de índices.
 * A árvore 2-3 é usada para indexar os registros de dados, onde cada chave na árvore pode representar um livro registrado.
 * O total de livros registrados é determinado contando o número de nós na árvore 2-3, pois cada nó contém informações relacionadas a um livro.
 *
 * A função utiliza a função auxiliar `twoThreeTreeCountNodes`, que percorre toda a árvore 2-3 e conta o número total de nós.
 * Como cada nó da árvore 2-3 representa um livro, o número de nós corresponde ao número de livros registrados no sistema.
 *
 * @param indexFile Ponteiro para o arquivo de índices. O arquivo contém a árvore 2-3 que armazena as informações de índice.
 *                  A árvore 2-3 é percorrida para calcular o número total de livros registrados.
 *
 * @return O total de livros registrados. Retorna um valor inteiro correspondente ao número de livros registrados na árvore 2-3.
 *         Caso o arquivo não esteja aberto ou um erro seja detectado, a função retorna -1.
 *
 * @note A função espera que o arquivo de índices esteja corretamente aberto e que a árvore 2-3 tenha sido configurada
 *       de acordo com o modelo esperado. Caso contrário, a função irá retornar um valor de erro.
 */
int calcularTotalLivrosRegistrados(FILE *indexFile)
{
    // Verifica se o arquivo foi aberto corretamente
    if (!indexFile)
    {
        // Caso o arquivo não esteja aberto, imprime mensagem de erro e retorna -1
        printf("Erro: arquivo de indices nao esta aberto.\n");
        return -1;
    }

    // Variável que armazena o total de livros
    int totalLivros = 0;

    // A árvore 2-3 deve ser percorrida e cada chave representa um livro.
    // A função twoThreeTreeCountNodes conta o número de nós na árvore,
    // e cada nó da árvore representa um livro registrado.
    totalLivros = twoThreeTreeCountNodes(indexFile);

    // Retorna o total de livros registrados
    return totalLivros;
}

/**
 * @brief Soma a quantidade total de livros em estoque no arquivo de dados, ignorando registros deletados.
 *
 * Esta função percorre o arquivo de dados, começando após o cabeçalho, e soma as quantidades de estoque
 * dos livros cujos códigos não são -1 (indicando que o livro não foi deletado). Para cada livro válido,
 * a quantidade de estoque é somada.
 *
 * @param dataFile Ponteiro para o arquivo de dados, que contém os registros dos livros.
 * @param headerSize Tamanho do cabeçalho no arquivo de dados (em bytes), que será pulado.
 *
 * @return A soma das quantidades de livros em estoque. Se ocorrer algum erro, a função retorna -1.
 */
int calcularTotalLivrosEmEstoque(FILE *dataFile, size_t headerSize)
{
    // Verifica se o arquivo foi aberto corretamente
    if (!dataFile)
    {
        printf("Erro: arquivo de dados nao esta aberto.\n");
        return -1;
    }

    int totalEstoque = 0;
    Book book;

    // Pula o cabeçalho do arquivo de dados
    fseek(dataFile, headerSize, SEEK_SET);

    // Lê o arquivo de dados, um livro por vez, e soma o estoque
    while (fread(&book, sizeof(Book), 1, dataFile))
    {
        // Se o código do livro não for -1 (livro não deletado), soma o estoque
        if (book.code != -1)
        {
            totalEstoque += book.stock_quantity;
        }
    }

    // Retorna o total de livros em estoque
    return totalEstoque;
}
