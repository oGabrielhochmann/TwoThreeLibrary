/**
 * @file main.c
 * @author Gabriel Hochmann
 * 
 * @brief Contém a função principal do programa.
 */

#include "menu.h"
#include "file_manager.h"

int main()
{
    FILE *dataFile = openFile("books.bin", "w+b");
    FILE *indexFile = openFile("TwoThreeTree.bin", "w+b");

    handleChoice();

    closeFile(&dataFile);
    closeFile(&indexFile);

    return 0;
}