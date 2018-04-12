/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "lexer.h"
#include "firstAndFollow.h"
#include "parser.h"
#include "ast.h"



int getSizeOfFile(FILE *fp){
	fseek(fp, 0L, SEEK_END);
	int sizeOfFile = ftell(fp);
	rewind(fp);
	return sizeOfFile;
}

void removeCommentsWrapper(FILE *fp){
	int sizeOfFile = getSizeOfFile(fp);
	char *fileBuff = (char*)malloc(sizeOfFile*sizeof(char));
	char *clean_file = (char*)malloc(sizeOfFile*sizeof(char));
	memset(fileBuff,'\0',sizeOfFile);
	memset(clean_file,'\0',sizeOfFile);
	fread(fileBuff,1,sizeOfFile,fp);
	removeComments(fileBuff,clean_file);
	printf("%s\n",clean_file);
	rewind(fp);
	// fwrite(clean_file,1,sizeOfFile,fp);
	// rewind(fp);
}


void printTokens(FILE *fp){
	TokenInfo token;
	line = 1;
	int begin = 0;
	char *fileBuff = (char*)malloc(BUFF_SIZE*sizeof(char));
	memset(fileBuff,'\0',BUFF_SIZE);

	char lexeme[MAX_LENGTH+1];
	
	printf("%20s%20s%20s\n","Token Name","Lexeme","Line");

	do{
		token = getToken(fp,&fileBuff,lexeme,&begin);
		begin++;
		while(token.id==-1){
			token = getToken(fp,&fileBuff,lexeme,&begin);
			begin++;
		}
		printf("%20s%20s%20d\n",getTokenFromId(token.id,token.name),token.name,token.line);
	}while(token.id!=0);
	free(fileBuff);
	rewind(fp);
}


void parserWrapper(char *grammarFile,char *firstFile,char *followFile,char *parseTableFile,FILE *fp){
	loadGrammar(grammarFile);
	findFirstAndFollow();
	saveFirstAndFollow(firstFile,followFile);
	createParseTable();
	saveParseTable(parseTableFile);
	createParseTree(fp);
	rewind(fp);
}


void printSeperator(){
	printf("****************************************************\n");
}


void initialize(){
	symbols = NULL;
}

int main(int argc, char const *argv[])
{
	if(argc<2){
		printf("ERROR: All input files not provided\nCOMMAND: ./stage1exe <source code file>\n");
		exit(0);
	}


	FILE *fp = fopen(argv[1],"r+");
	if(fp==NULL){
		printf("ERROR: Source file does not exist\n");
		exit(0);
	}

	

	char *grammarFile = "language/grammar.txt";
	char *firstFile = "language/first.txt";
	char *followFile = "language/follow.txt";
	char *parseTableFile = "language/parseTable.csv";
	char *parseTreeFile = "language/parseTree.txt";
	char *ASTFile = "language/ASTree.txt";


	FILE *fp2 = fopen(parseTreeFile,"w");
	if(fp2==NULL){
		printf("ERROR: Source file does not exist\n");
		exit(0);
	}

	FILE *fp3 = fopen(ASTFile,"w");
	if(fp2==NULL){
		printf("ERROR: Source file does not exist\n");
		exit(0);
	}

	printSeperator();
    printf("%41s\n","COMPILER CONSTRUCTION: STAGE 1");
	printSeperator();
    printf("\nImplementations:\n");
    printf("\t a)FIRST and FOLLOW set automated\n");
    printf("\t b)Both Lexical and Syntax analysis modules implemented\n");
    printf("\t c)Lexical Error handling and Panic Mode Recovery implemented\n");
    printf("\t d)Parse Tree implemented\n");
    printf("\t e)All test cases work\n");
    printf("\n");
	printSeperator();

	initialize();
    while(1){


        fflush(stdin);
        printf("\nPress option for the defined task:\n");
        printf("\t1 : For removal of comments\n");
        printf("\t2 : For printing the token list generated by the lexer.\n");
        printf("\t3 : For parsing to verify the syntactic correctness of the input source code\n");
        printf("\t4 : For printing the parse tree\n");
        printf("\t5 : For printing the abstract syntax tree\n");
        printf("\t0 : Exit\n");
        printf("\n\nEnter Value: ");
        char option;
        scanf(" %c",&option);

        switch(option){
        	case '1':
				printf("\nCleaned File:\n\n");
	        	removeCommentsWrapper(fp);
	        	break;
	        case '2':
				printf("\nPrinting Tokens:\n\n");
		        printTokens(fp);
		        break;
		    case '3':
				printf("\nChecking the file for syntactic correctness:\n");
				parserWrapper(grammarFile,firstFile,followFile,parseTableFile,fp);
				saveParseTree(pTreeHead,fp2);
				break;
			case '4':
				printf("\nPrinting Parse Tree:\n\n");
				saveParseTree(pTreeHead,stdout);
				break;
			case '5':
				printf("\nPrinting AST:\n\n");
				ASTHead = buildAST(pTreeHead);
				printAST(ASTHead,0);
				// saveParseTree(ASTHead,fp3);
				// saveParseTree(ASTHead,stdout);
				break;
			case '0':
				printf("\n\nProgram Terminating....\n");
				exit(0);
			default :
				printf("\n\nInvalid Input. Please Try Again\n");
				break;
        }
        printf("\n");
        printSeperator();
    }
	fclose(fp);
	return 0;
}