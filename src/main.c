/**
 * @file main.c
 * @author Gabriel Hochmann
 * 
 * @brief Contém a função principal do programa, que gerencia a inicialização e execução do sistema.
 * 
 * A função `main` é responsável por abrir os arquivos binários de dados e índices, invocar o menu de opções para o usuário e, por fim, fechar os arquivos antes de encerrar o programa.
 * 
 * @see menu.h
 * @see file_manager.h
 */

#include "menu.h"
#include "file_manager.h"

int main()
{
    // Abre o arquivo de dados em modo de leitura e escrita binária
    FILE *dataFile = openFile("books.bin", "w+b");

    // Abre o arquivo de índices em modo de leitura e escrita binária
    FILE *indexFile = openFile("TwoThreeTree.bin", "w+b");

    // Exibe o menu de opções para o usuário
    handleChoice();

    // Fecha os arquivos de dados e índices
    closeFile(&dataFile);
    closeFile(&indexFile);

    return 0;
}
