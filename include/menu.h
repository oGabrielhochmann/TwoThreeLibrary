/**
 * @file menu.h
 *
 * @brief Contém funções para exibição e manipulação do menu principal.
 *
 * @author Gabriel Hochmann
 * @see menu.c
 */

#ifndef menu_h
#define menu_h

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
