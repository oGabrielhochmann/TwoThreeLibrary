#ifndef menu_h
#define menu_h

/**
 * @brief Exibe um menu com as opções disponíveis.
 *
 * Esta função é responsável por imprimir o menu na tela, mostrando um título e as opções disponíveis.
 * O menu é impresso de forma formatada, com uma borda superior e inferior, e as opções numeradas.
 *
 * @pre `title` não deve ser NULL.
 * @pre `option` não deve ser NULL.
 * @pre `numOptions` deve ser maior que 0.
 * @pre `width` deve ser maior que 2.
 *
 * @post O menu é impresso na tela com as opções fornecidas, e o título aparece no topo do menu.
 *
 * @param title Título do menu.
 * @param option Array de strings com as opções disponíveis.
 * @param numOptions Número de opções disponíveis.
 * @param width Largura do menu.
 */
void displayMenu(const char *title, const char *option[], int numOptions, int width);

/**
 * @brief Lê a escolha do usuário.
 *
 * Esta função solicita ao usuário que insira uma opção de menu e retorna a opção selecionada.
 * Se a entrada for inválida, a entrada será descartada e a função retorna -1 para indicar um erro.
 *
 * @pre Nenhuma.
 * @post Nenhuma.
 *
 * @return A opção selecionada pelo usuário, ou -1 se a entrada for inválida.
 */
int getMenuChoice();

/**
 * @brief Manipula o submenu de livres que está relacionada a manipulação da lista de livres.
 *
 * Este submenu oferece ao usuário três opções: sair, imprimir do arquivo de índices e imprimir do arquivo de dados.
 * O submenu permanece em execução até que o usuário escolha a opção de sair.
 *
 * @pre Nenhuma.
 * @post o submenu é exibido na tela e as opções são tratadas de acordo com a escolha do usuário.
 *
 * @param Nenhum.
 *
 * @return Nenhum. A função não retorna nada.
 */
void handleSubMenuFreeList();

/**
 * @brief Manipula o submenu de quantidades que está relacionada a manipulação de quantidades de livros.
 *
 * Este submenu oferece ao usuário cinco opções: sair, total de livros diferentes, total de livros em estoque, total de livros por autor, total de livros por editora e total de livros por ano de lançamento.
 * O submenu permanece em execução até que o usuário escolha a opção de sair.
 *
 * @pre Nenhuma.
 * @post o submenu é exibido na tela e as opções são tratadas de acordo com a escolha do usuário.
 *
 * @param Nenhum.
 *
 * @return Nenhum. A função não retorna nada.
 */
void handleSubMenuQuantities();

/**
 * @brief Exibe o menu principal e manipula as escolhas do usuário.
 *
 * Esta função exibe o menu principal com as opções principais de operações, incluindo cadastro, remoção, impressão e listagem de livros, etc.
 * O menu principal permanece em execução até que o usuário escolha a opção de sair.
 *
 * @pre Nenhuma.
 * @post O menu principal é exibido na tela e as opções são tratadas de acordo com a escolha do usuário.
 *
 * @param Nenhum.
 * @return Nenhum. A função não retorna nada.
 */
void handleChoice();

#endif /* menu_h */