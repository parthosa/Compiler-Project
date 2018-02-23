#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "symbolDef.h"
#include "parserDef.h"



void loadGrammar(char const *f_name){
	FILE * fp;
	symbols = NULL;
	char leftSide[100],rightSide[100];
	char *line = NULL;
	size_t len=0;
	fp = fopen(f_name,"rowIx");
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
	fp = fopen(first_file,"rowIx");
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
	fp = fopen(follow_file,"rowIx");
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





void insertRulePTable(SymbolDef *symbol,SymbolList *firsts,RuleList *rule){
	SymbolList * temp3 = firsts;
	int colIx,rowIx,err=0;
	while(temp3!=NULL){
		if(isEpsilon(temp3->symbol)){
			insertRulePTable(symbol,symbol->follow,rule);
		}else{
			err=0;
			rowIx = getIndexNumber(symbol);
			colIx = getIndexNumber(temp3->symbol);
			if(ptable[rowIx][colIx].symbol!=NULL && ptable[rowIx][colIx].rule!=NULL){
				err=1;
				printf("Found Conflicting Grammar at (%d,%d) (%s,%s) \n",rowIx,colIx,symbol->value,temp3->symbol->value);
				printf("%s ==> ", ptable[rowIx][colIx].symbol->value);
				printSymbolList(ptable[rowIx][colIx].rule->rule_tokens,NULL);
				printf("\n");
			}
			ptable[rowIx][colIx].symbol = symbol;
			ptable[rowIx][colIx].rule = rule;
			if(err==1){
				printf("%s ==> ", ptable[rowIx][colIx].symbol->value);
				printSymbolList(ptable[rowIx][colIx].rule->rule_tokens,NULL);
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
			ptable[rowIx][colIx].rule = NULL;
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

	initParseTable();
	
	temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==0){
			RuleList * temp2 = temp->symbol->rules;
			while(temp2!=NULL){
				SymbolList * firsts = getFirsts(temp2->rule_tokens);
				insertRulePTable(temp->symbol,firsts,temp2);
				temp2=temp2->next;
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
				if(ptable[rowIx][colIx].rule!=NULL){
					fprintf(fp," ==> ");
					printSymbolList(ptable[rowIx][colIx].rule->rule_tokens,fp);
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

int main(int argc, char const *argv[])
{
	loadData(argv[1],argv[2],argv[3]);
	createParseTable();
	saveParseTable(argv[4]);
	return 0;
}
