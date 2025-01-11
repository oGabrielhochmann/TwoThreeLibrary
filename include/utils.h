/**
 * @file utils.h
 *
 * @brief Definições de funções e utilitários genéricos utilizados no sistema.
 *
 * Este arquivo contém declarações de funções auxiliares e macros que podem
 * ser reutilizadas em diferentes partes do sistema.
 *
 * @author Gabriel Hochmann
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * @file utils.c
 * @brief Implementação de funções utilitárias para manipulação de strings, arquivos e outros recursos gerais usados no projeto.
 *
 * Este arquivo contém funções auxiliares que desempenham tarefas comuns, como manipulação de strings, validação de arquivos,
 * ajustes de formatação e outras operações que podem ser reutilizadas em diferentes partes do projeto.
 *
 * @author Gabriel Hochmann
 */

#include "utils.h"

/**
 * @brief Remove espaços extras de uma string e normaliza os espaços internos.
 *
 * @details
 * Esta função realiza três operações principais em uma string:
 * 1. Remove os espaços em branco no início (leading spaces) e no final (trailing spaces).
 * 2. Substitui múltiplos espaços consecutivos no meio da string por um único espaço.
 * 3. Move o resultado processado para o início do buffer original e adiciona um caractere nulo ao final.
 *
 * @param[in,out] str Ponteiro para a string a ser normalizada. A string é modificada no local.
 *
 * @pre
 * - O ponteiro `str` deve ser válido.
 * - A string apontada por `str` deve ser terminada por um caractere nulo (`\0`).
 *
 * @post
 * - A string apontada por `str` terá os espaços do início e do final removidos.
 * - Múltiplos espaços consecutivos no meio da string serão substituídos por um único espaço.
 *
 * @example
 * char texto[] = "   Olá,   mundo!   ";
 * trimWhitespace(texto);
 * // Após a execução, texto conterá: "Olá, mundo!"
 *
 * @return Retorna o ponteiro para a string normalizada.
 */
char *trimWhitespace(char *str);

/**
 * @brief Compara duas strings ignorando diferenças entre maiúsculas e minúsculas.
 *
 * @details Esta função realiza uma comparação entre duas strings, caractere por caractere,
 *          de forma case-insensitive, ou seja, desconsiderando a distinção entre letras maiúsculas e minúsculas.
 *          A comparação para ao encontrar caracteres diferentes ou quando ambas as strings chegam ao final.
 *
 * @pre Os ponteiros `str1` e `str2` devem ser válidos e não devem ser `NULL`.
 *      As strings devem ser terminadas por caractere nulo (`\0`).
 * @post Nenhuma modificação é feita nas strings fornecidas. O resultado da comparação é retornado.
 *
 * @param str1 Ponteiro para a primeira string a ser comparada.
 * @param str2 Ponteiro para a segunda string a ser comparada.
 *
 * @return Retorna `1` se as strings forem iguais, ignorando diferenças de caso.
 *         Retorna `0` caso as strings sejam diferentes.
 *
 * @warning Certifique-se de que ambas as strings sejam válidas e acessíveis. O comportamento é indefinido
 *          se qualquer uma das strings não for terminada por caractere nulo.
 */
int compareCaseInsensitive(const char *str1, const char *str2);

/**
 * @brief Converte separadores decimais de vírgula para ponto em uma string.
 *
 * @details Substitui todas as ocorrências do caractere vírgula (`,`) por ponto (`.`) em uma string.
 * Isso é útil para adaptar números decimais em formatos regionais que utilizam vírgula como separador decimal
 * para o formato padrão aceito por funções como `atof` e outras operações numéricas em C.
 *
 * @param str Ponteiro para a string a ser processada. Deve ser um array de caracteres nulo-terminado,
 *            e será modificada diretamente pela função.
 *
 * @pre A string apontada por `str` deve ser válida, alocada, e terminada por caractere nulo (`\0`).
 *      O ponteiro `str` não deve ser `NULL`.
 *
 * @post Todas as vírgulas na string original serão substituídas por pontos, permitindo seu uso em funções
 *       que esperam o ponto como separador decimal.
 *
 * @warning A string passada será modificada diretamente. Garanta que essa modificação seja segura no contexto de uso.
 */
void formatDecimalSeparator(char *str);

#endif /* UTILS_H */
