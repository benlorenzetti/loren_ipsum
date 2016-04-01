/* loren_ipsum.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loren_ipsum.h"

extern inline int lor_parse_header (const void *hdr,
  unsigned int *alloc_size,
  unsigned int *lor_length)
{
  int hdr_copy, continue_flag;

  /* Skip any padding, then make a copy of header for manipulation */
  while (*(char*)hdr == ' ')
    hdr++;
  hdr_copy = *(int*)hdr;

  /* Test if the first byte has header continue flag */
  continue_flag = 1 << (sizeof (char) * 8 -1);
  if (0 == (continue_flag & hdr_copy))
  {
    /* The header is 1 byte long */
    *alloc_size = continue_flag >> 1;
    if (0 != (*alloc_size & hdr_copy))
    {
      *lor_length = ((char) hdr_copy) - *alloc_size;
      *alloc_size = *lor_length;
    }
    else 
    {
      while (0 == (*alloc_size & hdr_copy))
        *alloc_size = *alloc_size >> 1;
      *lor_length = ((char) hdr_copy) - *alloc_size;
    }
    return sizeof (char);
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
      *alloc_size = continue_flag >> 1;
      if (0 != (*alloc_size & hdr_copy))
      {
        *lor_length = ((short) hdr_copy) - *alloc_size;
        *alloc_size = *lor_length;
      }
      else
      {
        while (0 == (*alloc_size & hdr_copy))
          *alloc_size = *alloc_size >> 1;
        *lor_length = ((short)hdr_copy) - *alloc_size;
      }
      return sizeof (short);
    }
    else
    {
      *(short*) &hdr_copy = (short) hdr_copy << 1; /*rotate out old cont flag*/

      /* Test if the first int has header continue flag */
      continue_flag = 1 << (sizeof (int) * 8 - 1);
      if (0 == (continue_flag & hdr_copy))
      {
        /* The header has length of int */
        *(short*) &hdr_copy = (short) hdr_copy >> 2; /* restore length x 2 */
        /* Test if the allocation size equals the str length */
        *alloc_size = continue_flag >> 1;
        if (0 != (*alloc_size & hdr_copy))
        {
          /* Allocation size = str len; find str len */
          *lor_length = ((int) hdr_copy) - *alloc_size;
          *alloc_size = *lor_length;
        }
        else
        {
          /* Determine the allocation size and str len separately */
          while (0 == (*alloc_size & hdr_copy))
            *alloc_size = *alloc_size >> 1;
          *lor_length = ((int)hdr_copy) - *alloc_size;
        }
        return sizeof (int);
      }
      else
      {
        /* Headers longer than integer not yet implemented. */
        *alloc_size = 0;
        *lor_length = 0;
        return 0;
      }
    }
  }
}


ipsum lor_init (ipsum *target, const char *init_parameters)
{

  size_t str_length, header_length, alloc_size;
  char * ctarget;

  /* The initialization parameter is usually a loren_ipsum string that should
   * be copied into the new dynamic ipsum string.
   * There are 7 special exceptions, each with an action:
   * (1) Zero - generate empty string
   * (2) LOR_THREADSAFE - generate threadsafe header + empty string
   * (3) LOR_RECURSE - generate recursion header + empty set
   * (4) LOR_REL_PTR - error
   * (5) LOR_ABS_PTR - error
   * (6) LOR_PADDING - error
   * (7) LOR_CSTRING - generate loren ipsum string from C string
   */ 

  switch (init_parameters[0])
  {
    case LOR_MEMORY_ACCESS_ERROR: 
      /* Allocate Integer Size of Memory */
      *target = malloc (sizeof (*target));
      if (*target == NULL)
      {
        *target = NULL; /* this is the only function in loren_ipsum which */
        return (*target); /* can return NULL. */
      }
      /* Fill header with lor_length = 1 and allocation size of integer */
      **target = 1 + sizeof (*target);
      /* Return pointer to new loren_ipsum string */
      return (*target);

    case LOR_THREADSAFE:
    break;
    case LOR_RECURSE:
    break;
    case LOR_REL_PTR:
    case LOR_ABS_PTR:      
    case LOR_PADDING:
      fprintf (stderr, "lor_init() bad initialization %s\n", init_parameters);
      exit (EXIT_FAILURE);
    break;

    case LOR_CSTRING:
      /* Convert a C-string parameter into Loren Ipsum string */

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
        *target = NULL; /* this is the only function in loren_ipsum which */
        return (*target); /* can return NULL. */
      }
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
     
    break;

    default: /* Default case = the init_parameter is a loren_ipsum string */
      printf ("lor_init() default not yet implemented.\n");
      return NULL;
  }
}

ipsum lor_strcat (ipsum *dest, void *src) {
  return *dest;
}
