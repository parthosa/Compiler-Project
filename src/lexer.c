#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "langDef.h"

tokenDesc setTokenValue(int state,char * name,int line){
	tokenDesc token;
	memset(token.name,'\0',MAX_LENGTH+1);
	strcpy(token.name,name);
	token.id=state;
	token.line=line;
	return token;
}

tokenDesc getToken(FILE *fp,char **fileBuffInit,char *lexeme,int *begin){
	int state=1,offset=0,tokLen=0,tokLenErr=0,unknownErr=0;
	char ch;
	char *fileBuff = (*fileBuffInit)+(*begin);
	memset(lexeme,'\0',MAX_LENGTH+1);
	
	while(1){
		if(*fileBuff=='\0'){
			memset(*fileBuffInit,'\0',BUFF_SIZE);
			fread(*fileBuffInit,1,BUFF_SIZE,fp);
			*begin = 0;
			fileBuff = (*fileBuffInit)+(*begin);
		}
		ch=*fileBuff;
		// printf("%c ",ch );
		if(ch=='\0'){
			if(state==1){
				strcpy(lexeme,"$");
				state=0;
			}
			*begin = fileBuff- (*fileBuffInit);
			return setTokenValue(state,lexeme,line);
		}

		if(ch=='\n'){
			if(state==1)
				line++;
			else if(state!=2 && state!=23 && state!=42 && state!=45){ // not for comment lines
				fileBuff-=1;
				printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
				state=-1;
				*begin = fileBuff- (*fileBuffInit);
				return setTokenValue(state,lexeme,line);
			}
		}
		// check for invalid characters
		if(ch<0){
			*begin = fileBuff- (*fileBuffInit);
			return setTokenValue(state,lexeme,line);
		}
		switch(state){
			case 1:
				switch(ch){
					case ' ':
					case '\t':
					case '\n':
					case '\r':
						break;
					case '#':
						state = 2;
						break;
					case '[':
						state = 4;
						strcpy(lexeme,"[");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case ']':
						state = 5;
						strcpy(lexeme,"]");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case '(':
						state = 6;
						strcpy(lexeme,"(");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case ')':
						state = 7;
						strcpy(lexeme,")");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case ';':
						state = 8;
						strcpy(lexeme,";");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case ',':
						state = 9;
						strcpy(lexeme,",");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case '+':
						state = 10;
						strcpy(lexeme,"+");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case '-':
						state = 11;
						strcpy(lexeme,"-");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case '*':
						state = 12;
						strcpy(lexeme,"*");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case '/':
						state = 13;
						strcpy(lexeme,"/");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case '@':
						state = 14;
						strcpy(lexeme,"@");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					case'<':
						state=15;
						lexeme[offset++] = ch;
						break;
					case'>':
						state=17;
						lexeme[offset++] = ch;
						break;
				    case '=':
				    	state = 19;
						lexeme[offset++] = ch;
						break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						tokLen=1;
						tokLenErr=0;
						state = 23;
						lexeme[offset++] = ch;
						break;
					case '"':
						state = 27;
						tokLen=0;
						tokLenErr=0;
						unknownErr=0;
						lexeme[offset++] = ch;
						break;
					case '.':
						state = 30;
						lexeme[offset++] = ch;
						break;
					case 'a': case 'A':
					case 'b': case 'B':
					case 'c': case 'C':
					case 'd': case 'D':
					case 'e': case 'E':
					case 'f': case 'F':
					case 'g': case 'G':
					case 'h': case 'H':
					case 'i': case 'I':
					case 'j': case 'J':
					case 'k': case 'K':
					case 'l': case 'L':
					case 'm': case 'M':
					case 'n': case 'N':
					case 'o': case 'O':
					case 'p': case 'P':
					case 'q': case 'Q':
					case 'r': case 'R':
					case 's': case 'S':
					case 't': case 'T':
					case 'u': case 'U':
					case 'v': case 'V':
					case 'w': case 'W':
					case 'x': case 'X':
					case 'y': case 'Y':
					case 'z': case 'Z':
						state = 42;
						tokLen=1;
						tokLenErr=0;
						lexeme[offset++] = ch;
						break;
					case '_':
						state = 44;
						tokLen=1;
						tokLenErr=0;
						lexeme[offset++] = ch;
						break;
					default:
						state = -1;
						// strcpy(lexeme,"ERROR");
						lexeme[offset++] = ch;
						printf("%d: LEXICAL ERROR: Unknown Symbol %s\n",line,lexeme);
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
				}
				break;
			// <,<=,>,>=,==
			case 2:
				if(ch!='\n'){
					state=2;
				}else{
					state=3;
					fileBuff-=1;
					strcpy(lexeme,"COMMENT");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 15:
			case 17:
			case 19:
				if(strcmp(lexeme,"=")==0 && ch=='/'){
					if(*(fileBuff+1)!='='){ // check for empty
						// printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
						fileBuff-=1;
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					}
					state=21;
					lexeme[offset++]=ch;
				}
				else {
					if(ch=='='){
						state++;
						lexeme[offset++]=ch;
					}
					else{
						fileBuff-=1;
					}
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			// =/= 
			case 21:
				if(ch=='='){
					lexeme[offset++]=ch;
					state++;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 23:
				if(isdigit(ch)){
					state = 23;
					if(tokLen<MAX_LENGTH){
						lexeme[offset++] = ch;
						tokLen++;
					}
				}else if(ch=='.'){
					if(isalpha(*(fileBuff+1))){
						if(*(fileBuff+1)=='a'||*(fileBuff+1)=='o'||*(fileBuff+1)=='n'){
							fileBuff-=1;
							*begin = fileBuff- (*fileBuffInit);
							return setTokenValue(state,lexeme,line);
						}else{
							fileBuff-=1;
							*begin = fileBuff - (*fileBuffInit);
							return setTokenValue(state,lexeme,line);
						}
					}else{
						// fileBuff-=1;
						state = 24;
						if(tokLen<MAX_LENGTH){
							lexeme[offset++] = ch;
							tokLen++;
						}
					}
				}else{
					// lexeme[offset++] = '\0';
					fileBuff-=1;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 24:
				if(isdigit(ch)){
					if(tokLen<MAX_LENGTH){
						lexeme[offset++] = ch;
						tokLen++;
					}
					state++;
				}else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff - (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 25:
				if(isdigit(ch)){
					if(tokLen<MAX_LENGTH){
						lexeme[offset++] = ch;
						tokLen++;
					}
					state++;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}else{
					
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 27:
				if ((ch >= 'a' && ch <= 'z')|| ch==' '){
					lexeme[offset++] = ch;
					state=28;
					tokLen++;
				}else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 28:
				if ((ch >= 'a' && ch <= 'z') || ch==' '){
					state=28;
					if(tokLen<(MAX_LENGTH-2)){
						lexeme[offset++] = ch;
						tokLen++;
					}
					else
						tokLenErr=1;
				}else if(ch=='"'){
					state=29;
					*begin = fileBuff- (*fileBuffInit);
					lexeme[offset++] = ch;
					tokLen++;
					// printf("len: %d\n",tokLen );
					if(unknownErr){
						state=-1;
						// lexeme[offset++] = ch;
						printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
						return setTokenValue(state,lexeme,line);
					}
					if(tokLenErr){
						// state=-1;
						printf("%d: LEXICAL ERROR: String is longer than the prescribed length: %s...\n",line,lexeme);
						return setTokenValue(state,lexeme,line);
					}
					return setTokenValue(state,lexeme,line);

				}
				else{
					state=28;
					unknownErr=1;
					if(tokLen<(MAX_LENGTH-2)){
						lexeme[offset++] = ch;
						tokLen++;
					}
					else
						tokLenErr=1;
					// strcpy(lexeme,"ERROR");
				}
				break;
			case 30:
				switch(ch){
					case 'o':
						lexeme[offset++] = ch;
						state=31;
						break;
					case 'a':
						lexeme[offset++] = ch;
						state=34;
						break;
					case 'n':
						lexeme[offset++] = ch;
						state=38;
						break;
					default:
						if(!isalpha(ch))
							fileBuff-=1;
						else
							lexeme[offset++] = ch;
						state = -1;
						printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
						break;
				}
				break;
			case 31:
				if(ch=='r'){
					lexeme[offset++] = ch;
					state=32;
				}
				else{
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 32:
				if(ch=='.'){
					lexeme[offset++] = ch;
					state=33;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 34:
				if(ch=='n'){
					lexeme[offset++] = ch;
					state=35;
				}
				else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 35:
				if(ch=='d'){
					lexeme[offset++] = ch;
					state=36;
				}
				else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 36:
				if(ch=='.'){
					lexeme[offset++] = ch;
					state=37;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 38:
				if(ch=='o'){
					lexeme[offset++] = ch;
					state=39;
				}
				else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 39:
				if(ch=='t'){
					lexeme[offset++] = ch;
					state=40;
				}
				else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 40:
				if(ch=='.'){
					state=41;
					lexeme[offset++] = ch;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				else{
					// strcpy(lexeme,"ERROR");
					fileBuff-=1;
					state = -1;
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 42:
				if(isalpha(ch)){
					state=42;
					if(tokLen<MAX_LENGTH){
						lexeme[offset++] = ch;
						tokLen++;
					}
					else
						tokLenErr=1;
				}else if(isdigit(ch)){
					state=43;
					if(tokLen<MAX_LENGTH){
						lexeme[offset++] = ch;
						tokLen++;
					}
					else
						tokLenErr=1;

					*begin = fileBuff- (*fileBuffInit);
					if(tokLenErr){
						// state = -1;
						printf("%d: LEXICAL ERROR: Identifier is longer than the prescribed length '%s...'\n",line,lexeme);
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					}
					return setTokenValue(state,lexeme,line);
				}else{
					fileBuff-=1;
					if(tokLenErr){
						// state = -1;
						printf("%d: LEXICAL ERROR: Identifier is longer than the prescribed length '%s...'\n",line,lexeme);
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					}
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
					
				}
				break;
			case 44:
				if(isalpha(ch)){
					tokLen++;
					lexeme[offset++] = ch;
					state=45;
				}else{
					fileBuff-=1;
					state = -1;
					// strcpy(lexeme,"ERROR");
					printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			case 45:
				if(isalpha(ch)||isdigit(ch)){
					state=45;
					if(tokLen<MAX_LENGTH){
						lexeme[offset++] = ch;
						tokLen++;
					}
					else
						tokLenErr=1;
				}else{
					fileBuff-=1;
					if(tokLenErr){
						// state = -1;
						printf("%d: LEXICAL ERROR: Identifier is longer than the prescribed length '%s...'\n",line,lexeme);
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme,line);
					}
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme,line);
				}
				break;
			default:
				// strcpy(lexeme,"ERROR");
				lexeme[offset++] = ch;
				state = -1;
				printf("%d: LEXICAL ERROR: Unknown Pattern %s\n",line,lexeme);
				*begin = fileBuff- (*fileBuffInit);
				return setTokenValue(state,lexeme,line);
		}
		fileBuff++;
	}
	state = 1;
	*begin = fileBuff- (*fileBuffInit);
	return setTokenValue(state,lexeme,line);
}


char *getTokenForIdentifiers(int id,char *name){
	if(strcmp(name,"int")==0)
		return "INT";
	if(strcmp(name,"end")==0)
		return "END";
	if(strcmp(name,"real")==0)
		return "REAL";
	if(strcmp(name,"string")==0)
		return "STRING";
	if(strcmp(name,"matrix")==0)
		return "MATRIX";
	if(strcmp(name,"if")==0)
		return "IF";
	if(strcmp(name,"else")==0)
		return "ELSE";
	if(strcmp(name,"endif")==0)
		return "ENDIF";
	if(strcmp(name,"read")==0)
		return "READ";
	if(strcmp(name,"print")==0)
		return "PRINT";
	if(strcmp(name,"function")==0)
		return "FUNCTION";
	return "ID";
}

char *getTokenForFunctions(int id,char *name){
	if(strcmp(name,"_main")==0)
		return "MAIN";
	else
		return "FUNID";
}



char *getTokenFromId(int id,char * name){
	switch(id){
		case 0:return "EOF";
		case 3:return "COMMENT";
		case 4:return "SQO";
		case 5:return "SQC";
		case 6:return "OP";
		case 7:return "CL";
		case 8:return "SEMICOLON";
		case 9:return "COMMA";
		case 10:return "PLUS";
		case 11:return "MINUS";
		case 12:return "MUL";
		case 13:return "DIV";
		case 14:return "SIZE";
		case 15:return "LT";
		case 16:return "LE";
		case 17:return "GT";
		case 18:return "GE";
		case 19:return "ASSIGNOP";
		case 20:return "EQ";
		case 22:return "NE";
		case 23:return "NUM";
		case 26:return "RNUM";
		case 29:return "STR";
		case 33:return "OR";
		case 37:return "AND";
		case 41:return "NOT";
		case 42:
		case 43:return getTokenForIdentifiers(id,name);
		case 45:return getTokenForFunctions(id,name);
		case -1:return "ERROR";
		default:return "ERROR";
	}
}


void removeComments(char *testFile,char *cleanFile){
	int i,j;
	for(i=0,j=0;testFile[i]!='\0';i++){
		if(testFile[i]=='#'){
			while(testFile[i]!='\0' && testFile[i]!='\n')
				i++;
			i++;
			while(testFile[i]!='\0' && (testFile[i]==' ' || testFile[i]=='\t'))
				i++;
			i--;
		}else{
			cleanFile[j] = testFile[i];
			j++;
		}
	}
}
