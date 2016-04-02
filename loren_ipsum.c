/* loren_ipsum.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loren_ipsum.h"

ipsum lor_init (ipsum *target, const char *init_param, int alloc_size)
{
  /* The initialization parameter is usually a loren_ipsum string that should
   * be copied into the new dynamic ipsum string.
   * There are 8 special exceptions, each with an action:
   * (1) Zero - generate empty string
   * (2) LOR_RECURRENT - generate threadsafe header + empty string
   * (3) LOR_RECURSE - generate recursion header + empty set
   * (4) LOR_REL_PTR - error
   * (5) LOR_ABS_PTR - error
   * (6) LOR_PADDING - error
   * (7) LOR_CSTRING - generate loren ipsum string from C string
   */ 

  char header[LOR_MAX_HEADER_LENGTH];
  int ipsize, hdr_length, str_length, alloc_size;
  ipsize = header_length = alloc_size = 0;

  if (LOR_CONCURRENT & init_param[0])
  {
    printf ("lor_init() concurrency not yet supported.\n");
  }

  if ((LOR_RECURSE & init_param[0]) && (LOR_ABS_PTR & init_param[0]))
  {
    fprintf (stderr, "Error: LOR_RECURSE and LOR_ABS_PTR are mutually");
    fprintf (stderr, " exclusive options for lor_init()\n");
    exit (EXIT_FAILURE);
  }

  if (LOR_RECURSE & init_parameters[0])
  {
    printf ("lor_init() recursive strings not yet supported.\n");
  }

  if (LOR_ABS_PTR & init_parameters[0])
  {
    printf ("lor_init() absolute pointers not yet implemented.\n");
  }

  if (LOR_PADDING & init_parameters[0])
  {
    fprintf (stderr, "Error: LOR_PADDING not valid option for lor_init()\n");
    exit (EXIT_FAILURE);
  }

  if ((LOR_CSTRING & init_parameters[0]) & (~LOR_CSTRING & init_parameters[0]))
  {
    fprintf (stderr, "Error: LOR_CSTRING not compatible with any other ");
    fprintf (stderr, "options for lor_init().\n");
    exit (EXIT_FAILURE);
  }

  if (LOR_CSTRING == init_parameters[0])
  {
    printf ("lor_init() c strings not yet implemented.\n");
  }
}


extern inline int lor_parse_std_header (const void *hdr,
  int *alloc_size,
  int *lor_length)
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


