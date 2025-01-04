#ifndef DATA_FILE_HEADER_H
#define DATA_FILE_HEADER_H

// Cabeçalho do Arquivo de Dados.
typedef struct {
    int firstEmptyPosition; // Endereço (deslocamento/offset) da primeira posição livre do arquivo de dados.
    int headEmptyPosition; // Endereço (deslocamento/offset) da cabeça dos registros de dados livres.
} dataFileHeader;

#endif /* DATA_FILE_HEADER_H */