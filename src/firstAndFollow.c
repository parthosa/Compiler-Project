#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SYMBOLS 50

typedef struct {
	int isTerminal;
	char value[100];
	struct RuleList *rules;
	struct SymbolList *first;
	struct SymbolList *follow;
} SymbolDef ;


typedef struct RuleList RuleList;

struct RuleList {
	struct SymbolList * rule_tokens;
	struct RuleList *next;
};


typedef struct SymbolList SymbolList;

struct SymbolList {
	SymbolDef * symbol;
	struct SymbolList *next;
	struct SymbolList *prev;
};

bool isEpsilon(SymbolDef * symbol){
	if(strcmp(symbol->value,"_eps_")==0)
		return true;
	else
		return false;
}

bool isTerminal(SymbolDef * symbol){
	int i=0;
	
	if(isEpsilon(symbol))
		return true;

	while(symbol->value[i]){
		if(!isupper(symbol->value[i]))
			return false;
		i++;
	}
	return true;
}


SymbolList * symbols;

SymbolDef * getSymbolIndex(SymbolList ** symbols,char * sym_string){
	SymbolList * temp = *symbols;
	while(temp!=NULL){
		if(strcmp(temp->symbol->value,sym_string)==0)
			return temp->symbol;
		temp = temp->next;
	}
	return NULL;
}

SymbolDef * insertSymbol(SymbolList ** symbols,SymbolDef *new_symbol){
	SymbolList *new_symbol_ls  = (SymbolList*) malloc(sizeof(SymbolList));
	SymbolList *last = *symbols;
	new_symbol_ls->symbol = new_symbol;
	new_symbol_ls->next = NULL;
	new_symbol_ls->prev = NULL;

	if (*symbols == NULL)
    {
       *symbols = new_symbol_ls;
       return new_symbol;
    } 

    
    while (last->next != NULL){
        last = last->next;
    }
    last->next = new_symbol_ls;
    new_symbol_ls->prev = last;
	return new_symbol;
}

SymbolDef *  insertSymbolFromToken(SymbolList ** symbols,char * leftSide){
	SymbolDef* new_symbol = (SymbolDef*) malloc(sizeof(SymbolDef));
	strcpy(new_symbol->value,leftSide);
	if(isTerminal(new_symbol)){
		new_symbol->isTerminal=1;
	}
	else
		new_symbol->isTerminal=0;
	new_symbol->rules = NULL;
	new_symbol->first = NULL;
	new_symbol->follow = NULL;

	return insertSymbol(symbols,new_symbol);
	
}

void insertRule(SymbolDef **symbol,char * rightSide){
	RuleList* new_rule  = (RuleList*) malloc(sizeof(RuleList));
	RuleList *last = (*symbol)->rules;
	char * token = strtok(rightSide , " ");

	// printf("u of: %s rhs: %s\n",(*symbol)->value,rightSide);
    while(token!=NULL){
    	SymbolDef * symbol2 = getSymbolIndex(&symbols,token);
		if(symbol2==NULL){
			symbol2 = insertSymbolFromToken(&symbols,token);
		}
        insertSymbol(&new_rule->rule_tokens,symbol2);
        token = strtok(NULL, " ");
    }

	new_rule->next = NULL;

	if ((*symbol)->rules == NULL)
    {
       (*symbol)->rules = new_rule;
       return;
    } 

    while (last->next != NULL)
        last = last->next;

    last->next = new_rule;
}


void parseGrammar(char const *f_name){
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


int sizeOfList(SymbolList * symbols){
	SymbolList * temp = symbols;
	int count=0;
	while(temp!=NULL){
		count++;
		temp=temp->next;
	}
	return count;
}

int mergeList(SymbolList ** first_list, SymbolList * first,int includeEps){
	SymbolList * temp = first;
	int init_sz = sizeOfList(*first_list);
	while(temp!=NULL){
		if(includeEps == 0 && isEpsilon(temp->symbol)){
			temp=temp->next;
			continue;
		}
		if(getSymbolIndex(first_list,temp->symbol->value)==NULL)
			insertSymbol(first_list,temp->symbol);
		temp=temp->next;
	}
	int final_sz = sizeOfList(*first_list);
	return final_sz != init_sz;
}

bool hasEpsilon(SymbolList * symbols){
	SymbolList * temp = symbols;
	
	while(temp!=NULL){
		if(isEpsilon(temp->symbol)){
			return true;
		}
		temp=temp->next;
	}
	return false;
}

int allEpsilon(SymbolList * begin,SymbolList * end){
	SymbolList * temp = begin;
	int flg;
	while(temp!=end){
		flg = 0;
		if(temp->symbol->isTerminal==0){
			RuleList * temp2 = temp->symbol->rules;
			while(temp2!=NULL){
				if(hasEpsilon(temp2->rule_tokens)){
					flg=1;
				}
				temp2=temp2->next;
			}
		}else if(!isEpsilon(temp->symbol))
			flg = 0;
		if(flg==0)
			return 0;
		temp=temp->next;
	}
	return 1;
}



void findFollow(char const *first_nm,char const *follow_nm){

	SymbolList * temp;
	temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==1){
			SymbolList *temp2 = NULL;
			insertSymbol(&temp2,temp->symbol);
			mergeList(&temp->symbol->first,temp2,1);
		}
		temp=temp->next;
	}

	while(1){
		int updated = 0;
		temp = symbols;
		while(temp!=NULL){
			if(temp->symbol->isTerminal==0){
				RuleList * temp2 = temp->symbol->rules;
				while(temp2!=NULL){
					SymbolList * temp3 = temp2->rule_tokens;
					while(temp3!=NULL){
						if(temp3 == temp2->rule_tokens || allEpsilon(temp2->rule_tokens,temp3))
							updated |= mergeList(&temp->symbol->first,temp3->symbol->first,1);

						if(temp3->symbol->isTerminal==0){
							SymbolList * temp4 = temp3->next;
							if(temp3->next==NULL)
								updated |= mergeList(&temp3->symbol->follow,temp->symbol->follow,0);
							while(temp4!=NULL){
								if(temp3->next==NULL || allEpsilon(temp3->next,NULL)) 
									updated |= mergeList(&temp3->symbol->follow,temp->symbol->follow,0);
								if(temp4==temp3->next || allEpsilon(temp3->next,temp4))
									updated |= mergeList(&temp3->symbol->follow,temp4->symbol->first,0);
								temp4=temp4->next;
							}
						}
						temp3=temp3->next;
					}
					temp2=temp2->next;
				}
			}
			temp=temp->next;
		}
		if(!updated)
			break;
	}


	FILE *fptr = fopen(first_nm,"w"),*fptr2 = fopen(follow_nm,"w");

	temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==0){
			fprintf(fptr,"%s -> ",temp->symbol->value);
			fprintf(fptr2,"%s -> ",temp->symbol->value);
			SymbolList * temp2 = temp->symbol->first;
			while(temp2!=NULL){
				fprintf(fptr,"%s, ",temp2->symbol->value);
				temp2=temp2->next;
			}
			fprintf(fptr,"\n");

			temp2 = temp->symbol->follow;
			while(temp2!=NULL){
				fprintf(fptr2,"%s, ",temp2->symbol->value);
				temp2=temp2->next;
			}
			fprintf(fptr2,"\n");
		}
		temp=temp->next;
	}
	fclose(fptr);
	fclose(fptr2);


}

int main(int argc, char const *argv[])
{
	if(argc < 2){
		printf("ERROR: Grammar file name not provided\n");
		exit(0);
	}
	if(argc < 3){
		printf("ERROR: Output file name not provided\n");
		exit(0);
	}
	parseGrammar(argv[1]);
	// findFirst(argv[2]);
	findFollow(argv[2],argv[3]);
	return 0;
}