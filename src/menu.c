/**
 * @file menu.c
 * @author Gabriel Hochmann
 * 
 * @brief Contém funções para exibição e manipulação do menu principal.
 * 
 * @see menu.h
 */

#include "menu.h"

#include <string.h>
#include <stdio.h>

/**
 * @brief Exibe uma borda para o menu.
 *
 * Esta função é responsável por imprimir uma borda para o menu na tela, com um tamanho específico.
 * A borda é impressa de forma formatada, com um caractere de barra vertical no início e no final, e traços no meio.
 *
 * @pre width deve ser maior que 2.
 *
 * @post A borda é impressa na tela com o tamanho fornecido.
 *
 * @param width Largura da borda.
 */
static void printMenuBorder(int width)
{
    printf("|");
    for (int i = 0; i < width - 2; i++)
    {
        printf("-");
    }
    printf("|\n");
}

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
static void displayMenu(const char *title, const char *option[], int numOptions, int width)
{
    printMenuBorder(width);

    printf("| %-*s |\n", width - 4, title);

    printMenuBorder(width);

    for (int i = 1; i < numOptions; i++)
    {
        printf("| %-2d. %-*s |\n", i, width - 8, option[i]);
    }
    printf("| 0 . %-*s |\n", width - 8, option[0]);

    printMenuBorder(width);
}

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
static int getMenuChoice()
{
    int choice;
    printf("Digite uma opcao:\n");

    if (scanf("%d", &choice) != 1)
    {
        // Limpar buffer de entrada
        while (getchar() != '\n')
            ;
        return -1; // Indica erro
    }

    return choice;
}

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
static void handleSubMenuFreeList()
{
    const char *options[] = {
        "Sair.",
        "Imprimir do arquivo de indices.",
        "Imprimir do arquivo de dados."};

    int numOptions = sizeof(options) / sizeof(options[0]);
    int choice;

    do
    {
        displayMenu("SUB-MENU", options, numOptions, 60);
        choice = getMenuChoice();
        switch (choice)
        {
        case 0:
            printf("Saindo do Sub-Menu de Livres...\n");
            break;
        case 1:
            // chamar função de impressão de livres do arquivo de índices
            break;
        case 2:
            // chamar função de impressão de livres do arquivo de dados
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
        }
    } while (choice != 0);
}

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
static void handleSubMenuQuantities()
{
    const char *options[] = {
        "Sair.",
        "Total de livros diferentes.",
        "Total de livros em estoque.",
        "Total de livros por autor.",
        "Total de livros por editora.",
        "Total de livros por ano de lançamento."};

    int numOptions = sizeof(options) / sizeof(options[0]);
    int choice;

    do
    {
        displayMenu("SUB-MENU", options, numOptions, 60);
        choice = getMenuChoice();
        switch (choice)
        {
        case 0:
            printf("Saindo...\n");
            break;
        case 1:
            // chamar função de calcular total de livros diferentes
            break;
        case 2:
            // chamar função de calcular total de livros em estoque
            break;
        case 3:
            // chamar função de calcular total de livros por autor
            break;
        case 4:
            // chamar função de calcular total de livros por editora
            break;
        case 5:
            // chamar função de calcular total de livros por ano de lançamento
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
        }
    } while (choice != 0);
}

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
void handleChoice()
{
    const char *options[] = {
        "Sair.",
        "Cadastrar novo livro.",
        "Remover livro.",
        "Imprimir dados do livro.",
        "Listar todos os livros.",
        "Imprimir arvore por niveis.",
        "Imprimir lista de livres.",
        "Calcular total de livros.",
        "Realizar operacoes em lote."};

    int numOptions = sizeof(options) / sizeof(options[0]);
    int choice;

    do
    {
        displayMenu("MENU", options, numOptions, 56);
        choice = getMenuChoice();

        if (choice == -1)
        {
            printf("Opcao invalida! Tente novamente.\n");
            continue;
        }

        switch (choice)
        {
        case 0:
            printf("Saindo...\n");
            break;
        case 1:
            // chamar função de cadastro unitario de livro
            break;
        case 2:
            // chamar função de remoção de livro
            break;
        case 3:
            // chamar função de impressão de dados de livro
            break;
        case 4:
            // chamar função de listagem de todos os livros
            break;
        case 5:
            // chamar função de impressão de arvore por niveis
            break;
        case 6:
            handleSubMenuFreeList();
            break;
        case 7:
            handleSubMenuQuantities();
            break;
        case 8:
            // chamar função de operações em lote (carregar arquivo txt)
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
        }
    } while (choice != 0);
}