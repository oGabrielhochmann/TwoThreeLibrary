/**
 * @file tree_manager.h
 * @see tree_manager.c
 *
 * @brief Contém funções para gerenciamento da árvore 2-3, incluindo inserção, remoção e busca de registros.
 *
 * @author Gabriel Hochmann
 */

#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

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
int createNode23(FILE *indexFile, int leftKey, int rightKey, int leftBook, int rightBook, int leftChild, int middleChild, int rightChild, int nKeys, IndexFileHeader *header);

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
Node23 loadNode23(FILE *indexFile, int offset);

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
int getRootAddress(FILE *file);

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
int twoThreeTreeSearch(FILE *file, int key);

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
int insertKey(FILE *indexFile, int key, int bookPosition, IndexFileHeader *header);

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
int removeKey(FILE *indexFile, int key, IndexFileHeader *header);

#endif /* TREE_MANAGER_H */