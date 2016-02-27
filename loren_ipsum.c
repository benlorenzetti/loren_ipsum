/* loren_ipsum.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loren_ipsum.h"

enum htype lor_parse_header (const void *hdr,
  unsigned int *alloc_size,
  unsigned int *str_length)
{
  int hdr_copy, continue_flag;
  hdr_copy = *(int*)hdr;

  /* Test if the first byte has header continue flag */
  continue_flag = 1 << (sizeof (char) * 8 -1);
  if (0 == (continue_flag & hdr_copy))
  {
    /* The header is 1 byte long */
    *alloc_size = continue_flag;
    while (0 == (*alloc_size & hdr_copy))
      *alloc_size = *alloc_size >> 1;
    *str_length = ((char) hdr_copy) - *alloc_size;
    return LOR_CHAR_HDR;
  }
  else
  {
    /* Rotate out the header continue flag that was just tested */
    *(char*) &hdr_copy = (char) hdr_copy << 1;

    /* Test if the first short has header continue flag */
    continue_flag = 1 << (sizeof (short) * 8 - 1);
    if (0 == (continue_flag & hdr_copy))
    {
      /* The header has length of short */
      *(char*) &hdr_copy = (char) hdr_copy >> 1; /* restore length */
      *alloc_size = continue_flag;
      while (0 == (*alloc_size & hdr_copy))
        *alloc_size = *alloc_size >> 1;
      *str_length = ((short)hdr_copy) - *alloc_size;
      return LOR_SHORT_HDR;
    }
    else
    {
      *(short*) &hdr_copy = (short) hdr_copy << 1; /* rotate out old cont flag */

      /* Test if the first int has header continue flag */
      continue_flag = 1 << (sizeof (int) * 8 - 1);
      if (0 == (continue_flag & hdr_copy))
      {
        /* The header has length of int */
        *(short*) &hdr_copy = (short) hdr_copy >> 2; /* restore length x 2 */
        *alloc_size = continue_flag;
        while (0 == (*alloc_size & hdr_copy))
          *alloc_size = *alloc_size >> 1;
        *str_length = ((int)hdr_copy) - *alloc_size;
        return LOR_INT_HDR;
      }
      else
      {
        /* Headers longer than integer not yet implemented. */
        *alloc_size = 0;
        *str_length = 0;
        return LOR_ZERO_HDR;
      }
    }
  }
}

ipsum lor_init (ipsum *target, const char *init_parameters)
{
  if (init_parameters == NULL) {
    fprintf (stderr, "in %s, lor_init() passed NULL parameter\n", __FILE__);
  }
  /* if init_parameter[0] is 1000 0000,
   *   then create atomic access prefix followed by a minimum length
   *   string header at the next alignment site
   * else if init_parameter[0] is 0000 0010 ('@'),
   *   then convert the following C string to ipsum string
   * otherwise,
   *   assume init_parameter[] is a valid loren ipsum string and copy
   *   it into a new memory location
  */
  if (*init_parameters == LOR_THREADSAFE) {
    printf ("lor_init() threadsafety not implemented.\n");
  }

  /* Convert a C-string parameter into Loren Ipsum string */
  else if (*init_parameters == LOR_C_STRING) {
    size_t str_length, header_length, alloc_size;
    char * ctarget;

    /* First, determine the string length after the '@' */
    str_length = strlen (init_parameters) - 1;

    /* Second, determine the required header size */
    header_length = 1;
    size_t mod;
    mod = 64; /* maximum size for single byte header */
    while ( (str_length + header_length) / mod > 0)
    {
      header_length *= 2;
      mod *= 128;
    }

    /* Third, determine the smallest power of 2 allocation size */
    alloc_size = sizeof (**target);
      /* (no point mallocing less than the heap alignment size) */
    while ( (str_length + header_length) > (alloc_size - 1))
      /* (n - 1 is the largest length that can be specified) */
      alloc_size *= 2;
    
    /* Allocate Memory */
    *target = malloc (alloc_size / sizeof (*target));
    if (*target == NULL)
    {
      *target = malloc (sizeof (*target));
      if (*target != NULL)
      {
        **target = LOR_MEMORY_ACCESS_ERROR;
        return *target;
      }
      else
      {
        *target = NULL; /* this is the only function in loren_ipsum which */
        return (*target); /* can return NULL. */
      }
    }
    else
      ctarget = (char *) *target;

    /* Create the Loren Ipsum header */
    str_length = str_length + header_length;
    while ( (str_length / 64) > 0)
    {
      /* (only a size of up to 63 can fit in the uppermost header byte) */
      (*ctarget) = 128 + (str_length % 128);
      ctarget++;
      str_length = str_length / 128;
    }
    (*ctarget) = str_length + alloc_size;
    ctarget = ctarget + 1;

    /* Copy the C string and return */
    strcpy (ctarget, (init_parameters + 1)); /* +1 over the '@' */
    return *target;
  } /* End of else if (*init_parameters == LOR_C_STRING) */
}

ipsum lor_strcat (ipsum *dest, void *src) {
  return *dest;
}
