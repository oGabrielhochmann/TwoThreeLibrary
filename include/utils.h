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
 * @brief Remove espaços em branco no início e no final de uma string.
 *
 * Esta função modifica a string fornecida para remover espaços em branco
 * no início e no final.
 *
 * @param str A string a ser modificada.
 * @return Um ponteiro para a string modificada.
 */
char *trimWhitespace(char *str);

#endif /* UTILS_H */
