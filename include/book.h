#ifndef BOOK_H
#define BOOK_H

typedef struct Book {
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