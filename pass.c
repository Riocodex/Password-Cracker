#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>

//compile with:cc -o pass pass.c -lcrypt
//run with:./pass AB22

#define SALT "$6$AS$"

int main(int argc, char *argv[]){
  printf("%s\n", crypt(argv[1], SALT));
  FILE *fp;
  fp = fopen("output.txt", "w"); // opens a file named "output.txt" in write mode
  if (fp == NULL) {
    printf("Error opening file!\n");
    return 1;
    }
  fprintf(fp, "%s", crypt(argv[1], SALT)); // writes the output of printf to the file
  fclose(fp); // closes the file

  return 0;
}
