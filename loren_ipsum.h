/* loren_ipsum.h
 */

#ifndef LOREN_IPSUM_H
#define LOREN_IPSUM_H

#define LOR_MAX_STRING_LENGTH 67108864

#define LOR_NULL       0
#define LOR_CONCURRENT 1
#define LOR_RECURSE    2
#define LOR_ABS_PTR    4
#define LOR_RESERVE    8
#define LOR_RESERVE    16
#define LOR_PADDING    32 /* ' ' */
#define LOR_CSTRING    64 /* '@' */

/*   There are 8 special header values after stripping away all the
 * header continue bits and the allocation size bit.
 * For some of the 8 cases, the remaining header bits have case
 * specific meaing--note that the # of remaining bits is still set
 * by the header continue bits.
 */

#define LOR_MAX_HEADER_LENGTH 8*sizeof(int)

/*   There are only X valid combinations of special headers + 
 * standard header.
 */

typedef int * ipsum;
typedef char ipstat;

/*   Ipsum and ipstat have different types to allow compile-time
 * checking of the use of dynamic/static functions (lor/lor_s).
 * Ipsum types should be declared and passed by reference to
 * the lor_init() function for allocation and ipstat types are
 * declared like char arrays.
 *   Ipsum types are integer size to ensure alignment in the heap,
 * which is necessary to ensure atomic access to concurrency headers
 * on many platforms.
 */

ipsum lor_init (ipsum *, const char *, int);

/*   A function for allocating initial heap memory for ipsum strings
 * and generating standard and/or special headers. The parameters are:
 *
 *   (1) reference to an ipsum string variable to be initialized
 *   (2) bitwise ORed special header flags or an '@' C string
 *   (3) desired allocation size or 0 for default
 *   (return) new ipsum value or NULL (0) on allocation failure
 * 
 * Examples:
 *   ipsum str1;
 *   if (!lor_init (&str1, {LOR_CONCURRENT | LOR_RECURSE}, 256) )
 *     // error handling
 *
 *   ipsum str2;
 *   if (!lor_init (&str2, "@a C string", 0) )
 *     // error handling
 */

ipsum lor_strscat (ipsum *, const void *, int, int);

ipsum lor_strcat (ipsum *, const void *);



inline int lor_parse_std_header (const void*, int*, int*);

/*  int lor_parse_header (1, 2, 3) ----------------------------------+
 *    1. pointer to the header to parse                              |
 *    2. integer to be filled with allocation size                   |
 *    3. integer to be filled with "lor length"                      |
 *  return: size of the header, or 0 if header had invalid format --*/



#endif
