#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "astDef.h"


ASTNode *buildAST(ParseTree *root);
void printAST(ASTNode *root,int level);

// void findASTRec(ParseTree *parsetree);

#endif
