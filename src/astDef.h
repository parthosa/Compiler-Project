#ifndef ASTDEF_H_INCLUDED
#define ASTDEF_H_INCLUDED

#include "langDef.h"
#include "parser.h"

#define LABEL_LEN 15

typedef struct ASTNode ASTNode;

struct ASTNode {
	char label[LABEL_LEN];
	// SymbolDef * symbol;
	TokenInfo token;
	struct ASTNode *firstChild;
	struct ASTNode *sibling;
	struct ASTNode *parent;
	struct ASTNode *inh;
};

ASTNode *ASTHead;

#endif
