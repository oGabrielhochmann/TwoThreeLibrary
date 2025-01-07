/**
 * @file book.h
 *
 * @author Gabriel Hochmann
 *
 * @brief Contém a estrutura de dados para um livro.
 *
 * @see book_manager.h
 */

#ifndef BOOK_H
#define BOOK_H

/**
 * @brief Estrutura de dados para um livro.
 *
 * Contém a estrutura de dados para um livro, com os seguintes campos:
 * - code: Código do livro, pode ser aproveitado para o encadeamento de registros livres.
 * - title: Título do livro.
 * - author: Autor do livro.
 * - publisher: Editora do livro.
 * - edition: Edição do livro.
 * - year: Ano do livro.
 * - price: Preço do livro.
 * - stock_quantity: Quantidade em estoque do livro.
 *
 * @see book_manager.h
 */
typedef struct Book
{
    int code; // Código do livro, pode ser aproveitado para o encadeamento de registros livres.
    char title[151];
    char author[201];
    char publisher[51];
    int edition;
    int year;
    double price;
    int stock_quantity;
} Book;

#endif /* BOOK_H*/