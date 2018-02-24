#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "symbolDef.h"
#include "parserDef.h"
#include "symbolStack.h"
#include "lexer.h"

void loadGrammar(char const *f_name){
	FILE * fp;
	symbols = NULL;
	char leftSide[100],rightSide[100];
	char *line = NULL;
	size_t len=0;
	fp = fopen(f_name,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}

	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s ===> %[^\n\t]",leftSide,rightSide);
		SymbolDef * symbol = getSymbolIndex(&symbols,leftSide);
		if(symbol==NULL){
			symbol = insertSymbolFromToken(&symbols,leftSide);
		}
		insertRule(&symbol,rightSide);
	}
	

	fclose(fp);
}


void loadFirst(char const *first_file){
	FILE * fp;
	char leftSide[100],rightSide[100];
	char *line = NULL;
	size_t len=0;
	fp = fopen(first_file,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}

	SymbolList * temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==1){
			SymbolList *temp2 = NULL;
			insertSymbol(&temp2,temp->symbol);
			mergeList(&temp->symbol->first,temp2,1);
		}
		temp=temp->next;
	}

	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s --> %[^\n\t]",leftSide,rightSide);
		SymbolDef * symbol = getSymbolIndex(&symbols,leftSide);
		char * token = strtok(rightSide , ", ");
		while(token!=NULL){
	    	SymbolDef * symbol2 = getSymbolIndex(&symbols,token);
	        insertSymbol(&symbol->first,symbol2);
	        token = strtok(NULL, ", ");
	    }
	}

}

void loadFollow(char const *follow_file){
	FILE * fp;
	char leftSide[100],rightSide[200];
	char *line = NULL;
	size_t len=0;
	fp = fopen(follow_file,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}

	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s --> %[^\n\t]",leftSide,rightSide);
		SymbolDef * symbol = getSymbolIndex(&symbols,leftSide);
		char * token = strtok(rightSide , ", ");
		while(token!=NULL){
	    	SymbolDef * symbol2 = getSymbolIndex(&symbols,token);
	        insertSymbol(&symbol->follow,symbol2);
	        token = strtok(NULL, ", ");
	    }
	}

	fclose(fp);
	
}


int getIndexNumber(SymbolDef *symbol){

	if(isEpsilon(symbol))
		return -1;


	SymbolList * temp = symbols;
	int ix = 1;
	while(temp!=NULL){
		if(temp->symbol ==  symbol)
			return ix;
		if(temp->symbol->isTerminal == symbol->isTerminal && !isEpsilon(temp->symbol))
			ix++;
		temp = temp->next;
	}
	return -1;
}




SymbolList *getFirsts(SymbolList *symbolsLoc){
	SymbolList * temp3 = symbolsLoc;
	SymbolList * firsts = NULL;
	int i=0;

	while(temp3!=NULL){
		if(temp3 == symbolsLoc || allEpsilon(symbolsLoc,temp3))
			 mergeList(&firsts,temp3->symbol->first,1);
		temp3=temp3->next;
	}

	return firsts;
	
}





void insertRuleParseTable(SymbolDef *symbol,SymbolList *firsts,int ruleNo){
	SymbolList * temp3 = firsts;
	int colIx,rowIx,err=0;
	while(temp3!=NULL){
		if(isEpsilon(temp3->symbol)){
			insertRuleParseTable(symbol,symbol->follow,ruleNo);
		}else{
			err=0;
			rowIx = getIndexNumber(symbol);
			colIx = getIndexNumber(temp3->symbol);
			if(ptable[rowIx][colIx].symbol!=NULL && ptable[rowIx][colIx].ruleNo!=-1){
				err=1;
				printf("Found Conflicting Grammar at (%d,%d) (%s,%s) \n",rowIx,colIx,symbol->value,temp3->symbol->value);
				printf("%s ==> ", ptable[rowIx][colIx].symbol->value);
				printSymbolList(getRuleFromIndex(ptable[rowIx][colIx].symbol,ptable[rowIx][colIx].ruleNo),NULL);
				printf("\n");
			}
			ptable[rowIx][colIx].symbol = symbol;
			ptable[rowIx][colIx].ruleNo = ruleNo;
			if(err==1){
				printf("%s ==> ", ptable[rowIx][colIx].symbol->value);
				printSymbolList(getRuleFromIndex(ptable[rowIx][colIx].symbol,ptable[rowIx][colIx].ruleNo),NULL);
				printf("\n");
				
			}
		}
		temp3=temp3->next;
	}
}

void initParseTable(){
	SymbolList *temp = symbols;
	int rowIx,colIx;

	for(rowIx=0;rowIx<50;rowIx++){
		for(colIx=0;colIx<50;colIx++){
			ptable[rowIx][colIx].symbol = NULL;
			ptable[rowIx][colIx].ruleNo = -1;
		}
	}

	while(temp!=NULL){
		if(temp->symbol->isTerminal){
			colIx = getIndexNumber(temp->symbol);
			ptable[0][colIx].symbol = temp->symbol;
		}else{
			rowIx = getIndexNumber(temp->symbol);
			ptable[rowIx][0].symbol = temp->symbol;
		}
		temp=temp->next;
	}
}

void createParseTable(){
	SymbolList *temp = symbols;
	int rowIx,colIx;
	int ruleNo;
	initParseTable();
	
	temp = symbols;
	while(temp!=NULL){
		ruleNo = 0;
		if(temp->symbol->isTerminal==0){
			RuleList * temp2 = temp->symbol->rules;
			while(temp2!=NULL){
				SymbolList * firsts = getFirsts(temp2->rule_tokens);
				insertRuleParseTable(temp->symbol,firsts,ruleNo);
				temp2=temp2->next;
				ruleNo++;
			}
		}
		temp=temp->next;
	}
}

void saveParseTable(char const *f_name){
	FILE *fp = fopen(f_name,"w");
	int rowIx,colIx;
	for(rowIx=0;rowIx<50;rowIx++){
		for(colIx=0;colIx<50;colIx++){
			if(ptable[rowIx][colIx].symbol!=NULL){
				fprintf(fp,"%s",ptable[rowIx][colIx].symbol->value);
				if(ptable[rowIx][colIx].ruleNo!=-1){
					fprintf(fp," ==> ");
					printSymbolList(getRuleFromIndex(ptable[rowIx][colIx].symbol,ptable[rowIx][colIx].ruleNo),fp);
				}
			}
			fprintf(fp,",");
		}
		fprintf(fp,"\n");
	}
}

void loadData(char const *gm_file,char const *first_file,char const *follow_file){
	loadGrammar(gm_file);
	loadFirst(first_file);
	loadFollow(follow_file);
}

void createParseTree(char const *file_name){
	SymbolStack *symStack = NULL;
	push(&symStack,makeSymbol("$"));
	push(&symStack,getSymbolIndex(&symbols,"<mainFunction>"));


	FILE *fp = fopen(file_name,"r");
	tokenDesc token;
	line = 1;
	char *fileBuff = (char*)malloc(BUFF_SIZE*sizeof(char));
	
	// printf("%20s%20s%20s\n","Token Name","Lexeme","Line");
	SymbolDef *k=NULL;

	token = getToken(fp,&fileBuff);
	fileBuff++;
	int i=0;
	do{
		printf("Read: %10s%10s\n",getTokenFromId(token.id,token.name),token.name);
		printStack(symStack);
		if(k!=NULL)
			printf("%p n value : %s\n",k,k->value);
		// printf("2,%p\n",symStack);
		// printf("st_top sym:%p\n",symStack->symbol);
		if(isEndSymbol(symStack->symbol) || symStack->symbol->isTerminal ){
			if(strcmp(symStack->symbol->value,getTokenFromId(token.id,token.name))==0){
				pop(&symStack);	
				token = getToken(fp,&fileBuff);
				while(token.id==3)
					token = getToken(fp,&fileBuff);

				// printf("Read: %10s%10s \t",getTokenFromId(token.id,token.name),token.name);
				// int i;
				// for(i=0;i<strlen(token.name);i++)
				// 	printf("%d ",token.name[i]);
				// printf("\n");
				fileBuff++;
			}
			else{
				printf("1 SYNTACTIC ERROR\n");
				printf("Expected %s \nFound %s\n",symStack->symbol->value,getTokenFromId(token.id,token.name));
				exit(0);
			}
		}else{
			int rowIx = getIndexNumber(symStack->symbol);
			int colIx = getIndexNumber(getSymbolIndex(&symbols,getTokenFromId(token.id,token.name)));
			if(ptable[rowIx][colIx].symbol==NULL){
				printf("2 SYNTACTIC ERROR\n");
				printf("No rule for (%s,%s)\n",symStack->symbol->value,getTokenFromId(token.id,token.name));
				exit(0);
			}else{
				pop(&symStack);	
				SymbolList *rules = getRuleFromIndex(ptable[rowIx][colIx].symbol,ptable[rowIx][colIx].ruleNo);
				if(strcmp(ptable[rowIx][colIx].symbol->value,"<stmtsAndFunctionDefs>")==0){
					SymbolList *t2 = rules;
					while(t2!=NULL){
						if(strcmp(t2->symbol->value,"<stmtsAndFunctionDefsOrEmpty>")==0){
							k=t2->symbol;
							printf("found %p\n",k);
						}
						t2=t2->next;
					}
					
				}
				pushAll(&symStack,rules);
				// printf("%2d %s",++i,ptable[rowIx][colIx].symbol->value);
				// printf(" ==> ");
				// printSymbolList(rules,NULL);
				// printf("\n");
			}
		}
		// printf("1\n");
	}while(token.id!=0);

	// while(symStack){
	// 	SymbolDef *symbol = pop(&symStack); 
	// 	printf("%s\n",symbol->value);
	// }
}

int main(int argc, char const *argv[])
{
	loadData(argv[1],argv[2],argv[3]);
	createParseTable();
	saveParseTable(argv[4]);
	createParseTree(argv[5]);
	return 0;
}
