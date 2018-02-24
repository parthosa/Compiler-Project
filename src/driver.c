#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"



void printTokens(char const *file_name){
	FILE *fp = fopen(file_name,"r");
	tokenDesc token;
	line = 1;
	int begin = 0;
	char *fileBuff = (char*)malloc(BUFF_SIZE*sizeof(char));
	memset(fileBuff,'\0',BUFF_SIZE);

	char lexeme[MAX_LENGTH];
	
	printf("%20s%20s%20s\n","Token Name","Lexeme","Line");

	do{
		token = getToken(fp,&fileBuff,lexeme,&begin);
		printf("%20s%20s%20d\n",getTokenFromId(token.id,token.name),token.name,line);
		begin++;
	}while(token.id!=0);
	free(fileBuff);
}


int main(int argc, char const *argv[])
{
	printTokens(argv[1]);
	return 0;
}