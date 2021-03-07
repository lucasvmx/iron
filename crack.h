/**
 * @file file_handler.h
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-03-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef IO_HEADER
#define IO_HEADER

#include "main.h"


/**
 * @brief Maximum password size
 * 
 */
#define MAX_PASSWORD_LEN    256

/**
 * @brief 
 * 
 * @param dict 
 * @param password_md5 
 * @param id 
 * @return int 
 */
extern int crack_password(const char *dict, unsigned char *password_md5, enum algorithm_ids id);

/**
 * @brief Obtém o tamanho total de um arquivo
 * 
 * @param fp Ponteiro para o arquivo
 * @return long Tamanho total do arquivo
 */
//extern long get_filesize(FILE **fp);

#endif
