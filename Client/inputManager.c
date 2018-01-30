#include "inputManager.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#define LINE_MAX 77
#define CLEAN_BUFFER(ch) while ((ch = getchar()) != '\n');

char getOption(){
  int i,c, aux;
  c = getchar();
  for(i = 1;(aux = getchar()) != '\n'; i++){
    if(aux == EOF)
      return EOF;
  }

  return (i == 1)? c : UNEXPECTED_ERROR;
}

int getInt(){

    char *p, s[LINE_MAX];
    int n;

    if(fgets(s, sizeof(s), stdin)) {
        n = strtol(s, &p, 10);
        if ((p != s && *p == '\n'))
            return n;
    }
    return UNEXPECTED_ERROR;
}



