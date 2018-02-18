#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#define BUFF_SIZE 64


void printTokens(char const *file_name){
	FILE *fp;
	fp = fopen(file_name,"r");
	line = 1;
	int state;
	char *fileBuff = (char*)malloc(BUFF_SIZE*sizeof(char));
	memset(fileBuff,'\0',BUFF_SIZE);
	int rd;
	tokenDesc token;
	printf("%20s%20s%20s\n","Token Name","Lexeme","Line");
	
	state = 1;
	char *lexeme;
	// memset(lexeme,0,MAX_LENGTH);
	while(!feof(fp)){
		fread(fileBuff,BUFF_SIZE,1,fp);
		// printf("\n\t*****\n%s\n\t****\n",fileBuff);
		while(*fileBuff!='\0'){
			lexeme = (char*)malloc(MAX_LENGTH * sizeof(char));
			memset(lexeme,'\0',MAX_LENGTH);
			token = getToken(&fileBuff,&state,&lexeme);
			if(state==1){
			// printf("%s\n",lexeme);
				printf("%20s%20s%20d\n",getTokenFromId(token.id,token.name),token.name,line);
			}
			fileBuff++;
		}
	}
	fclose(fp);
}


int main(int argc, char const *argv[])
{
	printTokens(argv[1]);
	return 0;
}