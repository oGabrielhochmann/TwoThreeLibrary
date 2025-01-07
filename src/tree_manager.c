/**
 * @file tree_manager.c
 * 
 * @brief Contém funções para gerenciamento da arvore 2-3.
 * 
 * Este arquivo contém funções para gerenciamento da arvore 2-3, como busca, inserção e remoção de chaves.
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

/**
 * @brief Cria um nó 2-3 no arquivo de índices.
 * 
 * Esta função é responsável por criar um nó 2-3 no arquivo de índices.
 * 
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita.
 * 
 * @post O nó 2-3 é criado no arquivo de índices.
 * 
 * @param indexFile Ponteiro para o arquivo de índices.
 * @param leftKey Chave da esquerda.
 * @param rightKey Chave da direita.
 * @param leftBook Posição do livro (dado) da esquerda.
 * @param rightBook Posição do livro (dado) da direita.
 * @param leftChild Filho da esquerda.
 * @param middleChild Filho do meio.
 * @param rightChild Filho da direita.
 * @param nKeys Número de chaves no nó.
 * @param header Ponteiro para o cabeçalho do arquivo de índices.
 * 
 * @return Inteiro representando o deslocamento do nó no arquivo de índices.
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
 *
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita.
 * 
 * @post O endereço da raiz da árvore 2-3 é lido a partir do cabeçalho do arquivo de índices.
 * 
 * @param file Ponteiro para o arquivo de índices.
 *
 * @return Endereço da raiz da árvore 2-3.
 *
 * @note O endereço da raiz é lido a partir do cabeçalho do arquivo de índices.
 *      O cabeçalho do arquivo de índices é composto por três informações: a raiz da árvore, a primeira posição livre e a cabeça dos registros livres.
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
 * Esta função é responsável por realizar a busca de um nó na árvore 2-3 a partir de uma chave.
 * 
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita.
 * 
 * @post O nó é buscado na árvore 2-3 a partir da chave especificada.
 *
 * @param file Ponteiro para o arquivo de índices.
 * @param root Endereço da raiz da árvore 2-3.
 * @param key Chave a ser buscada.
 *
 * @return Inteiro representando o endereço do nó na árvore 2-3 (arquivo de índices).
 */
int searchNode(FILE *file, int root, int key)
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
 * Esta função é responsável por realizar a busca de uma chave na árvore 2-3.
 * 
 * @pre O arquivo de índices deve estar aberto no modo de leitura e escrita.
 * 
 * @post A chave é buscada na árvore 2-3.
 *
 * @param file Ponteiro para o arquivo de índices.
 * @param key Chave a ser buscada.
 *
 * @return Inteiro representando o endereço do nó na árvore 2-3 (arquivo de índices).
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
 * Esta função é responsável por adicionar uma chave em um nó que tem espaço para 1 chave.
 * 
 * @pre O nó deve ter espaço para 1 chave.
 * 
 * @post A chave é adicionada ao nó.
 *
 * @param node Ponteiro para o nó.
 * @param key Chave a ser adicionada.
 * @param bookPosition Posição do livro (dado) a ser inserido.
 *
 * @return Nenhum.
 */
void addKeyToNodeWithOneKey(Node23 *node, int key, int bookPosition)
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
 * Esta função é responsável por verificar se um nó é uma folha.
 * 
 * @pre Nenhuma.
 * 
 * @post Nenhuma.
 *
 * @param node Ponteiro para o nó.
 *
 * @return 1 se o nó é uma folha, 0 caso contrário.
 */
int isLeafNode(Node23 *node)
{
    return node->left_child == -1;
}

int splitNode(FILE *indexFile, int parentNodeOffset, Node23 *parentNode, int key, int *promotedKey, int subarvoreOffset, IndexFileHeader *header, int leftBookOffset, int rightBookOffset)
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

int insertKeyAux(FILE *indexFile, int parentNodeOffset, Node23 *parentNode, int key, int *promotedKey, int subarvoreOffset, IndexFileHeader *header, int leftBookOffset, int rightBookOffset)
{
    if (isLeafNode(parentNode))
    {
        if (parentNode->nKeys == 1)
        {
            addKeyToNodeWithOneKey(parentNode, key, leftBookOffset);
            saveNode(indexFile, parentNodeOffset, parentNode);
            return parentNodeOffset;
        }
        else
        {
            return splitNode(indexFile, parentNodeOffset, parentNode, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
    }
    else // Precisa descer na árvore
    {
        Node23 auxTree;
        int auxKey;

        // Verifica em qual filho descer
        if (key < parentNode->left_key) // Desce para o filho da esquerda
        {
            auxTree = loadNode23(indexFile, parentNode->left_child);
            auxKey = insertKeyAux(indexFile, parentNode->left_child, &auxTree, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
        else if (parentNode->nKeys == 1 || key < parentNode->right_key) // Desce para o filho do meio
        {
            auxTree = loadNode23(indexFile, parentNode->middle_child);
            auxKey = insertKeyAux(indexFile, parentNode->middle_child, &auxTree, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
        else // Desce para o filho da direita
        {
            auxTree = loadNode23(indexFile, parentNode->right_child);
            auxKey = insertKeyAux(indexFile, parentNode->right_child, &auxTree, key, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }

        // Se não houve split, não precisa fazer nada
        if (auxKey == -1)
        {
            return -1;
        }

        // Se o nó tem espaço, basta adicionar a chave
        if (parentNode->nKeys == 1)
        {
            addKeyToNodeWithOneKey(parentNode, auxKey, leftBookOffset);
            saveNode(indexFile, parentNodeOffset, parentNode);
            return parentNodeOffset;
        }
        else // Se o nó está cheio, é necessário fazer o split
        {
            return splitNode(indexFile, parentNodeOffset, parentNode, auxKey, promotedKey, subarvoreOffset, header, leftBookOffset, rightBookOffset);
        }
    }
}

int insertKey(FILE *indexFile, int key, int bookPosition, IndexFileHeader *header)
{
    int root = getRootAddress(indexFile);

    // Se a árvore está vazia
    if (root == -1)
    {
        // Cria um novo nó raiz
        int newRoot = createNode23(indexFile, key, -1, bookPosition, -1, -1, -1, -1, 1, header);

        // Atualiza o endereço da raiz no cabeçalho
        header->rootAddress = newRoot;
        saveHeader(indexFile, header, sizeof(IndexFileHeader));

        return newRoot;
    }
    else
    {
        Node23 rootNode = loadNode23(indexFile, root);

        int promotedKey;
        int newRoot = insertKeyAux(indexFile, root, &rootNode, key, &promotedKey, -1, header, bookPosition, -1);

        if (newRoot != -1)
        {
            // Cria um novo nó raiz e atualiza o cabeçalho
            newRoot = createNode23(indexFile, promotedKey, -1, -1, -1, root, newRoot, -1, 1, header);

            header->rootAddress = newRoot;
            saveHeader(indexFile, header, sizeof(IndexFileHeader));
        }
        else
        {
            // Caso não tenha havido split, apenas salva o nó raiz
            saveNode(indexFile, root, &rootNode);
        }
    }

    return 0; // Sucesso
}

void removeKeyFromLeaf(Node23 *node, int key)
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

int findParentAux(FILE *indexFile, int parentAddress, Node23 *parentNode, int nodeAddress, int key)
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

int findParent(FILE *indexFile, int nodeAddress, IndexFileHeader *header)
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

int findSibling(FILE *indexFile, int parentAddress, int nodeAddress, IndexFileHeader *header)
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

void handleLeafUnderflow(FILE *indexFile, int nodeAddress, Node23 *node, IndexFileHeader *header, int key)
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

int findMinInSubtree(FILE *indexFile, Node23 *node)
{
    // Percorrer a subárvore à esquerda até chegar ao nó mais à esquerda
    while (node->left_child != -1)
    {
        *node = loadNode23(indexFile, node->left_child);
    }

    // Retorna a chave mais à esquerda do nó
    return node->left_key;
}

int findInOrderSuccessor(FILE *indexFile, Node23 *node, int key)
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

int replaceKey(Node23 *node, int key, int newKey)
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

void handleEmptyNode(FILE *indexFile, Node23 *node, IndexFileHeader *header)
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
        int successorKey = findInOrderSuccessor(indexFile, &rootNode, key);

        replaceKey(&rootNode, key, successorKey);
        saveNode(indexFile, header->rootAddress, &rootNode);

        removeKey(indexFile, successorKey, header);
    }

    if (rootNode.nKeys == 0)
    {
        handleEmptyNode(indexFile, &rootNode, header);
    }

    return 0; // Sucesso
}
