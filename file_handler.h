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

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/**
 * @brief Tamanho máximo da senha
 * 
 */
#define MAX_PASSWORD_LEN    256

/**
 * @brief Procura por uma senha no dicionário especificado
 * 
 * @param dict  Arquivo de dicionário
 * @param password_md5 MD5 hash da senha a ser procurada
 * @return int 
 */
extern int crack_password(const char *dict, unsigned char *password_md5);

#endif
