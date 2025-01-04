#ifndef DATA_FREE_LIST_H
#define DATA_FREE_LIST_H

// Lista de Registros de Dados Livres
typedef struct DataFreeNode
{
    int offset;                 // Endereço do registro livre no arquivo de dados.
    struct DataFreeNode *next; //  Ponteiro para o próximo nó na lista.
};

#endif /* DATA_FREE_LIST_H */