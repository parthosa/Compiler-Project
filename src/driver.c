#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"



void printTokens(char const *file_name){
	FILE *fp = fopen(file_name,"r");
	tokenDesc token;
	line = 1;
	char *fileBuff = (char*)malloc(BUFF_SIZE*sizeof(char));
	
	printf("%20s%20s%20s\n","Token Name","Lexeme","Line");

	do{
		token = getToken(fp,&fileBuff);
		printf("%20s%20s%20d\n",getTokenFromId(token.id,token.name),token.name,line);
		fileBuff++;
	}while(token.id!=0);

}


int main(int argc, char const *argv[])
{
	printTokens(argv[1]);
	return 0;
}