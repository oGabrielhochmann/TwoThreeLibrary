/**
 * @file menu.c
 * @author Gabriel Hochmann
 *
 * @brief Contém funções para exibição e manipulação do menu principal.
 *
 * Este arquivo implementa as funções relacionadas à exibição do menu principal e ao tratamento das
 * escolhas feitas pelo usuário. O menu inclui opções para diversas operações como cadastro, remoção,
 * listagem e impressão de livros, além de submenus para manipulação de listas e quantidades. As opções
 * são apresentadas ao usuário de forma interativa, e o programa executa as operações correspondentes
 * com base na escolha do usuário.
 *
 * @see menu.h
 */

#include "menu.h"

#include <string.h>
#include <stdio.h>

/**
 * @brief Exibe uma borda formatada para o menu.
 *
 * Esta função é responsável por imprimir uma borda ao redor do menu na tela. A borda é composta por barras verticais
 * no início e no final, e traços no meio, com a largura especificada.
 *
 * @pre `width` deve ser maior que 2 para garantir que a borda tenha um formato válido.
 *
 * @post A borda é impressa na tela com o tamanho fornecido, delimitando visualmente o menu.
 *
 * @param width Largura total da borda. O valor deve ser maior que 2 para garantir que haja espaço para os caracteres
 *              da borda vertical e horizontal.
 *
 * @note A função assume que a largura fornecida seja suficiente para uma borda válida. Não há verificação de erro
 *       caso `width` seja menor ou igual a 2.
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
 * @brief Exibe um menu com as opções disponíveis e um título.
 *
 * Esta função é responsável por imprimir o menu na tela de forma formatada. O menu inclui um título no topo,
 * uma borda superior e inferior, e as opções numeradas. As opções são exibidas dentro de uma borda com a
 * largura especificada, e cada uma é alinhada de forma apropriada.
 *
 * @pre `title` não deve ser NULL. O título será exibido no topo do menu.
 * @pre `option` não deve ser NULL. O array de opções deve conter as opções a serem exibidas no menu.
 * @pre `numOptions` deve ser maior que 0, indicando o número de opções a serem apresentadas.
 * @pre `width` deve ser maior que 2 para garantir que a borda tenha o formato adequado.
 *
 * @post O menu é impresso na tela com as opções fornecidas e um título no topo. A borda superior e inferior é
 *       impressa e cada opção é numerada.
 *
 * @param title Título do menu, que será exibido no topo.
 * @param option Array de strings contendo as opções do menu.
 * @param numOptions Número total de opções disponíveis no menu.
 * @param width Largura total do menu, incluindo bordas e conteúdo.
 *
 * @note A largura do menu (`width`) deve ser grande o suficiente para acomodar o título e as opções. A função
 *       não realiza validação de erro caso a largura fornecida seja inadequada.
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
 * @brief Lê a escolha do usuário a partir do menu.
 *
 * Esta função solicita que o usuário insira uma opção de menu, aguardando uma entrada do tipo inteiro.
 * Caso a entrada seja inválida (não um número inteiro), a função limpa o buffer de entrada e retorna -1 para
 * indicar que houve um erro. Caso contrário, retorna a opção selecionada.
 *
 * @pre Nenhuma.
 * @post Nenhuma.
 *
 * @return A opção selecionada pelo usuário. Retorna -1 se a entrada for inválida (não um número inteiro).
 *
 * @note Se a entrada for inválida, o buffer de entrada é limpo para evitar que a função falhe em chamadas subsequentes.
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
 * @brief Manipula o submenu de livres relacionado à manipulação da lista de registros livres.
 *
 * Este submenu oferece três opções ao usuário: sair, imprimir registros do arquivo de índices
 * e imprimir registros do arquivo de dados. O submenu continua em execução até que o usuário escolha a opção de sair.
 * Quando o usuário seleciona uma opção válida, a ação correspondente é executada.
 *
 * @pre Nenhuma.
 * @post O submenu é exibido na tela, e as opções selecionadas pelo usuário são tratadas. O submenu é encerrado
 *       quando o usuário escolhe a opção de sair (opção 0).
 *
 * @param Nenhum.
 *
 * @return Nenhum. A função não retorna nada.
 *
 * @note As opções de impressão dos arquivos de índices e dados ainda não estão implementadas e precisam
 *       ser chamadas com funções específicas para essa tarefa.
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
 * @brief Manipula o submenu de quantidades relacionado à manipulação de quantidades de livros.
 *
 * Este submenu oferece ao usuário seis opções: sair, total de livros diferentes,
 * total de livros em estoque, total de livros por autor, total de livros por editora
 * e total de livros por ano de lançamento. O submenu permanece em execução até que o
 * usuário escolha a opção de sair. Quando o usuário escolhe uma opção, a função correspondente
 * será chamada para realizar o cálculo ou operação desejada.
 *
 * @pre Nenhuma.
 * @post O submenu é exibido na tela e as opções selecionadas pelo usuário são tratadas adequadamente.
 *       O submenu é encerrado quando o usuário escolhe a opção de sair (opção 0).
 *
 * @param Nenhum.
 *
 * @return Nenhum. A função não retorna nada.
 *
 * @note As opções de cálculo dos totais de livros, por autor, editora, etc., ainda precisam ser implementadas.
 *       Cada uma delas deve ser associada a uma função específica para realizar os cálculos desejados.
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
 * Esta função exibe o menu principal com várias opções de operações relacionadas ao cadastro,
 * remoção, impressão e listagem de livros, bem como outras funcionalidades, como cálculos e
 * operações em lote. O menu principal permanece em execução até que o usuário escolha a opção de sair.
 * O menu oferece opções para acessar submenus relacionados à manipulação de livros e de listas.
 *
 * @pre Nenhuma.
 * @post O menu principal é exibido na tela e as opções são tratadas de acordo com a escolha do usuário.
 *       O submenu é chamado conforme a seleção do usuário, ou a operação correspondente é executada.
 *
 * @param Nenhum.
 *
 * @return Nenhum. A função não retorna nada.
 *
 * @note As funções correspondentes para cada operação (como cadastro, remoção de livro,
 *       e manipulação de dados) ainda precisam ser implementadas.
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
