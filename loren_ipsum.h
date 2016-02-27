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

enum htype
{
  LOR_CHAR_HDR,
  LOR_SHORT_HDR,
  LOR_INT_HDR,
  LOR_ZERO_HDR,
  LOR_THREADSAFE_PREFIX,
  LOR_THREADSAFE_COLLECTED_PREFIX
};
/*  param1: pointer to the header to parse
 *  param3: integer to be filled with allocation size
 *  param2: integer to be filled with string length
 *  return: size of the header or other (see enum list)
 */
enum htype lor_parse_header (const void*, unsigned int*, unsigned int*);

#endif
