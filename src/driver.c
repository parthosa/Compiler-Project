#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"



void printTokens(char const *file_name){
	FILE *fp = fopen(file_name,"r");
	tokenDesc token;
	line = 1;
	char *fileBuff = (char*)malloc(BUFF_SIZE*sizeof(char));
	memset(fileBuff,'\0',BUFF_SIZE);
	
	printf("%20s%20s%20s\n","Token Name","Lexeme","Line");

	while(!feof(fp)){
		fread(fileBuff,1,BUFF_SIZE,fp);
		while(*fileBuff!='\0'){
			token = getToken(fp,&fileBuff);

			if(token.id!=1){
				printf("%20s%20s%20d\n",getTokenFromId(token.id,token.name),token.name,line);
			}
			fileBuff++;
		}
	}
	// fclose(fp);
}


int main(int argc, char const *argv[])
{
	printTokens(argv[1]);
	return 0;
}