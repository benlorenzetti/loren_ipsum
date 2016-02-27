/* loren_ipsum.h
 */

#ifndef LOREN_IPSUM_H
#define LOREN_IPSUM_H

#define LOR_MEMORY_ACCESS_ERROR 0
#define LOR_THREADSAFE 1
#define LOR_C_STRING 64 /* '@' */

typedef int * ipsum;
typedef char ipstat;

ipsum lor_init (ipsum *, const char *);
ipsum lor_strcat (ipsum *, void *);

/*  param1: pointer to the header to parse
 *  param3: integer to be filled with allocation size
 *  param2: integer to be filled with string length
 *  return: size of the header or 0 if header had invalid format
 */
int lor_parse_header (const void*, unsigned int*, unsigned int*);

#endif
