#include <stdio.h>
#include <stdlib.h>

typedef int * ipsum;
typedef char * ipstat;

void lor_strcpy (ipsum dest) {
  
}

int main () {
  ipsum str1;
  str1 = malloc (1000);
  ipstat str2[1000];
 
  lor_strcpy (str1);
  lor_strcpy (str2);
}
