#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "symbolDef.h"
#include "parserDef.h"
#include "symbolStack.h"
#include "lexer.h"

ParseTree *pTree,*pTreeHead;


void loadGrammar(char const *f_name){
	FILE * fp;
	symbols = NULL;
	char leftSide[100],rightSide[100];
	char *line = NULL;
	size_t len=0;
	SymbolDef * symbol;
	fp = fopen(f_name,"r");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}
	while(getline(&line,&len,fp)!=-1){
		sscanf(line,"%s ===> %[^\n\t]",leftSide,rightSide);
		symbol = getSymbolIndex(&symbols,leftSide);
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
		if(temp->symbol == '\0')
			return -1;
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
SymbolList *temp;
void initParseTable(){
	temp = symbols;
	int rowIx,colIx;

	for(rowIx=0;rowIx<45;rowIx++){
		for(colIx=0;colIx<45;colIx++){
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
	// int rowIx,colIx;
	int ruleNo;
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
	for(rowIx=0;rowIx<45;rowIx++){
		for(colIx=0;colIx<45;colIx++){
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

	pTree = addChild(pTree,getSymbolIndex(&symbols,MAINFUNCTION));
	pTreeHead = pTree;

	tokenDesc tokenLex;

	push(&symStack,makeSymbol(ENDSYMBOL));
	push(&symStack,getSymbolIndex(&symbols,MAINFUNCTION));

	
	FILE *fp = fopen(file_name,"r");
	char lexeme[MAX_LENGTH];
	line = 1;
	
	int begin = 0,i=0;
	char *fileBuff = (char*)malloc(BUFF_SIZE*sizeof(char));
	memset(fileBuff,'\0',BUFF_SIZE);

	tokenLex = getToken(fp,&fileBuff,lexeme,&begin);
	begin++;
	while(tokenLex.id==-1){
		tokenLex = getToken(fp,&fileBuff,lexeme,&begin);
		begin++;
	}
	// printf("%s ",getTokenFromId(tokenLex.id,tokenLex.name));
	do{
		if(isEndSymbol(symStack->symbol) || symStack->symbol->isTerminal){
			if(strcmp(symStack->symbol->value,getTokenFromId(tokenLex.id,tokenLex.name))==0){
				pop(&symStack);	
				pTree->token = tokenLex;
				while(pTree->sibling==NULL && pTree!=pTreeHead)
					pTree = pTree->parent;
				pTree = pTree->sibling;
				tokenLex = getToken(fp,&fileBuff,lexeme,&begin);
				// printf("%s ",getTokenFromId(tokenLex.id,tokenLex.name));
				begin++;
				while(tokenLex.id==-1){
					tokenLex = getToken(fp,&fileBuff,lexeme,&begin);
					begin++;
				}
				while(tokenLex.id==3){
					tokenLex = getToken(fp,&fileBuff,lexeme,&begin);
					begin++;
				}

			}
			else{
				printf("1 SYNTACTIC ERROR\n");
				printf("Expected %s \nFound %s\n",symStack->symbol->value,getTokenFromId(tokenLex.id,tokenLex.name));
				printStack(symStack);
				exit(0);
			}
		}else{
			int rowIx = getIndexNumber(symStack->symbol);
			int colIx = getIndexNumber(getSymbolIndex(&symbols,getTokenFromId(tokenLex.id,tokenLex.name)));
			if(ptable[rowIx][colIx].symbol==NULL){
				printf("2 SYNTACTIC ERROR\n");
				printf("No rule for (%s,%s)\n",symStack->symbol->value,getTokenFromId(tokenLex.id,tokenLex.name));
				exit(0);
			}else{
				pop(&symStack);	
				SymbolList *rules = getRuleFromIndex(ptable[rowIx][colIx].symbol,ptable[rowIx][colIx].ruleNo);
				addChildren(&pTree,rules);
				if(isEpsilon(rules->symbol)){
					while(pTree->sibling==NULL && pTree!=pTreeHead)
						pTree = pTree->parent;
					pTree = pTree->sibling;
				}
				pushAll(&symStack,rules);
				// printf("%2d %s",++i,ptable[rowIx][colIx].symbol->value);
				// printf(" ==> ");
				// printSymbolList(rules,NULL);
				// printf("\n");
			}
		}
	}while(tokenLex.id!=0);


}


void saveParseTree(char const *f_name){
	FILE *fp = fopen(f_name,"w");
	if(fp==NULL){
		printf("Invalid source file\n");
        return;
	}

	fprintf(fp,"%-20s%-10s%-20s%-30s%-10s%-30s\n","Lexeme","Line","Token","Parent Node","Leaf","Node");
	printTree(pTreeHead,fp);
	fclose(fp);
}

int main(int argc, char const *argv[])
{
	
	loadData(argv[1],argv[2],argv[3]);
	initParseTable();
	createParseTable();
	saveParseTable(argv[4]);
	createParseTree(argv[5]);
	saveParseTree(argv[6]);
	return 0;
}
