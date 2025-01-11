/**
 * @file tree_manager.c
 *
 * @brief Contém funções para gerenciamento da árvore 2-3.
 *
 * Este arquivo implementa funções para o gerenciamento da árvore 2-3, incluindo operações como
 * busca, inserção e remoção de chaves. Essas funções são essenciais para o funcionamento da estrutura
 * de dados, permitindo que os dados sejam organizados e manipulados de maneira eficiente de acordo com
 * as regras da árvore 2-3.
 *
 * @see tree_manager.h
 */

#include "tree_manager.h"
#include "two_three_tree.h"
#include "file_manager.h"

#include <stdio.h>

/**
 * @brief Salva um nó no arquivo de índices.
 *
 * Esta função é responsável por salvar um nó da árvore 2-3 no arquivo de índices. O nó é armazenado
 * na posição especificada pelo deslocamento fornecido. A operação é realizada utilizando a função
 * `fseek` para posicionar o ponteiro do arquivo e `fwrite` para gravar o conteúdo do nó.
 *
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita. O nó a ser salvo deve
 *     ser válido e conter informações corretas.
 *
 * @post O nó é salvo no arquivo de índices na posição especificada pelo deslocamento.
 *
 * @param indexFile Ponteiro para o arquivo de índices onde o nó será salvo.
 * @param offset Deslocamento (em bytes) no arquivo de índices, indicando onde o nó deve ser gravado.
 * @param node Ponteiro para o nó a ser salvo, que será gravado no arquivo de índices.
 *
 * @return Nenhum. A função não retorna nenhum valor.
 */
static void saveNode(FILE *indexFile, int offset, Node23 *node)
{
    fseek(indexFile, offset, SEEK_SET);
    fwrite(node, sizeof(Node23), 1, indexFile);
}

/**
 * @brief Cria um nó 2-3 no arquivo de índices.
 *
 * Esta função é responsável por criar um nó 2-3 no arquivo de índices. Um nó 2-3 é uma estrutura usada
 * em árvores 2-3, que é um tipo de árvore balanceada. Ela armazena duas chaves e três filhos, sendo
 * utilizada para manter a ordem e balanceamento da árvore.
 *
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita. O cabeçalho do arquivo
 *      de índices deve estar carregado e contido no parâmetro `header`.
 *
 * @post Um novo nó 2-3 é criado no arquivo de índices. Se houver nós livres disponíveis, um nó
 *       existente será reutilizado. Caso contrário, um novo nó será criado no final do arquivo.
 *
 * @param indexFile Ponteiro para o arquivo de índices, que estará sendo manipulado para salvar o nó.
 * @param leftKey Chave da esquerda, representando o valor da chave da posição mais à esquerda do nó.
 * @param rightKey Chave da direita, representando o valor da chave da posição mais à direita do nó.
 * @param leftBook Posição do livro (dado) associado à chave da esquerda.
 * @param rightBook Posição do livro (dado) associado à chave da direita.
 * @param leftChild Filho da esquerda, que aponta para a subárvore à esquerda do nó.
 * @param middleChild Filho do meio, que aponta para a subárvore entre as chaves.
 * @param rightChild Filho da direita, que aponta para a subárvore à direita do nó.
 * @param nKeys Número de chaves no nó. Deve ser 2 para um nó 2-3 válido.
 * @param header Ponteiro para o cabeçalho do arquivo de índices, contendo informações como a posição do
 *               primeiro nó livre.
 *
 * @return Inteiro representando o deslocamento (offset) do nó no arquivo de índices. Retorna -1 se houver
 *         algum erro durante o processo de criação ou gravação do nó.
 *
 * @note A função pode reutilizar um nó previamente apagado (se houver um nó livre), atualizando a posição
 *       do nó livre e o deslocamento do nó a ser reutilizado.
 */
int createNode23(FILE *indexFile, int leftKey, int rightKey, int leftBook, int rightBook, int leftChild, int middleChild, int rightChild, int nKeys, IndexFileHeader *header)
{
    Node23 node;
    node.nKeys = nKeys;
    node.left_key = leftKey;
    node.right_key = rightKey;
    node.leftBook = leftBook;
    node.rightBook = rightBook;
    node.left_child = leftChild;
    node.middle_child = middleChild;
    node.right_child = rightChild;

    int nodeOffset;

    // Reutiliza um nó livre, se houver
    if (header->headEmptyPosition != -1)
    {
        // Carrega o nó livre
        Node23 freeNode = loadNode23(indexFile, header->headEmptyPosition);

        // Atualiza a cabeça da lista de nós livres
        header->headEmptyPosition = freeNode.left_child;

        // Atualiza o deslocamento do nó a ser reutilizado
        nodeOffset = header->headEmptyPosition;
    }
    else
    {
        if (fseek(indexFile, 0, SEEK_END) != 0)
        {
            perror("Erro ao posicionar o cursor no final do arquivo de índices");
            return -1;
        }

        nodeOffset = ftell(indexFile);

        if (nodeOffset == -1)
        {
            perror("Erro ao obter o deslocamento do cursor no arquivo de índices");
            return -1;
        }
    }

    saveNode(indexFile, nodeOffset, &node);

    fseek(indexFile, 0, SEEK_SET);
    if (fwrite(header, sizeof(IndexFileHeader), 1, indexFile) != 1)
    {
        perror("Erro ao escrever o cabeçalho no arquivo de índices");
        return -1;
    }

    return nodeOffset;
}

/**
 * @brief Carrega um nó do arquivo de índices.
 *
 * Esta função é responsável por carregar um nó a partir de um arquivo de índices existente,
 * dado um deslocamento (offset) no arquivo.
 *
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita.
 *
 * @post O nó é carregado a partir do arquivo de índices.
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
 * @brief Lê o endereço da raiz da árvore 2-3.
 *
 * Esta função é responsável por ler o endereço da raiz da árvore 2-3 a partir do cabeçalho do arquivo de índices.
 * O endereço da raiz é armazenado no cabeçalho do arquivo de índices e é utilizado para identificar o nó raiz
 * da árvore 2-3, a partir do qual as operações de inserção, remoção e busca são realizadas.
 *
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita. O cabeçalho do arquivo deve estar
 *      corretamente estruturado e ser acessível para leitura.
 *
 * @post O endereço da raiz da árvore 2-3 é lido a partir do cabeçalho do arquivo de índices e retornado
 *       para ser utilizado nas operações com a árvore.
 *
 * @param file Ponteiro para o arquivo de índices, de onde o cabeçalho será lido.
 *
 * @return Endereço da raiz da árvore 2-3. A função retorna o endereço (deslocamento no arquivo) do nó raiz
 *         da árvore 2-3, que é utilizado como ponto de partida para manipulação da árvore.
 *
 * @note O cabeçalho do arquivo de índices contém três informações principais: o endereço da raiz da árvore,
 *       a primeira posição livre para reutilização de nós e a cabeça da lista de registros livres. A função
 *       lê apenas o endereço da raiz.
 */
int getRootAddress(FILE *file)
{
    IndexFileHeader header;
    readFileHeader(file, &header, sizeof(IndexFileHeader));

    return header.rootAddress;
}

/**
 * @brief Realiza a busca de um nó na árvore 2-3.
 *
 * Esta função é responsável por realizar a busca de um nó na árvore 2-3 a partir de uma chave fornecida.
 * A busca é realizada de forma recursiva, descendo pela árvore a partir da raiz até o nó desejado,
 * comparando a chave de busca com as chaves presentes nos nós. A função retorna o endereço do nó
 * correspondente à chave buscada.
 *
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita. A raiz da árvore 2-3 deve ser
 *      fornecida para iniciar a busca.
 *
 * @post O nó correspondente à chave fornecida é buscado na árvore 2-3.
 *
 * @param file Ponteiro para o arquivo de índices, onde os nós da árvore 2-3 estão armazenados.
 * @param root Endereço (deslocamento) da raiz da árvore 2-3, utilizado como ponto de partida para a busca.
 * @param key Chave a ser buscada no nó da árvore 2-3.
 *
 * @return Inteiro representando o endereço do nó na árvore 2-3, que é o endereço (deslocamento) no arquivo de índices.
 *         Se a chave não for encontrada, a função retornará o endereço do nó onde a busca foi realizada.
 *
 * @note A busca é realizada recursivamente, descendo pela árvore 2-3. A função verifica primeiro a chave
 *       à esquerda do nó e, caso necessário, desce para os filhos do nó conforme a chave a ser buscada.
 *       Se a árvore tiver dois filhos, a chave à direita do nó também será verificada.
 */
static int searchNode(FILE *file, int root, int key)
{
    Node23 node = loadNode23(file, root);

    if (node.left_key == key)
    {
        return root;
    }

    if (node.nKeys == 2 && node.right_key == key)
    {
        return root;
    }

    if (key < node.left_key)
    {
        return searchNode(file, node.left_child, key);
    }
    else if (node.nKeys == 1 || key < node.right_key)
    {
        return searchNode(file, node.middle_child, key);
    }
    else // key > node.right_key
    {
        return searchNode(file, node.right_child, key);
    }
}

/**
 * @brief Realiza a busca de uma chave na árvore 2-3.
 *
 * Esta função é responsável por realizar a busca de uma chave na árvore 2-3 armazenada em um arquivo de índices.
 * A busca é iniciada a partir da raiz da árvore, e a função chama a função auxiliar `searchNode` para procurar
 * a chave dentro da estrutura de árvore 2-3. Se a árvore estiver vazia (caso em que a raiz é -1), a função retorna
 * -1 indicando que não há nenhum nó para buscar.
 *
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita.
 *
 * @post A chave é buscada na árvore 2-3 a partir da raiz.
 *
 * @param file Ponteiro para o arquivo de índices, onde os nós da árvore 2-3 estão armazenados.
 * @param key Chave a ser buscada na árvore 2-3.
 *
 * @return Inteiro representando o endereço do nó na árvore 2-3 (arquivo de índices),
 *         ou -1 se a árvore estiver vazia.
 *
 * @note A função obtém o endereço da raiz da árvore 2-3 através da função `getRootAddress`
 *       e inicia a busca pela função `searchNode`. Se a árvore estiver vazia (não houver raiz),
 *       a função retorna -1.
 */
int twoThreeTreeSearch(FILE *file, int key)
{
    int root = getRootAddress(file);

    if (root == -1)
    {
        return -1; // Árvore vazia
    }

    return searchNode(file, root, key);
}

/**
 * @brief Adiciona uma chave em um nó que tem 1 chave.
 *
 * Esta função é responsável por adicionar uma chave em um nó da árvore 2-3 que já contém uma chave,
 * atualizando o nó para armazenar duas chaves. A chave é inserida de forma que as chaves no nó fiquem ordenadas.
 * A chave inserida é comparada com a chave existente no nó para determinar a posição correta.
 *
 * @pre O nó deve ter espaço para 1 chave, ou seja, a variável `nKeys` deve ser 1.
 *
 * @post A chave é adicionada ao nó, atualizando as chaves do nó e a posição do livro.
 *
 * @param node Ponteiro para o nó onde a chave será inserida.
 * @param key Chave a ser adicionada ao nó.
 * @param bookPosition Posição do livro (dado) a ser inserido junto com a chave.
 *
 * @return Nenhum.
 *
 * @note A função compara a chave a ser inserida com a chave existente no nó e coloca a chave na posição
 *       correta, deslocando a chave existente para a posição da chave à direita, se necessário.
 */
static void addKeyToNodeWithOneKey(Node23 *node, int key, int bookPosition)
{
    if (node->left_key < key)
    {
        node->right_key = key;
        node->rightBook = bookPosition;
    }
    else
    {
        node->right_key = node->left_key;
        node->rightBook = node->leftBook;
        node->left_key = key;
        node->leftBook = bookPosition;
    }
    node->nKeys = 2;
}

/**
 * @brief Verifica se um nó é uma folha.
 *
 * Esta função verifica se um nó na árvore 2-3 é uma folha. Um nó é considerado folha
 * se não tiver filhos, ou seja, se os ponteiros para seus filhos forem iguais a -1.
 *
 * @pre Nenhuma.
 *
 * @post Nenhuma.
 *
 * @param node Ponteiro para o nó a ser verificado.
 *
 * @return 1 se o nó é uma folha, 0 caso contrário.
 *
 * @note A função assume que o nó contém informações sobre seus filhos, e considera
 *       que a ausência de filhos é indicada por valores de -1 nos campos `left_child`,
 *       `middle_child` e `right_child`.
 */
static int isLeafNode(Node23 *node)
{
    return node->left_child == -1;
}

/**
 * @brief Realiza a divisão de um nó na árvore 2-3.
 *
 * Esta função é responsável por dividir um nó na árvore 2-3 quando ele não pode acomodar
 * mais de 2 chaves, promovendo a chave do meio para o nó pai e criando novos nós filhos.
 *
 * @pre O nó pai e o nó a ser dividido devem estar corretamente carregados a partir do arquivo de índices.
 *      A chave a ser inserida já foi determinada.
 *
 * @post O nó é dividido e a chave do meio é promovida para o nó pai. O nó pai é atualizado com a nova chave.
 *       Um novo nó filho é criado para acomodar a chave inserida.
 *
 * @param indexFile Ponteiro para o arquivo de índices.
 * @param parentNodeOffset Deslocamento (offset) do nó pai no arquivo de índices.
 * @param parentNode Ponteiro para o nó pai.
 * @param key Chave a ser inserida no nó.
 * @param promotedKey Ponteiro para armazenar a chave promovida para o nó pai.
 * @param subarvoreOffset Deslocamento (offset) da subárvore a ser associada ao novo nó.
 * @param header Cabeçalho do arquivo de índices, utilizado para gerenciar informações do arquivo.
 * @param leftBookOffset Deslocamento (offset) do livro associado à chave à esquerda do nó.
 * @param rightBookOffset Deslocamento (offset) do livro associado à chave à direita do nó.
 *
 * @return O deslocamento (offset) do nó direito criado ou atualizado após a divisão.
 *
 * @note O nó a ser dividido pode ter 1 ou 2 chaves, dependendo da condição de inserção.
 *       O comportamento da divisão varia dependendo da comparação da chave a ser inserida
 *       com as chaves do nó pai. Em cada caso, um novo nó é criado e o nó pai é atualizado.
 */
static int splitNode(FILE *indexFile, int parentNodeOffset, Node23 *parentNode, int key, int *promotedKey, int subarvoreOffset, IndexFileHeader *header, int leftBookOffset, int rightBookOffset)
{
    int rightNodeOffset;

    // Se a chave a ser inserida é maior que a chave à direita
    if (key > parentNode->right_key)
    {
        // Promove a antiga maior chave
        *promotedKey = parentNode->right_key;

        // Cria um novo nó à direita
        rightNodeOffset = createNode23(indexFile, key, -1, rightBookOffset, -1, subarvoreOffset, -1, -1, 1, header);

        // Atualiza o nó pai
        parentNode->nKeys = 1;                     // O nó pai terá 1 chave
        parentNode->right_key = -1;                // A chave da direita será nula
        parentNode->rightBook = -1;                // A posição do livro associada à chave da direita será nula
        parentNode->right_child = rightNodeOffset; // O filho à direita será nulo
        parentNode->middle_child = -1;             // O filho do meio será nulo

        return rightNodeOffset;
    }

    if (key >= parentNode->left_key) // Se a chave a ser inserida é maior que a chave à esquerda e menor que a chave à direita
    {
        // Promove a chave do meio
        *promotedKey = key;

        // Cria um novo nó à direita
        rightNodeOffset = createNode23(indexFile, parentNode->right_key, -1, parentNode->rightBook, -1, -1, -1, -1, 1, header);

        // Atualiza o nó pai (muda a chave à direita para a chave inserida)
        parentNode->nKeys = 1;                      // O nó pai terá 1 chave
        parentNode->right_key = -1;                 // A chave da direita será nula
        parentNode->rightBook = -1;                 // A posição do livro associada à chave da direita será nula
        parentNode->right_child = -1;               // O filho à direita será nulo
        parentNode->middle_child = rightNodeOffset; // O filho do meio será o novo nó criado

        return rightNodeOffset;
    }

    // Se a chave a ser inserida é menor que a chave à esquerda
    if (key < parentNode->left_key)
    {
        // Promove a chave da esquerda
        *promotedKey = parentNode->left_key;

        // Cria um novo nó à direita
        rightNodeOffset = createNode23(indexFile, parentNode->right_key, -1, parentNode->rightBook, -1, -1, -1, -1, 1, header);

        // Atualiza o nó pai
        parentNode->nKeys = 1;                      // O nó pai terá 1 chave
        parentNode->left_key = key;                 // A chave da esquerda será a chave inserida
        parentNode->right_key = -1;                 // A chave da direita será nula
        parentNode->rightBook = -1;                 // A posição do livro associada à chave da direita será nula
        parentNode->right_child = -1;               // O filho à direita será nulo
        parentNode->middle_child = rightNodeOffset; // O filho do meio será nulo

        return rightNodeOffset;
    }
}

/**
 * @brief Função auxiliar recursiva para inserção de uma chave em uma árvore 2-3.
 *
 * Esta função insere uma chave na árvore 2-3, propagando a inserção recursivamente até
 * alcançar um nó folha. Caso um nó ultrapasse sua capacidade máxima de chaves, ele será
 * dividido e a chave promovida para o nó pai. O processo pode envolver divisões
 * recursivas até a raiz.
 *
 * @pre O arquivo de índice deve estar aberto no modo de leitura e escrita. O nó pai e o nó
 *     atual devem estar corretamente carregados a partir do arquivo de índice. O cabeçalho
 *    do arquivo de índice deve estar carregado e disponível para atualização.
 *
 * @post A chave é inserida na árvore 2-3, com divisões de nós conforme necessário.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore é armazenada.
 * @param parentNodeOffset Deslocamento do nó pai no arquivo de índice.
 * @param parentNode Ponteiro para o nó pai na árvore 2-3.
 * @param key A chave a ser inserida na árvore.
 * @param promotedKey Ponteiro para a chave promovida do nó dividido, se houver.
 * @param subarvoreOffset Deslocamento da subárvore usada para a inserção.
 * @param header Ponteiro para o cabeçalho do arquivo de índice.
 * @param leftBookOffset Deslocamento do livro à esquerda no arquivo de índice.
 * @param rightBookOffset Deslocamento do livro à direita no arquivo de índice.
 * @return O deslocamento do nó pai após a inserção. Caso haja uma divisão, retorna o deslocamento do nó pai
 *         atualizado; caso contrário, retorna -1 indicando que não houve divisão.
 *
 * @note A função realiza a inserção de forma recursiva e cuida das divisões de nós conforme necessário.
 *       Ela também lida com a descida para os filhos apropriados com base na chave a ser inserida.
 */
static int insertKeyAux(FILE *indexFile, int parentNodeOffset, Node23 *parentNode, int key, int *promotedKey, int subarvoreOffset, IndexFileHeader *header, int leftBookOffset, int rightBookOffset)
{
    // Caso o nó seja folha, o processo de inserção é direto ou envolve uma divisão.
    if (isLeafNode(parentNode))
    {
        // Caso o nó tenha apenas uma chave, adiciona a nova chave e salva o nó.
        if (parentNode->nKeys == 1)
        {
            addKeyToNodeWithOneKey(parentNode, key, leftBookOffset);
            saveNode(indexFile, parentNodeOffset, parentNode);
            return parentNodeOffset;
        }
        // Caso o nó tenha duas chaves, é necessário dividir o nó.
        else
        {
            return splitNode(indexFile, parentNodeOffset, parentNode, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
    }
    else // Caso o nó não seja folha, desce na árvore.
    {
        Node23 auxTree;
        int auxKey;

        // Determina para qual filho descer, dependendo da chave a ser inserida.
        if (key < parentNode->left_key) // Desce para o filho da esquerda.
        {
            auxTree = loadNode23(indexFile, parentNode->left_child);
            auxKey = insertKeyAux(indexFile, parentNode->left_child, &auxTree, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
        else if (parentNode->nKeys == 1 || key < parentNode->right_key) // Desce para o filho do meio.
        {
            auxTree = loadNode23(indexFile, parentNode->middle_child);
            auxKey = insertKeyAux(indexFile, parentNode->middle_child, &auxTree, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
        else // Desce para o filho da direita.
        {
            auxTree = loadNode23(indexFile, parentNode->right_child);
            auxKey = insertKeyAux(indexFile, parentNode->right_child, &auxTree, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }

        // Se não houve divisão, retorna -1.
        if (auxKey == -1)
        {
            return -1;
        }

        // Se o nó tem espaço para a chave promovida, adiciona a chave e salva o nó.
        if (parentNode->nKeys == 1)
        {
            addKeyToNodeWithOneKey(parentNode, auxKey, leftBookOffset);
            saveNode(indexFile, parentNodeOffset, parentNode);
            return parentNodeOffset;
        }
        // Caso o nó esteja cheio, é necessário realizar uma divisão.
        else
        {
            return splitNode(indexFile, parentNodeOffset, parentNode, auxKey, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
    }
}

/**
 * @brief Insere uma chave na árvore 2-3.
 *
 * Esta função é responsável por inserir uma chave na árvore 2-3, criando um nó raiz caso a árvore esteja vazia
 * ou realizando a inserção recursiva nos nós existentes. Caso haja uma divisão de nó, a chave promovida será
 * colocada em um novo nó raiz. O cabeçalho do arquivo de índice será atualizado conforme necessário.
 *
 * @pre O arquivo de índice deve estar aberto no modo de leitura e escrita. O cabeçalho do arquivo de índice
 *     deve estar carregado e disponível para atualização. A chave a ser inserida deve ser fornecida.
 *
 * @post A chave é inserida na árvore 2-3, com divisões de nós conforme necessário. O cabeçalho do arquivo de índice
 *      é atualizado com o novo endereço da raiz, se necessário.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore 2-3 é armazenada.
 * @param key A chave a ser inserida na árvore.
 * @param bookPosition Posição do livro associada à chave a ser inserida.
 * @param header Ponteiro para o cabeçalho do arquivo de índice, que contém informações da árvore, incluindo o endereço da raiz.
 *
 * @return O endereço do nó raiz após a inserção. Caso não haja divisão de nó, retorna `0` indicando sucesso.
 *
 * @note Se a árvore estiver vazia, um novo nó raiz será criado com a chave inserida. Caso contrário, a inserção
 *       será realizada recursivamente, e se necessário, a árvore será reestruturada com a criação de um novo nó raiz
 *       para acomodar a chave promovida.
 */
int insertKey(FILE *indexFile, int key, int bookPosition, IndexFileHeader *header)
{
    // Recupera o endereço da raiz da árvore
    int root = getRootAddress(indexFile);

    // Se a árvore está vazia
    if (root == -1)
    {
        // Cria um novo nó raiz com a chave inserida
        int newRoot = createNode23(indexFile, key, -1, bookPosition, -1, -1, -1, -1, 1, header);

        // Atualiza o endereço da raiz no cabeçalho
        header->rootAddress = newRoot;
        saveHeader(indexFile, header, sizeof(IndexFileHeader));

        return newRoot;
    }
    else
    {
        // Carrega o nó raiz existente
        Node23 rootNode = loadNode23(indexFile, root);

        int promotedKey;
        // Insere a chave recursivamente na árvore
        int newRoot = insertKeyAux(indexFile, root, &rootNode, key, &promotedKey, -1, header, bookPosition, -1);

        if (newRoot != -1)
        {
            // Caso tenha ocorrido uma divisão, cria um novo nó raiz
            newRoot = createNode23(indexFile, promotedKey, -1, -1, -1, root, newRoot, -1, 1, header);

            // Atualiza o cabeçalho com o novo endereço da raiz
            header->rootAddress = newRoot;
            saveHeader(indexFile, header, sizeof(IndexFileHeader));
        }
        else
        {
            // Se não houve divisão, apenas salva o nó raiz atualizado
            saveNode(indexFile, root, &rootNode);
        }
    }

    return 0; // Retorna 0 para indicar sucesso
}

/**
 * @brief Remove uma chave de um nó folha na árvore 2-3.
 *
 * Esta função é responsável por remover uma chave de um nó folha da árvore 2-3. Se a chave a ser removida
 * for a chave à esquerda, ela é substituída pela chave à direita. Se a chave removida for a chave à direita,
 * ela é simplesmente apagada. Após a remoção, o número de chaves do nó é atualizado.
 *
 * @pre O nó passado deve ser um nó folha da árvore 2-3. A chave a ser removida deve existir no nó.
 *
 * @post A chave é removida do nó folha, e o número de chaves é atualizado.
 *
 * @param node Ponteiro para o nó folha de onde a chave será removida.
 * @param key A chave a ser removida do nó.
 *
 * @note Esta função assume que o nó passado é um nó folha. Se a chave removida for a chave à esquerda,
 *       a chave à direita é movida para a posição da chave à esquerda. Se a chave removida for a chave à direita,
 *       ela é removida diretamente, e o número de chaves do nó é decrementado.
 */
static void removeKeyFromLeaf(Node23 *node, int key)
{
    // Verifica qual chave remover
    if (node->left_key == key)
    {
        // Move a chave da direita para a esquerda
        node->left_key = node->right_key;
        node->leftBook = node->rightBook;

        // Limpa a chave da direita e a posição do livro associada
        node->right_key = -1;
        node->rightBook = -1;
    }
    else if (node->right_key == key)
    {
        // Remove a chave da direita
        node->right_key = -1;
        node->rightBook = -1;
    }
    else
    {
        // Chave não encontrada
        return;
    }

    // Atualiza o número de chaves
    node->nKeys--;
}

/**
 * @brief Encontra o endereço do pai de um nó na árvore 2-3.
 *
 * Esta função realiza a busca do pai de um nó na árvore 2-3. Dado o endereço do nó e sua chave,
 * ela determina se o nó em questão é filho esquerdo, do meio ou direito de um nó pai. Se o nó
 * não for encontrado ou não tiver um pai válido, a função retorna -1.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore está armazenada.
 * @param parentAddress Endereço do nó pai a ser verificado.
 * @param parentNode Ponteiro para o nó pai que contém as referências aos filhos.
 * @param nodeAddress Endereço do nó filho cujo pai está sendo buscado.
 * @param key A chave a ser verificada para determinar a relação com o nó pai.
 *
 * @pre O nó pai (`parentNode`) deve estar carregado e o endereço do nó filho (`nodeAddress`)
 *      deve ser válido. O arquivo de índice deve estar acessível e conter a árvore 2-3.
 *
 * @post Se o nó for encontrado e for filho esquerdo, do meio ou direito, o endereço do pai
 *       será retornado. Se o nó não tiver pai ou não for encontrado, retorna -1.
 *
 * @return O endereço do pai do nó se encontrado, ou -1 caso contrário.
 *
 * @note A função assume que o nó pai é um nó não folha. Se o nó filho for folha, a função
 *       retorna imediatamente -1, pois folhas não têm pais.
 */
static int findParentAux(FILE *indexFile, int parentAddress, Node23 *parentNode, int nodeAddress, int key)
{
    if (isLeafNode(parentNode))
    {
        return -1; // Nó folha, não tem pai
    }

    // Verificando se o nodeAddres é filho da esquerda, meio ou direita
    if (parentNode->left_child == nodeAddress)
    {
        if (key < parentNode->left_key)
        {
            return parentAddress; // Não tem pai
        }
        else
        {
            return -1;
        }
    }
    else if (parentNode->middle_child == nodeAddress)
    {
        if (key >= parentNode->left_key && key < parentNode->right_key)
        {
            return parentAddress;
        }
        else
        {
            return -1; // Não tem pai
        }
    }
    else if (parentNode->right_child == nodeAddress)
    {
        if (key >= parentNode->right_key)
        {
            return parentAddress;
        }
        else
        {
            return -1; // Não tem pai
        }
    }

    return -1; // Não tem pai
}

/**
 * @brief Encontra o endereço do pai de um nó na árvore 2-3.
 *
 * Esta função busca o pai de um nó em uma árvore 2-3, dado o endereço do nó e o cabeçalho do arquivo de índice.
 * A busca começa pela raiz da árvore, e se o nó for encontrado, o endereço do pai é retornado. Se o nó não tiver pai
 * (se for a raiz ou a árvore estiver vazia), a função retorna -1.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore está armazenada.
 * @param nodeAddress Endereço do nó filho para o qual o pai está sendo buscado.
 * @param header Cabeçalho do arquivo de índice que contém informações da árvore.
 *
 * @return O endereço do pai do nó, ou -1 caso contrário.
 *
 * @pre O arquivo de índice deve estar carregado e acessível, e a árvore 2-3 deve estar bem estruturada.
 *
 * @post Se o nó filho for encontrado, o endereço do pai será retornado. Se o nó for a raiz ou se a árvore estiver vazia,
 *       retorna -1. Se o nó não for encontrado, a função também retorna -1.
 *
 * @note Caso a árvore esteja vazia ou o nó fornecido seja a raiz, a função retorna imediatamente -1.
 */
static int findParent(FILE *indexFile, int nodeAddress, IndexFileHeader *header)
{
    int root = getRootAddress(indexFile);

    if (root == -1 || root == nodeAddress)
    {
        return -1; // Árvore vazia ou nó raiz
    }

    // Carrega a raiz da árvore
    Node23 rootNode = loadNode23(indexFile, root);

    return findParentAux(indexFile, root, &rootNode, nodeAddress, -1);
}

/**
 * @brief Encontra o irmão de um nó em uma árvore 2-3.
 *
 * Esta função busca o irmão de um nó, dado o endereço do nó filho e o endereço do nó pai. A busca é realizada com base
 * na relação de filhos na árvore 2-3. Dependendo da posição do nó filho (esquerdo, meio ou direito), o irmão será retornado.
 * Se o nó não tiver irmãos ou se houver um erro com a estrutura do nó pai, a função retorna -1.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore está armazenada.
 * @param parentAddress Endereço do nó pai do nó filho para o qual estamos buscando o irmão.
 * @param nodeAddress Endereço do nó filho para o qual o irmão está sendo buscado.
 * @param header Cabeçalho do arquivo de índice que contém informações da árvore.
 *
 * @return O endereço do irmão do nó, ou -1 em caso de erro ou se o nó não tiver irmão.
 *
 * @pre O arquivo de índice deve estar carregado e acessível, e a árvore 2-3 deve estar bem estruturada.
 *      O nó pai deve conter filhos válidos.
 *
 * @post Se o nó filho tiver um irmão, o endereço do irmão será retornado. Se o nó pai estiver corrompido ou se o nó filho
 *       não tiver um irmão, retorna -1.
 *
 * @note Caso o nó pai tenha apenas um filho, o nó não terá irmão. Caso o nó procurado seja o filho esquerdo ou direito,
 *       o irmão será o filho do meio. Caso o nó seja o filho do meio, o irmão pode ser o filho à esquerda ou à direita,
 *       dependendo do número de chaves do nó pai.
 */
static int findSibling(FILE *indexFile, int parentAddress, int nodeAddress, IndexFileHeader *header)
{
    // Carregar o nó pai
    Node23 parentNode = loadNode23(indexFile, parentAddress);

    if (parentNode.left_child == -1 || parentNode.middle_child == -1 || parentNode.right_child == -1)
    {
        fprintf(stderr, "Erro: O nó pai está corrompido, faltando filhos.\n");
        return -1; // Se faltar algum filho, retorna erro
    }

    // Verificar se o nó procurado é o filho esquerdo
    if (parentNode.left_child == nodeAddress)
    {
        return parentNode.middle_child; // O irmão é o filho do meio
    }
    // Verificar se o nó procurado é o filho do meio
    else if (parentNode.middle_child == nodeAddress)
    {
        // O irmão pode ser o filho à esquerda ou à direita
        if (parentNode.nKeys == 1) // Caso o nó pai tenha apenas uma chave
        {
            return parentNode.left_child; // O irmão à esquerda
        }
        else
        {
            return parentNode.right_child; // O irmão à direita
        }
    }
    // Verificar se o nó procurado é o filho direito
    else if (parentNode.right_child == nodeAddress)
    {
        return parentNode.middle_child; // O irmão é o filho do meio
    }

    return -1; // Nó sem irmãos
}

/**
 * @brief Trata o underflow (falta de chaves) em um nó folha de uma árvore 2-3.
 *
 * Esta função é chamada quando um nó folha em uma árvore 2-3 não possui chaves e ocorre um underflow. O tratamento envolve
 * procurar o nó pai e verificar a possibilidade de redistribuição de chaves com um irmão ou fusão de nós. Caso o nó pai
 * também tenha underflow após a fusão, a função propaga o underflow para o nó pai.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore está armazenada.
 * @param nodeAddress Endereço do nó folha que sofreu underflow.
 * @param node Ponteiro para o nó que sofreu underflow.
 * @param header Cabeçalho do arquivo de índice, que contém informações sobre a árvore.
 * @param key A chave que causou o underflow (caso necessário para redistribuição ou fusão).
 *
 * @return Esta função não retorna nenhum valor. Ela faz alterações diretamente na estrutura da árvore.
 *
 * @pre O arquivo de índice deve estar carregado e acessível, a árvore 2-3 deve estar bem estruturada, e o nó folha
 *      deve ter um underflow (sem chaves).
 *
 * @post Após o tratamento, o underflow é resolvido, seja por redistribuição de chaves ou fusão de nós. Se necessário,
 *       o underflow é propagado para o nó pai. As alterações são salvas no arquivo de índice.
 *
 * @note Caso o nó pai fique vazio após a fusão de nós, a função `handleLeafUnderflow` é chamada recursivamente para
 *       tratar o underflow no nó pai.
 */
static void handleLeafUnderflow(FILE *indexFile, int nodeAddress, Node23 *node, IndexFileHeader *header, int key)
{
    if (node->nKeys > 0)
    {
        return; // Não há underflow
    }

    int parentAddress = findParent(indexFile, nodeAddress, header);

    if (parentAddress == -1)
    {
        // Se não tem pai e o nó está vazio, ajusta a raiz
        header->rootAddress = -1; // Árvore vazia
        saveHeader(indexFile, header, sizeof(IndexFileHeader));
        return;
    }

    Node23 parentNode = loadNode23(indexFile, parentAddress);

    // Localizar o irmão mais próximo para redistribuição ou fusão
    int siblingAddress = findSibling(indexFile, parentAddress, nodeAddress, header);

    if (siblingAddress == -1)
    {
        fprintf(stderr, "Erro: Nó sem irmãos em uma árvore 2-3.\n");
        return;
    }

    Node23 siblingNode = loadNode23(indexFile, siblingAddress);

    // Verifica redistribuição ou fusão
    if (siblingNode.nKeys == 2)
    {
        // Redistribuir chaves
        redistributeKeys(indexFile, &parentNode, &siblingNode, node, key);
    }
    else
    {
        // Mesclar com o irmão
        mergeNodes(indexFile, &parentNode, &siblingNode, node, key);

        // Se o pai agora está vazio, propaga o underflow
        if (parentNode.nKeys == 0)
        {
            handleLeafUnderflow(indexFile, parentAddress, &parentNode, header, key);
        }
    }

    // Salvar alterações
    saveNode(indexFile, parentAddress, &parentNode);
    saveNode(indexFile, siblingAddress, &siblingNode);
    saveNode(indexFile, nodeAddress, node);
}

/**
 * @brief Encontra a chave mínima em uma subárvore de uma árvore 2-3.
 *
 * Esta função percorre a subárvore à esquerda de um nó até chegar ao nó mais à esquerda da árvore.
 * O nó mais à esquerda de uma árvore 2-3 contém a chave mínima dentro de sua subárvore.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore está armazenada.
 * @param node Ponteiro para o nó a partir do qual a busca pela chave mínima será realizada.
 *
 * @return A chave mínima encontrada na subárvore do nó fornecido.
 *
 * @pre O arquivo de índice deve estar carregado e acessível, e o nó fornecido deve ser parte de uma árvore 2-3 válida.
 *
 * @post A função retorna a chave mais à esquerda da subárvore do nó fornecido.
 *
 * @note Esta função assume que a árvore está estruturada corretamente e que a subárvore do nó tem pelo menos um nó.
 */
static int findMinInSubtree(FILE *indexFile, Node23 *node)
{
    // Percorrer a subárvore à esquerda até chegar ao nó mais à esquerda
    while (node->left_child != -1)
    {
        *node = loadNode23(indexFile, node->left_child);
    }

    // Retorna a chave mais à esquerda do nó
    return node->left_key;
}

/**
 * @brief Encontra o sucessor em ordem de uma chave em um nó de uma árvore 2-3.
 *
 * Esta função busca o sucessor em ordem de uma chave específica dentro de um nó da árvore 2-3.
 * O sucessor em ordem de uma chave é o próximo valor em uma travessia em ordem da árvore.
 * A função lida com diferentes casos para encontrar o sucessor dependendo do número de chaves no nó e da posição da chave procurada.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore está armazenada.
 * @param node Ponteiro para o nó onde a chave procurada está localizada.
 * @param key A chave cujo sucessor em ordem será buscado.
 *
 * @return O sucessor em ordem da chave fornecida, ou -1 se não houver sucessor.
 *
 * @pre O arquivo de índice deve estar carregado e acessível, e o nó fornecido deve ser parte de uma árvore 2-3 válida.
 *
 * @post A função retorna o sucessor em ordem da chave fornecida ou -1 se a chave não for encontrada ou não houver sucessor.
 *
 * @note Esta função assume que o nó e a chave fornecida são válidos dentro da árvore 2-3.
 */
static int findInOrderSuccessor(FILE *indexFile, Node23 *node, int key)
{
    // Se o nó tem apenas uma chave e a chave procurada é a esquerda, o sucessor será a chave à direita
    if (node->nKeys == 1)
    {
        if (node->left_key == key)
        {
            return node->right_key;
        }
        else
        {
            return -1; // A chave não está presente
        }
    }

    // Se o nó tem duas chaves e a chave procurada é a esquerda
    if (node->left_key == key)
    {
        // Se o nó tem um filho à direita, o sucessor será o menor valor na subárvore direita
        if (node->right_child != -1)
        {
            Node23 rightChild = loadNode23(indexFile, node->right_child);
            return findMinInSubtree(indexFile, &rightChild); // Encontrar o valor mínimo na subárvore à direita
        }
        return -1; // Não há sucessor
    }

    // Se a chave procurada é a direita
    else if (node->right_key == key)
    {
        return findParent(indexFile, node->right_child, key);
    }

    return -1; // Chave não encontrada
}

/**
 * @brief Substitui uma chave em um nó de uma árvore 2-3.
 *
 * Esta função substitui uma chave específica em um nó da árvore 2-3 com uma nova chave fornecida.
 * A substituição pode ocorrer tanto na chave à esquerda quanto na chave à direita, se a chave procurada for encontrada.
 *
 * @param node Ponteiro para o nó onde a chave será substituída.
 * @param key A chave que será substituída.
 * @param newKey A nova chave que substituirá a chave fornecida.
 *
 * @return 1 se a substituição for bem-sucedida, ou 0 se a chave não for encontrada.
 *
 * @pre O nó deve ser parte de uma árvore 2-3 válida.
 *
 * @post Se a chave fornecida for encontrada, ela será substituída pela nova chave.
 *       Se a chave não for encontrada, o nó permanecerá inalterado.
 *
 * @note Esta função pressupõe que o nó contém apenas duas chaves (esquerda e direita).
 *       A árvore 2-3 permite que um nó tenha duas chaves e, consequentemente, um máximo de dois valores.
 */
static int replaceKey(Node23 *node, int key, int newKey)
{
    // Verificar se o nó contém a chave que será substituída
    if (node->left_key == key)
    {
        node->left_key = newKey;
        return 1; // Substituição bem-sucedida
    }
    else if (node->right_key == key)
    {
        node->right_key = newKey;
        return 1; // Substituição bem-sucedida
    }

    // Caso a chave não seja encontrada
    return 0; // Falha na substituição, chave não encontrada
}

/**
 * @brief Lida com um nó vazio e atualiza a posição de alocação de novos nós na árvore 2-3.
 *
 * Esta função é responsável por tratar um nó vazio em uma árvore 2-3. Caso o nó tenha sido removido ou
 * tenha se tornado vazio, ele será reutilizado ou desalocado. Se a posição de um nó livre já estiver
 * disponível, o nó será marcado como vazio e realocado. Caso contrário, um novo nó será alocado na próxima
 * posição disponível.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde os nós da árvore 2-3 são armazenados.
 * @param node Ponteiro para o nó que será tratado.
 * @param header Cabeçalho do arquivo de índice, contendo informações sobre a alocação de nós livres.
 *
 * @pre O arquivo de índice deve estar aberto e válido, e o cabeçalho deve estar carregado corretamente.
 *
 * @post Se o nó estiver vazio, ele será realocado ou removido, dependendo da disponibilidade de espaços livres.
 *       Se a posição de um nó livre não for encontrada, um novo nó será alocado na próxima posição disponível.
 *       As atualizações no cabeçalho do arquivo serão salvas.
 *
 * @note Esta função considera que a posição de nós livres é gerenciada por duas variáveis no cabeçalho:
 *       `headEmptyPosition` para a posição do nó livre, e `firstEmptyPosition` para a próxima posição disponível
 *       para alocar novos nós.
 */
static void handleEmptyNode(FILE *indexFile, Node23 *node, IndexFileHeader *header)
{
    // Se o nó não estiver vazio, devemos realocar a posição do nó livre
    if (header->headEmptyPosition != -1)
    {
        // Atualiza o nó livre com a posição do nó vazio
        Node23 freeNode;
        freeNode.left_child = header->headEmptyPosition;
        freeNode.nKeys = 0;

        // Salva o nó na posição disponível (header->headEmptyPosition)
        saveNode(indexFile, header->headEmptyPosition, &freeNode);

        // Atualiza a posição do próximo nó livre
        header->headEmptyPosition = freeNode.left_child;

        // Salva as atualizações no cabeçalho
        saveHeader(indexFile, header, sizeof(IndexFileHeader));
    }
    else
    {
        node->nKeys = 0;

        saveNode(indexFile, header->firstEmptyPosition, node);

        header->firstEmptyPosition++; // Supondo que a posição seguinte seja a próxima para alocar um novo nó livre
        saveHeader(indexFile, header, sizeof(IndexFileHeader));
    }
}

/**
 * @brief Remove uma chave da árvore 2-3 armazenada em um arquivo de índice.
 *
 * Esta função remove uma chave específica da árvore 2-3. Ela lida com diferentes casos, como quando a chave
 * está em um nó folha ou em um nó interno. Quando a chave não é encontrada, ou se a árvore estiver vazia,
 * a função retorna -1. Caso contrário, ela faz a remoção da chave e ajusta a árvore para manter a propriedade
 * da árvore 2-3. O nó pode precisar ser realocado ou pode ocorrer um "underflow", o que exigirá reequilíbrio da árvore.
 *
 * @param indexFile Ponteiro para o arquivo de índice onde a árvore 2-3 está armazenada.
 * @param key A chave que deve ser removida da árvore 2-3.
 * @param header Cabeçalho do arquivo de índice, contendo informações sobre a raiz e a estrutura da árvore.
 *
 * @return Retorna 0 se a remoção foi bem-sucedida, ou -1 se a chave não foi encontrada ou a árvore estiver vazia.
 *
 * @pre O arquivo de índice deve estar aberto e válido, e o cabeçalho deve estar carregado corretamente.
 *
 * @post A árvore 2-3 é modificada, e a chave especificada é removida. Dependendo da situação, a árvore pode ser
 *       balanceada ou ajustada após a remoção. Se necessário, a raiz será modificada e o nó pode ser desalocado.
 *
 * @note Esta função lida com diferentes cenários para remoção:
 *       - Se a chave estiver em um nó folha, ela é removida diretamente.
 *       - Se o nó folha ficar desequilibrado (com menos de uma chave), a função `handleLeafUnderflow` é chamada.
 *       - Se a chave estiver em um nó interno, a chave é substituída pelo sucessor em ordem, e a remoção é recursiva.
 */
int removeKey(FILE *indexFile, int key, IndexFileHeader *header)
{
    if (twoThreeTreeSearch(indexFile, key) == -1 || header->rootAddress == -1)
    {
        return -1; // Chave não encontrada ou árvore vazia
    }

    // Carrega a raiz
    Node23 rootNode = loadNode23(indexFile, header->rootAddress);

    // Se a raiz é uma folha
    if (isLeafNode(&rootNode))
    {
        if (rootNode.nKeys == 2)
        {
            removeKeyFromLeaf(&rootNode, key);
            saveNode(indexFile, header->rootAddress, &rootNode);
        }
        else
        {
            handleLeafUnderflow(indexFile, header->rootAddress, &rootNode, header, key);
        }
    }
    else
    {
        // Se a raiz não for uma folha, encontra o sucessor em ordem
        int successorKey = findInOrderSuccessor(indexFile, &rootNode, key);

        // Substitui a chave pela chave sucessora
        replaceKey(&rootNode, key, successorKey);
        saveNode(indexFile, header->rootAddress, &rootNode);

        // Remove recursivamente a chave sucessora
        removeKey(indexFile, successorKey, header);
    }

    // Se a raiz ficou sem chaves, trata o nó vazio
    if (rootNode.nKeys == 0)
    {
        handleEmptyNode(indexFile, &rootNode, header);
    }

    return 0; // Sucesso
}

/**
 * @brief Função recursiva para contar o número de nós em uma árvore 2-3.
 *
 * Esta função é chamada recursivamente para percorrer todos os nós de uma árvore 2-3 e contar a quantidade total de nós.
 * Cada nó pode ter até três filhos, dependendo do número de chaves que ele contém.
 * Se o nó contém uma chave, ele tem dois filhos. Se o nó contém duas chaves, ele tem três filhos.
 *
 * @param node O nó atual a ser contado. A função recursiva chama esta função para os filhos do nó.
 * @param indexFile O arquivo binário onde os nós da árvore estão armazenados. Este arquivo é utilizado
 *                  para carregar os filhos do nó, caso o nó tenha filhos.
 *
 * @return O número total de nós na subárvore com a raiz no nó fornecido.
 *         Retorna 1 para o nó atual, e adiciona a contagem dos filhos (se existirem).
 */
static int twoThreeTreeCountNodesRec(Node23 node, FILE *indexFile)
{
    int count = 1; // Conta o nó atual

    // Se o nó tem filhos (nKeys == 1 ou nKeys == 2), conta os filhos recursivamente
    if (node.nKeys == 1)
    {
        // Conta os dois filhos
        count += twoThreeTreeCountNodesRec(loadNode23(indexFile, node.left_child), indexFile);
        count += twoThreeTreeCountNodesRec(loadNode23(indexFile, node.middle_child), indexFile);
    }
    else if (node.nKeys == 2)
    {
        // Conta os três filhos
        count += twoThreeTreeCountNodesRec(loadNode23(indexFile, node.left_child), indexFile);
        count += twoThreeTreeCountNodesRec(loadNode23(indexFile, node.middle_child), indexFile);
        count += twoThreeTreeCountNodesRec(loadNode23(indexFile, node.right_child), indexFile);
    }

    return count;
}

/**
 * @brief Conta o número de nós na árvore 2-3 a partir da raiz.
 *
 * Esta função inicializa o processo de contagem de nós de uma árvore 2-3. Ela começa recuperando o endereço do nó raiz
 * da árvore, carregando o nó e então chamando a função recursiva `twoThreeTreeCountNodesRec` para contar os nós na árvore inteira.
 *
 * Se a árvore estiver vazia (raiz não encontrada ou inválida), a função retorna 0.
 *
 *
 * @param indexFile O arquivo binário onde os nós da árvore estão armazenados.
 *
 * @return O número total de nós na árvore 2-3.
 *         Retorna 0 se a árvore estiver vazia.
 */
int twoThreeTreeCountNodes(FILE *indexFile)
{
    // Obtém o endereço da raiz da árvore 2-3
    int rootAddress = getRootAddress(indexFile);

    // Verifica se a raiz é válida
    if (rootAddress == -1)
    {
        printf("Árvore 2-3 vazia.\n");
        return 0;
    }

    // Carrega o nó raiz da árvore usando a função loadNode23
    Node23 root = loadNode23(indexFile, rootAddress);

    // Função recursiva para contar os nós
    int count = twoThreeTreeCountNodesRec(root, indexFile);

    return count;
}
