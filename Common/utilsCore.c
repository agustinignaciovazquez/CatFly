
#include "utilsCore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copyStr(char * str, const char * data,  int max){
	int i;

	for(i = 0; i < max-1 && data[i] != '\0';i++){
		str[i] = data[i];
	}
	str[i] = '\0';
	i++;

	return i;
}

int copyBytes(void * dest, const void * data, int bytes){
	memcpy(dest, data, bytes);
	return bytes;
}

char * * dinamicMatrix(unsigned int rows, unsigned int columns){
    int i;
    char * * matrix;

    matrix = calloc(rows,sizeof(char *));
    if(matrix != NULL){
        for(i = 0;i < rows;i++){
            matrix[i] = calloc(columns,sizeof(char));
            if(matrix[i] == NULL){
                freeMatrix(matrix,i);
                return NULL;
            }
        }
    }
    return matrix;
}

void freeMatrix(char * * matrix, unsigned int rows){
    int i;
    for(i=0; i < rows;i++){
        free(matrix[i]);
    }
    free(matrix);
}