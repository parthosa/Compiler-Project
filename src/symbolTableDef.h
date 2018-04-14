
#ifndef SYMBOLTABLEDEF_H_INCLUDED
#define SYMBOLTABLEDEF_H_INCLUDED

#include "langDef.h"


struct matrix{
	int rows;
	int cols;
};

struct string{
	int length;
};


union initVal{
	int num;
	float rnum;
	char *str;
};


typedef struct SymbolTableEntry SymbolTableEntry;

struct SymbolTableEntry{
	char value[MAX_LENGTH];
	char type[10];

	union{
		struct string str;
		struct matrix mat;
	} info;

	int width;
	int offset;
	int lineNo;
	int isInit;

	// union initVal val;
	struct SymbolTableEntry *next;
	struct SymbolTableEntry *prev;
	 
};


typedef struct SymbolTable SymbolTable;

struct SymbolTable{
	int scopeLevel;

	SymbolTableEntry *symbolsHead;

	struct SymbolTable *parent;
	struct SymbolTable *firstChild;
	struct SymbolTable *sibling;
};


SymbolTable *symTable;
#endif

