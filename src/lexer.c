#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "langDef.h"

tokenDesc setTokenValue(int state,char * name){
	tokenDesc token;
	token.id=state;
	strcpy(token.name,name);
	return token;
}

tokenDesc getToken(FILE *fp,char **fileBuffInit,char *lexeme,int *begin){
	int state=1,offset=0;
	char ch;
	char *fileBuff = (*fileBuffInit)+(*begin);
	memset(lexeme,'\0',MAX_LENGTH);
	
	while(1){
		if(*fileBuff=='\0'){
			memset(*fileBuffInit,'\0',BUFF_SIZE);
			fread(*fileBuffInit,1,BUFF_SIZE,fp);
			*begin = 0;
			fileBuff = (*fileBuffInit)+(*begin);
		}
		ch=*fileBuff;

		if(ch=='\0'){
			strcpy(lexeme,"$");
			state=0;
			*begin = fileBuff- (*fileBuffInit);
			return setTokenValue(state,lexeme);
		}

		if(ch=='\n'){
			if(state==1)
				line++;
			else if(state!=2){ // not for comment lines
				fileBuff-=1;
				*begin = fileBuff- (*fileBuffInit);
				return setTokenValue(state,lexeme);
			}
		}
		// check for invalid characters
		if(ch<0){
			*begin = fileBuff- (*fileBuffInit);
			return setTokenValue(state,lexeme);
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
						return setTokenValue(state,lexeme);
					case ']':
						state = 5;
						strcpy(lexeme,"]");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case '(':
						state = 6;
						strcpy(lexeme,"(");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case ')':
						state = 7;
						strcpy(lexeme,")");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case ';':
						state = 8;
						strcpy(lexeme,";");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case ',':
						state = 9;
						strcpy(lexeme,",");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case '+':
						state = 10;
						strcpy(lexeme,"+");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case '-':
						state = 11;
						strcpy(lexeme,"-");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case '*':
						state = 12;
						strcpy(lexeme,"*");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case '/':
						state = 13;
						strcpy(lexeme,"/");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					case '@':
						state = 14;
						strcpy(lexeme,"@");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
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
						state = 23;
						lexeme[offset++] = ch;
						break;
					case '"':
						state = 27;
						// lexeme[offset++] = ch;
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
						lexeme[offset++] = ch;
						break;
					case '_':
						state = 44;
						lexeme[offset++] = ch;
						break;
					default:
						state = 1;
						strcpy(lexeme,"ERROR");
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
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
					return setTokenValue(state,lexeme);
				}
				break;
			case 15:
			case 17:
			case 19:
				if(strcmp(lexeme,"=")==0 && ch=='/'){
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
					return setTokenValue(state,lexeme);
				}
				break;
			// =/= 
			case 21:
				if(ch=='='){
					state++;
					lexeme[offset++]=ch;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 23:
				if(isdigit(ch)){
					state = 23;
				}else if(ch=='.'){
					if(isalpha(*fileBuff)){
						fileBuff-=2;
						*begin = fileBuff- (*fileBuffInit);
						return setTokenValue(state,lexeme);
					}else{
						// fileBuff-=1;
						state = 24;
					}
				}else{
					fileBuff-=1;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				lexeme[offset++] = ch;
				break;
			case 24:
				if(isdigit(ch)){
					state++;
					lexeme[offset++] = ch;
				}else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 25:
				if(isdigit(ch)){
					state++;
					lexeme[offset++] = ch;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}else{
					
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 27:
				if ((ch >= 'a' && ch <= 'z')|| ch==' '){
					state=28;
					lexeme[offset++] = ch;
				}else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 28:
				if ((ch >= 'a' && ch <= 'z') || ch==' '){
					state=28;
					lexeme[offset++] = ch;
				}else if(ch=='"'){
					state=29;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 30:
				switch(ch){
					case 'o':
						state=31;
						break;
					case 'a':
						state=34;
						break;
					case 'n':
						state=38;
						break;
					default:
						// throw err
						break;
				}
				lexeme[offset++] = ch;
				break;
			case 31:
				if(ch=='r')
					state=32;
				else{
					// throw err
				}
				lexeme[offset++] = ch;
				break;
			case 32:
				if(ch=='.'){
					state=33;
					lexeme[offset++] = ch;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 34:
				if(ch=='n')
					state=35;
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				lexeme[offset++] = ch;
				break;
			case 35:
				if(ch=='d')
					state=36;
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				lexeme[offset++] = ch;
				break;
			case 36:
				if(ch=='.'){
					state=37;
					lexeme[offset++] = ch;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 38:
				if(ch=='o')
					state=39;
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				lexeme[offset++] = ch;
				break;
			case 39:
				if(ch=='t')
					state=40;
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				lexeme[offset++] = ch;
				break;
			case 40:
				if(ch=='.'){
					state=41;
					lexeme[offset++] = ch;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 42:
				if(isalpha(ch)){
					state=42;
					lexeme[offset++] = ch;
				}else if(isdigit(ch)){
					state=43;
					lexeme[offset++] = ch;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}else{
					fileBuff-=1;
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 44:
				if(isalpha(ch)){
					state=45;
					lexeme[offset++] = ch;
				}else{
					strcpy(lexeme,"ERROR");
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			case 45:
				if(isalpha(ch)||isdigit(ch)){
					state=45;
					lexeme[offset++] = ch;
				}else{
					fileBuff-=1;
					fflush(stdout);
					*begin = fileBuff- (*fileBuffInit);
					return setTokenValue(state,lexeme);
				}
				break;
			default:
				strcpy(lexeme,"ERROR");
				*begin = fileBuff- (*fileBuffInit);
				return setTokenValue(state,lexeme);
		}
		fileBuff++;
	}
	state = 1;
	*begin = fileBuff- (*fileBuffInit);
	return setTokenValue(state,lexeme);
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
		default:return "ERROR";
	}
}