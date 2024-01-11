// Tales from the Crypt

#include <stdio.h>
#include <unistd.h>
#include <crypt.h>
#include <string.h>
#include <stdlib.h>


char* nTest(char* word, char* hash){
  
  char* mangle = malloc(32 * sizeof(char));
  char digit[16];
  int r;
  char* crypted;
  char* salt = "$1$";
  
  for(int i = 0; i < 10; i++){
    strcpy(mangle, word);
    r = sprintf(digit, "%d", i);
    if (r == -1){
      perror("sprintf");
    }
    strcat(mangle, digit);
    //word = mangle;
      
    crypted = crypt(mangle, salt);
    
      if (strcmp(hash, crypted) == 0){
        //printf("Return %s\n", mangle);
        return mangle;
      }
    }
  return "false";
}

char* checkMatch(char* hash, char* dict, int c, int n){
  /*takes the hash string and the dictionary file name as inputs
  open the dictionary file and loop through the words in contains
  calculate its hash using crypt and compare to the input hash string
  If you find a match, return the dictionary word that generated the correct hash
  */

  //printf("Hash to check: %s\n", hash);
  
   // Open the file
  FILE *f = fopen(dict, "r");

  // Check the return code
  if (f == NULL) {
    perror("fopen");
    exit(1);
  }

  char funcbuff[64];
  char* word;
  char* nTrue = "false";
  char* salt = "$1$";
  char* crypted;
  
  //printf("%d %d\n", c, n);
  
  while (fgets(funcbuff, sizeof(funcbuff), f) != NULL) {
    
    //loop through lines of dict

    word = strtok(funcbuff, "\n");
    
    if (c == 1){
      char* mangle = malloc(32 * sizeof(char));
      strcpy(mangle, word);
      mangle[0] = mangle[0] ^ ' ';
      word = mangle;
    }

    if (n == 1){
      nTrue = nTest(word, hash);
      if (strcmp(nTrue, "false") != 0){
        return nTrue;
      }
    }
    
    crypted = crypt(word, salt);
    
    if (strcmp(hash, crypted) == 0){
      //printf("Return %s\n", word);
      return word;
    }

    /*
    // toggle first letter and check
    if (c == 1){
      printf("in toggle");
      // copy word into char array
      strcpy(mang, word);
      
      //toggle first char
      mang[0] = mang[0] ^ ' ';

      strcat(word, mang);
      crypted = crypt(word, salt);
      printf("%s: %s", word, crypted);
      if (strcmp(hash, crypted) == 0){
        printf("%s", word);
      }
    }
    */
  }
  
  return "\0";
}

int main(int argc, char *argv[]) {

  //*** Process input arguments ***//

  char *input = NULL;
  char *output = NULL;
  char *dictionary = NULL;
  
  int n = 0;
  int c = 0;
  char a;
  
  while((a = getopt(argc, argv, "i:o:d:nc")) != -1) {
      if( a == 'i') {
          input = optarg;
      }
      if( a == 'o') {
          output = optarg;
      }
      if( a == 'd') {
          dictionary = optarg;
      }
      if( a == 'n') {
          n = 1;
      }
      if( a == 'c') {
          c = 1;
      }
  }

  //printf("%d %d %s %s\n", c, n, dictionary, output);
  
   // Open the files
  FILE *f = fopen(input, "r");
  FILE *w = fopen(output, "w");

  // Check the return code
  if (f == NULL || w == NULL) {
    perror("fopen");
    exit(1);
  }
  
  // buffer for fgets
  char buf[1024];
  //int numLine = 0;
  char *hash;

  //char *found;

  // Loop over the lines of input
  while (fgets(buf, sizeof(buf), f) != NULL) {

    //numLine++;
    
    // get the first line
    
    hash = strtok(buf, ":");
    hash = strtok(NULL, "\n");

    //printf("%s\n", hashes);
    
    fprintf(w, "%s\n", checkMatch(hash, dictionary, c, n));
    }
    
  return 0;
}
