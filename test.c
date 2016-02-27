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
  *str4 = (128 + 7 + 16384);
  int header_size, allocation_size, string_length;
  header_size = lor_parse_header (str4, &allocation_size, &string_length);
  printf ("Sizes: (header) %d, (allocation) %d, (str length) %d\n", header_size, allocation_size, string_length);

}
