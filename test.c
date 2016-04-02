#include <stdio.h>
#include <stdlib.h>
#include "loren_ipsum.h"
#include <string.h>

int main () {

  // Test binary_header_helper Function
  char str1[] = {64, 'a', 'b', 'c'};
  char str2[] = {1, 0, 0, 0, 4+1, 0, 0, 0};
  char binary_header[100];
  printf ("Testing binary_helper_function()\n");
  printf ("str1 (an '@' header): %s\n", binary_header_helper (binary_header, str1));
  printf ("str2 (a concurrent header): %s\n", binary_header_helper (binary_header, str2)); 

  // Test Dynamic Initalization
//  ipsum str1, str2, str3, str4, str5, str6;

//  lor_init (&str1, LOR_CONCURRENT, 0);


/*
  // Test static vs dynamic type checking
  ipsum str2;
  str2 = malloc (1000);
  ipstat str3[1000] = {0};
  lor_strcat (&str2, str3);
//  lor_strcat (str2);

  // Test Standard Header Parser
  ipsum str4;
  lor_init (&str4, "@1234");
  *str4 = (128 + 7 + 256);
  int header_size, alloc_size, lor_length;
  header_size = lor_parse_header (str4, &alloc_size, &lor_length);
  printf ("Sizes: (header) %d, (allocation) %d, ", header_size, alloc_size);
  printf ("(\"lor length\") %d\n", lor_length);
*/
}
