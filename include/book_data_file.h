#ifndef BOOK_DATA_FILE_H
#define BOOK_DATA_FILE_H

// Cabeçalho do Arquivo de Dados.
typedef struct {
    int firstEmptyPosition; // Endereço (deslocamento/offset) da primeira posição livre do arquivo de dados.
    int headEmptyPosition;  // Endereço (deslocamento/offset) da cabeça dos registros de dados livres.
} BookDataFileHeader;

// Lista de Registros de Dados Livres
typedef struct DataFreeNode
{
    int offset;       // Endereço do registro livre no arquivo de dados.
    int nextOffset;   // Offset para o próximo nó na lista de registros livres (ou  -1 se não houver próximo).
} BookDataFreeNode;

#endif /* BOOK_DATA_FILE_H */