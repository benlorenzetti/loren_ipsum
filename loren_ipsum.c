/* loren_ipsum.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loren_ipsum.h"

ipsum lor_init (ipsum target, const char *init_parameters) {

  if (*init_parameters == LOR_THREADSAFE) {
    printf ("lor_init() threadsafety not implemented.\n");
    exit (EXIT_FAILURE);
  }

  /* Convert a C-string parameter into Loren Ipsum string */
  if (*init_parameters == LOR_C_STRING) {
    size_t str_length, header_length, alloc_size;
    char * ctarget;

    /* First, determine the string length after the '@' */
    str_length = strlen (init_parameters) - 1;
printf ("str_length = %d\n", (int) str_length);

    /* Second, determine the required header size */
    header_length = 1;
    size_t mod;
    mod = 64; /* maximum size for single byte header */
    while ( (str_length + header_length) / mod > 0) {
      header_length *= 2;
      mod *= 128;
    }
printf ("header_length = %d\n", (int) header_length);

    /* Third, determine the smallest power of 2 allocation size */
    alloc_size = sizeof (*target);
      /* (no point mallocing less than the heap alignment size) */
    while ( (str_length + header_length) > (alloc_size - 1))
      /* (n - 1 is the largest length that can be specified) */
      alloc_size *= 2;
printf ("alloc_size = %d\n", (int) alloc_size);
    
    /* Allocate Memory */
    target = malloc (alloc_size / sizeof (*target));
    if (target == NULL) {
      target = malloc (sizeof (*target));
      if (target != NULL) {
        *target = LOR_MEMORY_ERROR;
        return target;
      }
      else
        return NULL;
    }
    else {
      ctarget = (char *) target;
    }

    /* Create the Loren Ipsum header */
    str_length = str_length + header_length;
    while ( (str_length / 64) > 0) {
      /* (only a size of up to 63 can fit in the uppermost header byte) */
      (*ctarget) = 128 + (str_length % 128);
      ctarget++;
      str_length = str_length / 128;
    }
    (*ctarget) = str_length + alloc_size;
    ctarget++;
printf ("Header: %d %d\n", *target %256, *target / 256);

    /* Copy the C string and return */
    strcpy (ctarget, (init_parameters + 1)); /* +1 over the '@' */
    return target;
  }
}

ipsum lor_strcat (ipsum dest) {
  return dest;
}
