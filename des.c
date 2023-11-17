#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include<omp.h>

//Compile with: cc -o des des.c -lcrypt
//run with: ./des
//AB22 plain password
//Encrypted password:$6$AS$0sfS3wn3rkVwEpt8D0X3yxgVMynBDHdkcMh7XDih7vjBqdKDejqyZWY7ySEVR9Pzl1pHNWnAUDEflXgGFs8UH/
//$6$AS$0sfS3wn3rkVwEpt8D0X3yxgVMynBDHdkcMh7XDih7vjBqdKDejqyZWY7ySEVR9Pzl1pHNWnAUDEflXgGFs8UH/
//$6$AS$0sfS3wn3rkVwEpt8D0X3yxgVMynBDHdkcMh7XDih7vjBqdKDejqyZWY7ySEVR9Pzl1pHNWnAUDEflXgGFs8UH/

int count=0;

void substr(char *dest, char *src, int start, int length)
{
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void crack(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
  char plain[5];   // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
  char *enc;       // Pointer to the encrypted password
  //int found = 0;
  
//67581
  substr(salt, salt_and_encrypted, 0, 6);
volatile int found = 0;

  int thread_count = omp_get_num_procs();
omp_lock_t lock;
  omp_init_lock(&lock);
#pragma omp parallel for num_threads(thread_count) schedule(dynamic) private(x, y, z, plain, enc) shared(count, found)
  
  for(x='A'; x<='Z'; x++)
  {
    if(found == 0)
    {
      for(y='A'; y<='Z'; y++)
      {
        if(found == 0)
        {
          for(z=0; z<=99; z++)
          {
            if(found == 0)
            {
              sprintf(plain, "%c%c%02d", x, y, z);
              enc = (char *) crypt(plain, salt);
              count++; 
                omp_set_lock(&lock);
              #pragma omp critical
              {
                if(strcmp(enc, salt_and_encrypted) == 0)
                {
                  printf("jsdnfjsdfnsdjfn\n");
                
                  if(found == 0)
                  {
                    printf("#%-8d%s %s\n", count, plain, enc);
                    found = 1;
                  }
                  //break;
                }
              } 
              omp_unset_lock(&lock);
            }
          }
        }
      }
    }
  }
}



int main(int argc, char *argv[])
{
    FILE *file;
    char c;
    char *data;
    int i = 0;
    int size = 0;

    file = fopen("output.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    while ((c = fgetc(file)) != EOF) {
        size++;
    }
    data = (char *) malloc((size + 1) * sizeof(char));
    rewind(file);
    while ((c = fgetc(file)) != EOF)
    {
      data[i] = c;
      i++;
    }

    fclose(file);
    data[i] = '\0';
  crack(data);		//Copy and Paste your ecrypted password here using EncryptShA512 program
  printf("%d solutions explored\n", count);

  return 0;
}

