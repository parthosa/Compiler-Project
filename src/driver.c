#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"


void printTokens(char *file_name){
	FILE *fp;
	fp = fopen(file_name,"r");
	line = 1;
	tokenDesc token;
	printf("%20s%20s%20s\n","Token Name","Lexeme","Line");
	while(!feof(fp)){
		token = getToken(fp);
		if(token.id!=1){
			printf("%20s%20s%20d\n",getTokenFromId(token.id,token.name),token.name,line);
		}
	}
	fclose(fp);
}


int main(int argc, char const *argv[])
{
	printTokens(argv[1]);
	return 0;
}