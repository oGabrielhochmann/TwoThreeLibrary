/**
 * @file book.h
 *
 * @brief Define a estrutura de dados para representar um livro.
 *
 * Este arquivo contém a definição da estrutura de dados usada para armazenar
 * informações sobre um livro no sistema, incluindo seus atributos principais.
 *
 * @author Gabriel Hochmann
 * @see book_manager.h
 */

#ifndef BOOK_H
#define BOOK_H

/**
 * @brief Estrutura de dados para representar um livro.
 *
 * Esta estrutura encapsula as principais informações de um livro, incluindo:
 * 
 * @param code Código único do livro, também é utilizado para o encadeamento de registros livres.
 * @param title Título do livro (até 150 caracteres).
 * @param author Nome do autor do livro (até 200 caracteres).
 * @param publisher Nome da editora do livro (até 50 caracteres).
 * @param edition Edição do livro (inteiro).
 * @param year Ano de publicação do livro (inteiro).
 * @param price Preço do livro (valor decimal).
 * @param stock_quantity Quantidade de exemplares em estoque (inteiro).
 *
 * @see book_manager.h
 */
typedef struct Book
{
    int code;           /**< Código único do livro. */
    char title[151];    /**< Título do livro. */
    char author[201];   /**< Nome do autor do livro. */
    char publisher[51]; /**< Nome da editora do livro. */
    int edition;        /**< Edição do livro. */
    int year;           /**< Ano de publicação do livro. */
    double price;       /**< Preço do livro. */
    int stock_quantity; /**< Quantidade de exemplares em estoque. */
} Book;

#endif /* BOOK_H */
