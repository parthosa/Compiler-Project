#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// #include "symbolDef.h"

#include "parser.h"
// #include "lexer.h"
#include "ast.h"



ASTNode *newASTNode(char label[LABEL_LEN],TokenInfo token){
    ASTNode *newNode = (ASTNode*) malloc(sizeof(ASTNode));
    // memset(&(newNode->label), '\0', sizeof(newNode->label));
    strcpy(newNode->label,label);
    newNode->token = token;
    newNode->firstChild = NULL;
    newNode->parent = NULL;
    newNode->sibling = NULL;
    newNode->inh = NULL;
    return newNode;
}


void setParent(ASTNode *parent,ASTNode *children){
    ASTNode *temp = children;
    while(temp!=NULL){
        temp->parent = parent;
        temp = temp->sibling;
    }
}

void printTabs(int c){
    while(c-- > 0){
        printf("\t");
    }
}

void printAST(ASTNode *root,int level){
    if(root==NULL)
        return;

    printTabs(level);
    printf("%s\n",root->label);

    ASTNode *child = root->firstChild;
    while(child!=NULL){
        printAST(child,level+1);
        child=child->sibling;
    }
        
    return;
}



ParseTree *getNode(ParseTree *children,char value[30],int nthCount){
    ParseTree *temp = children;
    int count = 1;
    while(temp!=NULL){
        if(strcmp(temp->symbol->value,value)==0){
            if(count == nthCount)
                return temp;
            else
                count++;
        }
        temp = temp->sibling;
    }
    return NULL;
}

ASTNode *buildAST(ParseTree *root){
    
    if(root==NULL)
        return NULL;

    if(root->symbol->isTerminal==1){
        if(isEpsilon(root->symbol)){
            return NULL;
        }
        else{
            ASTNode *newNode = newASTNode(root->symbol->value,root->token);
            return newNode;
        }
    }

    if(strcmp(root->symbol->value,"<mainFunction>")==0){
        ASTNode *stmtsAndFunctionDefs = buildAST(getNode(root->firstChild,"<stmtsAndFunctionDefs>",1));
        
        ASTNode *mainFunction = newASTNode("MAINFUNCTION",root->token);
        mainFunction->firstChild = stmtsAndFunctionDefs;
        setParent(mainFunction,stmtsAndFunctionDefs);
        return mainFunction;
    }

    if(strcmp(root->symbol->value,"<stmtsAndFunctionDefs>")==0){
        ASTNode *stmtOrFunctionDef = buildAST(root->firstChild);
        ASTNode *stmtsAndFunctionDefsOrEmpty = buildAST(root->firstChild->sibling);
        stmtOrFunctionDef->sibling = stmtsAndFunctionDefsOrEmpty;

        return stmtOrFunctionDef;
    }

    if(strcmp(root->symbol->value,"<stmtsAndFunctionDefsOrEmpty>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL;

        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<stmtOrFunctionDef>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<stmt>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<otherStmts>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL;

        ASTNode *stmt = buildAST(root->firstChild);
        ASTNode *otherStmts = buildAST(root->firstChild->sibling);
        stmt->sibling = otherStmts;

        return stmt;
    }

    if(strcmp(root->symbol->value,"<funcStmt>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<declarationStmt>")==0){
        ASTNode *type = buildAST(root->firstChild);
        ASTNode *idList = buildAST(root->firstChild->sibling);
        type->sibling = idList;
        
        ASTNode *declarationStmt = newASTNode("DECLARATION",root->token);
        declarationStmt->firstChild = type;
        setParent(declarationStmt,type);
        return declarationStmt;
    }

    if(strcmp(root->symbol->value,"<assignmentStmt>")==0){
        if(strcmp(root->firstChild->symbol->value,"ID")==0){
            ASTNode *id = buildAST(root->firstChild);
            ASTNode *rightHandSideOfId = buildAST(getNode(root->firstChild,"<rightHandSideOfId>",1));
            id->sibling = rightHandSideOfId;
            
            ASTNode *assignmentStmt = newASTNode("ASSIGNOP",root->token);
            assignmentStmt->firstChild = id;
            setParent(assignmentStmt,id);
            return assignmentStmt;
        }else{
            ASTNode *ids = buildAST(getNode(root->firstChild,"<idList>",1));

            ASTNode *idList = newASTNode("IDLIST",root->token);
            idList->firstChild = ids;
            setParent(idList,ids);

            ASTNode *rightHandSideOfTuple = buildAST(getNode(root->firstChild,"<rightHandSideOfTuple>",1));
            idList->sibling = rightHandSideOfTuple;
            
            ASTNode *assignmentStmt = newASTNode("ASSIGNOPTUPLE",root->token);
            assignmentStmt->firstChild = idList;
            setParent(assignmentStmt,idList);
            return assignmentStmt;
        }
    }


    if(strcmp(root->symbol->value,"<rightHandSideOfId>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<rightHandSideOfTuple>")==0){
        if(strcmp(root->firstChild->symbol->value,"SIZE")==0){
            ASTNode *id = buildAST(root->firstChild->sibling);

            ASTNode *size = newASTNode("SIZE",root->token);
            size->firstChild = id;
            setParent(size,id);
            return size;
        }
        else
            return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<conditionalStmt>")==0){
        ASTNode *booleanExpression = buildAST(getNode(root->firstChild,"<booleanExpression>",1));
        
        ASTNode *stmt = buildAST(getNode(root->firstChild,"<stmt>",1));
        ASTNode *otherStmts = buildAST(getNode(root->firstChild,"<otherStmts>",1));
        stmt->sibling = otherStmts;

        ASTNode *ifStmts = newASTNode("IFSTMTS",root->token);
        ifStmts->firstChild = stmt;
        setParent(ifStmts,stmt);

        ASTNode *elseStmt = buildAST(getNode(root->firstChild,"<elseStmt>",1));

        booleanExpression->sibling = ifStmts;
        ifStmts->sibling = elseStmt;

        ASTNode *ifstatement = newASTNode("IFSTATEMENT",root->token);
        ifstatement->firstChild = booleanExpression;
        setParent(ifstatement,booleanExpression);

        return ifstatement;
    }

    if(strcmp(root->symbol->value,"<elseStmt>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL;
        
        ASTNode *stmt = buildAST(getNode(root->firstChild,"<stmt>",1));
        ASTNode *otherStmts = buildAST(getNode(root->firstChild,"<otherStmts>",1));
        stmt->sibling = otherStmts;

        ASTNode *elseStmt = newASTNode("ELSESTMT",root->token);
        elseStmt->firstChild = stmt;
        setParent(elseStmt,stmt);

        return elseStmt;
    }

    if(strcmp(root->symbol->value,"<ioStmt>")==0){
        ASTNode *iotype = buildAST(root->firstChild);
        ASTNode *id = buildAST(getNode(root->firstChild,"ID",1));
        iotype->sibling = id;

        ASTNode *ioStmt = newASTNode("IOSTATEMENT",root->token);
        ioStmt->firstChild = iotype;
        setParent(ioStmt,iotype);
        return ioStmt;
    }


    if(strcmp(root->symbol->value,"<arithmeticExpression>")==0){
        ASTNode *term = buildAST(root->firstChild);

        ParseTree *arithmeticExpressionOpsPNode = root->firstChild->sibling;
        if(isEpsilon(arithmeticExpressionOpsPNode->firstChild->symbol)){
            return term; //<arithmeticExpressionOps>.inh_addr
        }else{
            ASTNode *plusOrMinus = buildAST(arithmeticExpressionOpsPNode->firstChild);
            ASTNode *arithmeticExpression = buildAST(arithmeticExpressionOpsPNode->firstChild->sibling);
            plusOrMinus->sibling = term; // <arithmeticExpressionOps>.inh_addr
            term->sibling = arithmeticExpression;

            ASTNode *arithmeticExpressionNode = newASTNode("ARITHMETICEXP1",arithmeticExpressionOpsPNode->token);
            arithmeticExpressionNode->firstChild = plusOrMinus;
            setParent(arithmeticExpressionNode,plusOrMinus);
            return arithmeticExpressionNode;

        }

    }

    if(strcmp(root->symbol->value,"<term>")==0){
        ASTNode *factor = buildAST(root->firstChild);

        ParseTree *termOpsPNode = root->firstChild->sibling;
        if(isEpsilon(termOpsPNode->firstChild->symbol)){
            return factor; //<termOps>.inh_addr
        }else{
            ASTNode *mulOrDiv = buildAST(termOpsPNode->firstChild);
            ASTNode *term = buildAST(termOpsPNode->firstChild->sibling);
            mulOrDiv->sibling = factor; // <termOps>.inh_addr
            factor->sibling = term;

            ASTNode *termNode = newASTNode("ARITHMETICEXP2",termOpsPNode->token);
            termNode->firstChild = mulOrDiv;
            setParent(termNode,mulOrDiv);
            return termNode;

        }
    }


    if(strcmp(root->symbol->value,"<factor>")==0){
        if(strcmp(root->firstChild->symbol->value,"OP") == 0){
            return buildAST(getNode(root->firstChild,"<arithmeticExpression>",1));
        }
        else
            return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<plusOrMinus>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<mulOrDiv>")==0){
        return buildAST(root->firstChild);
    }


    if(strcmp(root->symbol->value,"<booleanExpression>")==0){
        if(strcmp(root->firstChild->symbol->value,"OP")==0){
            ASTNode *binaryLogicalOp = buildAST(getNode(root->firstChild,"<binaryLogicalOp>",1));
            ASTNode *booleanExpression1 = buildAST(getNode(root->firstChild,"<booleanExpression>",1));
            ASTNode *booleanExpression2 = buildAST(getNode(root->firstChild,"<booleanExpression>",2));

            binaryLogicalOp->sibling = booleanExpression1;
            booleanExpression1->sibling = booleanExpression2;

            ASTNode *booleanexp = newASTNode("BOOLEANEXP",root->token);
            booleanexp->firstChild = binaryLogicalOp;
            setParent(booleanexp,binaryLogicalOp);
            return booleanexp;

        }
        else if(strcmp(root->firstChild->symbol->value,"NOT")==0){
            ASTNode *notOp = buildAST(root->firstChild);
            ASTNode *booleanExpression = buildAST(getNode(root->firstChild,"<booleanExpression>",1));

            notOp->sibling = booleanExpression;

            ASTNode *booleanexp = newASTNode("BOOLEANEXP",root->token);
            booleanexp->firstChild = notOp;
            setParent(booleanexp,notOp);
            return booleanexp;
        }
        else{
            ASTNode *relationalOp = buildAST(getNode(root->firstChild,"<relationalOp>",1));
            ASTNode *constrainedVars1 = buildAST(getNode(root->firstChild,"<constrainedVars>",1));
            ASTNode *constrainedVars2 = buildAST(getNode(root->firstChild,"<constrainedVars>",2));

            relationalOp->sibling = constrainedVars1;
            constrainedVars1->sibling = constrainedVars2;

            ASTNode *booleanexp = newASTNode("BOOLEANEXP",root->token);
            booleanexp->firstChild = relationalOp;
            setParent(booleanexp,relationalOp);
            return booleanexp;
        }
    }

    if(strcmp(root->symbol->value,"<binaryLogicalOp>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<relationalOp>")==0){
        return buildAST(root->firstChild);
    }


    if(strcmp(root->symbol->value,"<functionDef>")==0){
        ASTNode *parameterList1 = buildAST(getNode(root->firstChild,"<parameterList>",1));
        ASTNode *funId = buildAST(getNode(root->firstChild,"FUNID",1));
        ASTNode *parameterList2 = buildAST(getNode(root->firstChild,"<parameterList>",2));
        ASTNode *stmtsAndFunctionDefs = buildAST(getNode(root->firstChild,"<stmtsAndFunctionDefs>",1));

        funId->sibling = parameterList1;
        parameterList1->sibling = parameterList2;
        parameterList2->sibling = stmtsAndFunctionDefs;

        ASTNode *functionDef = newASTNode("FUNCTIONDEF",root->token);
        functionDef->firstChild = funId;
        setParent(functionDef,funId);
        return functionDef;

    }

    if(strcmp(root->symbol->value,"<parameterList>")==0){
        if(isEpsilon(root->firstChild->symbol)){
            ASTNode *parameterList = newASTNode("PARAMLIST",root->token);
            return parameterList;
        }
            

        ASTNode *type = buildAST(getNode(root->firstChild,"<type>",1));
        ASTNode *id = buildAST(getNode(root->firstChild,"ID",1));
        type->sibling = id;

        ASTNode *param = newASTNode("PARAM",root->token);
        param->firstChild = type;
        setParent(param,type);

        ASTNode *remainingList = buildAST(getNode(root->firstChild,"<remainingList>",1));
        param->sibling = remainingList;

        ASTNode *parameterList = newASTNode("PARAMLIST",root->token);
        parameterList->firstChild = param;
        setParent(parameterList,param);
        return parameterList;
    }

    if(strcmp(root->symbol->value,"<remainingList>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL; 

        ASTNode *type = buildAST(getNode(root->firstChild,"<type>",1));
        ASTNode *id = buildAST(getNode(root->firstChild,"ID",1));
        type->sibling = id;

        ASTNode *param = newASTNode("PARAM",root->token);
        param->firstChild = type;
        setParent(param,type);

        ASTNode *remainingList = buildAST(getNode(root->firstChild,"<remainingList>",1));
        param->sibling = remainingList;
        return param;
    }

    if(strcmp(root->symbol->value,"<funCall>")==0){
        ASTNode *funId = buildAST(root->firstChild);
        ASTNode *funIdList = buildAST(getNode(root->firstChild,"<funIdList>",1));
        funId->sibling = funIdList;

        ASTNode *funCall = newASTNode("FUNCALL",root->token);
        funCall->firstChild = funId;
        setParent(funCall,funId);
        return funCall;
    }

    if(strcmp(root->symbol->value,"<funIdList>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<type>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<var>")==0){
        if(strcmp(root->firstChild->symbol->value,"ID")==0){
            ASTNode *id = buildAST(root->firstChild);
            ParseTree *matrixRestPNode = root->firstChild->sibling;
            if(isEpsilon(matrixRestPNode->firstChild->symbol))
                return id;

            ASTNode *matrixRest = buildAST(matrixRestPNode);
            id->sibling = matrixRest;

            ASTNode *matrixele = newASTNode("MATRIXELE",root->token);
            matrixele->firstChild = id;
            setParent(matrixele,id);
            return matrixele;

        }
        if(strcmp(root->firstChild->symbol->value,"SIZE")==0){
            ASTNode *id = buildAST(root->firstChild->sibling);

            ASTNode *size = newASTNode("SIZE",root->token);
            size->firstChild = id;
            setParent(size,id);
            return size;
        }
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<constrainedVars>")==0){
        return buildAST(root->firstChild);
    }

    if(strcmp(root->symbol->value,"<matrixRest>")==0){
        ASTNode *num1 = buildAST(getNode(root->firstChild,"NUM",1));
        ASTNode *num2 = buildAST(getNode(root->firstChild,"NUM",2));
        num1->sibling = num2;
        return num1;
    }

    if(strcmp(root->symbol->value,"<varList>")==0){
        ASTNode *var = buildAST(root->firstChild);
        ASTNode *moreVars = buildAST(root->firstChild->sibling);
        var->sibling = moreVars;
        return var;
    }

    if(strcmp(root->symbol->value,"<moreVars>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL;
        return buildAST(root->firstChild->sibling);
    }

    if(strcmp(root->symbol->value,"<idList>")==0){
        ASTNode *id = buildAST(root->firstChild);
        ASTNode *moreIds = buildAST(root->firstChild->sibling);
        id->sibling = moreIds;
        return id;
    }

    if(strcmp(root->symbol->value,"<moreIds>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL;
        return buildAST(root->firstChild->sibling);
    }

    if(strcmp(root->symbol->value,"<matrix>")==0){
        ASTNode *numberList = buildAST(getNode(root->firstChild,"<numberList>",1));

        ASTNode *matrixRow = newASTNode("MATRIXROW",root->token);
        matrixRow->firstChild = numberList;
        setParent(matrixRow,numberList);

        ASTNode *moreMatrixNums = buildAST(getNode(root->firstChild,"<moreMatrixNums>",1));
        matrixRow->sibling = moreMatrixNums;

        ASTNode *matrix = newASTNode("MATRIX",root->token);
        matrix->firstChild = matrixRow;
        setParent(matrix,matrixRow);
        return matrix;
    }

    if(strcmp(root->symbol->value,"<numberList>")==0){
        ASTNode *num = buildAST(root->firstChild);
        ASTNode *moreNums = buildAST(root->firstChild->sibling);
        num->sibling = moreNums;
        return num;
    }

    if(strcmp(root->symbol->value,"<moreNums>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL;
        return buildAST(root->firstChild->sibling);
    }

    if(strcmp(root->symbol->value,"<moreMatrixNums>")==0){
        if(isEpsilon(root->firstChild->symbol))
            return NULL;

        ASTNode *numberList = buildAST(getNode(root->firstChild,"<numberList>",1));

        ASTNode *matrixRow = newASTNode("MATRIXROW",root->token);
        matrixRow->firstChild = numberList;
        setParent(matrixRow,numberList);

        ASTNode *moreMatrixNums = buildAST(getNode(root->firstChild,"<moreMatrixNums>",1));
        matrixRow->sibling = moreMatrixNums;

        return matrixRow;
    }

    return NULL;
}