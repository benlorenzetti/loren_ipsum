/* loren_ipsum.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "loren_ipsum.h"

ipsum lor_init (ipsum *target, int flags, int alloc_size)
{
  int ipsize, hdr_length, str_length;

  /* Count the Number of Special Headers */
  hdr_length = 1;
  int i;
  for (i=0; i<8; i++)
  {
    if (flags & (1 << i))
      hdr_length += sizeof (int);   
  }

  /* Test for Invalid Combinations */
  if (alloc_size > LOR_MAX_ALLOC_SIZE)
  {
    fprintf (stderr, "Error: alloc_size exceeds LOR_MAX_ALLOC_SIZE in ");
    fprintf (stderr, "lor_init() parameter.\n");
    exit (EXIT_FAILURE);
  }

  if ( (LOR_RECURSE | LOR_ABS_PTR) == flags & (LOR_RECURSE | LOR_ABS_PTR))
  {
    fprintf (stderr, "Error: LOR_RECURSE and LOR_ABS_PTR are mutually");
    fprintf (stderr, " exclusive options for lor_init()\n");
    exit (EXIT_FAILURE);
  }

  if (LOR_PADDING & flags)
  {
    fprintf (stderr, "Error: LOR_PADDING not valid option for lor_init()\n");
    exit (EXIT_FAILURE);
  }

  if (LOR_CSTRING & flags)
  {
    fprintf (stderr, "Error: LOR_CSTRING not valid option for lor_init()\n");
    exit (EXIT_FAILURE);
  }

  /* Determine Binary Allocation Size */
  ipsize = hdr_length;
  alloc_size = (alloc_size > ipsize) ? alloc_size : ipsize;
  alloc_size = (alloc_size > sizeof (int)) ? alloc_size : sizeof (int);
  int log;
  log = 0;
  while ( (alloc_size-1) / (1 << ++log) > 0);
  alloc_size = 1 << log;
  
  /* Allocate Memory */
  *target = malloc (alloc_size);
  if (*target == NULL)
    return NULL;
  char *ctarget;
  ctarget = (char*) *target;

  /* Generate Special Headers */
  hdr_length = 0;

  if (LOR_CONCURRENT & flags)
  {
    printf ("lor_init() concurrency not yet supported.\n");
    hdr_length += sizeof (int);
  }

  if (LOR_RECURSE & flags)
  {
    printf ("lor_init() recursive strings not yet supported.\n");
    hdr_length += sizeof (int);
  }

  if (LOR_ABS_PTR & flags)
  {
    printf ("lor_init() absolute pointers not yet implemented.\n");
    hdr_length += sizeof (int);
  }

  /* Should there be a Standard Header*/
  if (LOR_ABS_PTR & flags)
    return *target;

  /* Generate a Standard Header and Return */
  lor_write_std_header (ctarget + hdr_length, alloc_size, ipsize, 1);
  return *target;  
}

extern inline void lor_write_std_header (void *dest,
  int alloc_size,
  int ipsize,
  int hdr_size)
{
  char* cdest;
  cdest = (char*) dest;

  int alloc_size_flag;
  if (alloc_size == ipsize)
    alloc_size_flag = (1 << (7*hdr_size-1));
  else
    alloc_size_flag = alloc_size;

  int header;
  header = ipsize + alloc_size_flag;

  int i;
  for (i = 0; i < hdr_size; i++)
  {
    cdest[i] = 128 + header % 128;
    header = header / 128;
  }
  cdest[i] -= 128; 
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


