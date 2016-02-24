/* loren_ipsum.h
 */

#ifndef LOREN_IPSUM_H
#define LOREN_IPSUM_H

#define LOR_MEMORY_ERROR 0
#define LOR_THREADSAFE 1
#define LOR_C_STRING 64
#define LOR_CONTINUE_HEADER 128

typedef int * ipsum;
typedef char * ipstat;

ipsum lor_init (ipsum, const char *);
ipsum lor_strcat (ipsum);

#endif
