/* loren_ipsum.h
 */

#ifndef LOREN_IPSUM_H
#define LOREN_IPSUM_H

#define LOR_MEMORY_ACCESS_ERROR 0
#define LOR_THREADSAFE 1
#define LOR_C_STRING 64 /* '@' */

typedef int * ipsum; /* integer type to ensure heap alignment */
typedef char ipstat;

/*  Dynamic Loren Ipsum String Initialization - Note this is the only
 *    library function which might return NULL.
 *  ipsum lor_init (1, 2) ------------------------------------------+
 *    1. pointer to ipsum variable to be initialized.(pass it by ref)
 *    2. const C string to be copied into ipsum string 1            | 
 *  return: pointer to ipsum string, or NULL if malloc() fails ----*/

ipsum lor_init (ipsum *, const char *);

/*  Static Loren Ipsum String Inialization
 *  void lor_sinit (1, 2) ------------------------------------------+
 *    1. ipstat array with an '@' header that needs initialization  |
 *    2. array size, or 0 if array size is the C string length + 1  |
 *  return: guaranteed to succeed, or will exit() on failure ------*/

ipsum lor_strcat (ipsum *, void *);

/*  lor_parse_header (1, 2, 3) --------------------------------------+
 *    1. pointer to the header to parse                              |
 *    2. integer to be filled with allocation size                   |
 *    3. integer to be filled with string length                     |
 *  return: size of the header, or 0 if header had invalid format --*/

int lor_parse_header (const void*, unsigned int*, unsigned int*);

#endif
