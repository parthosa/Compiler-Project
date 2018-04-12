/*
*	NAME: PARTHO SARTHI
*	  ID: 2015A7PS0088P
*/

#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


#include "symbolDef.h"
#include "firstAndFollow.h"

/*
* Iterative function to calculate the first and follow sets
*/
void findFirstAndFollow(){

	SymbolList * temp = symbols;
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



}

void saveFirstAndFollow(char const *first_nm,char const *follow_nm){

	FILE *fptr = fopen(first_nm,"w"),*fptr2 = fopen(follow_nm,"w");

	SymbolList * temp = symbols;
	while(temp!=NULL){
		if(temp->symbol->isTerminal==0){
			fprintf(fptr,"%s --> ",temp->symbol->value);
			fprintf(fptr2,"%s --> ",temp->symbol->value);
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


// int main(int argc, char const *argv[])
// {
// 	if(argc < 2){
// 		printf("ERROR: Grammar file name not provided\n");
// 		exit(0);
// 	}
// 	if(argc < 4){
// 		printf("ERROR: Output files names not provided\n");
// 		exit(0);
// 	}

// 	loadGrammar(argv[1]);
// 	findFirstAndFollow();
// 	saveFirstAndFollow(argv[2],argv[3]);
// 	return 0;
// }