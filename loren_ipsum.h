/* loren_ipsum.h
 */

#ifndef LOREN_IPSUM_H
#define LOREN_IPSUM_H

#define LOR_MEMORY_ACCESS_ERROR 0
#define LOR_THREADSAFE 1
#define LOR_RECURSE    2
#define LOR_REL_PTR    4
#define LOR_ABS_PTR    8
#define LOR_RESERVED   16
#define LOR_PADDING    32 /* ' ' */
#define LOR_CSTRING    64 /* '@' */

/* There are 8 special header values after stripping away all the
 * header continue bits and the allocation size bit.
 * For some of the 8 cases, the remaining header bits have case
 * specific meaing--note that the # of remaining bits is still set
 * by the header continue bits.
 */

typedef int * ipsum; /* integer type to ensure heap alignment */
typedef char ipstat;

ipsum lor_init (ipsum *, const char *);

/*  Dynamic Loren Ipsum String Initialization - Note this is the only
 *    library function which might return NULL.
 *  ipsum lor_init (1, 2, 3) ---------------------------------------+
 *    1. pointer to ipsum variable to be initialized.(pass it by ref)
 *    2. Bitwise ORed special headers or a C "@string"              |
 *  return: pointer to ipsum string, or NULL if malloc() fails ----*/

ipsum lor_strcat (ipsum *, void *);

inline int lor_parse_header (const void*, unsigned int*, unsigned int*);

/*  int lor_parse_header (1, 2, 3) ----------------------------------+
 *    1. pointer to the header to parse                              |
 *    2. integer to be filled with allocation size                   |
 *    3. integer to be filled with "lor length"                      |
 *  return: size of the header, or 0 if header had invalid format --*/



#endif
