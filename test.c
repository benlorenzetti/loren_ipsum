#include <stdio.h>
#include <stdlib.h>
#include "loren_ipsum.h"
#include <string.h>

int main () {

  // Test dynamic initalization
  ipsum str1;
  str1 = lor_init (&str1, "@123");

  // Test static vs dynamic type checking
  ipsum str2;
  str2 = malloc (1000);
  ipstat str3[1000] = {0};
  lor_strcat (&str2, str3);
//  lor_strcat (str2);

  // Test header parser
  ipsum str4;
  lor_init (&str4, "@1234");
  *str4 = (128 + sizeof (int) + 256);
  int allocation_size, string_length;
  switch (lor_parse_header (str4, &allocation_size, &string_length))
  {
    case LOR_CHAR_HDR:
      printf ("char, %d, %d\n", allocation_size, string_length);
    break;
    case LOR_SHORT_HDR:
      printf ("short, %d, %d\n", allocation_size, string_length);
    break;
    case LOR_INT_HDR:
      printf ("int, %d, %d\n", allocation_size, string_length);
    break;
    case LOR_ZERO_HDR:
      printf ("zero error\n");
    break;
    default:
      printf ("default.\n");
  }

}
