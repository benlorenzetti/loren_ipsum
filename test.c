#include <stdio.h>
#include <stdlib.h>
#include "loren_ipsum.h"
#include <string.h>

int main () {

  // Test dynamic initalization
  ipsum str3;
  str3 = lor_init (str3, "@123");

  // Test static vs dynamic type checking
  ipsum str1;
  str1 = malloc (1000);
  ipstat str2[1000];
  lor_strcat (str1);
//  lor_strcat (str2);

}
