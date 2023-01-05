#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "node_type.h"


extern int yylex();
_YYLVAL yylval;
extern char* yytext;

int tok;

int path = 0;

#define __DEBUG 1

void showPath()
{
#if __DEBUG
	path++;
	printf("path: %d ", path);
	printf("token: %d\n", tok);
#else
	
#endif
}

void showYYTEXT()
{
	printf("oper : %s\n", yytext);
}

void advance()
{
	tok = yylex();
	// printf("tok: %s\n", yytext);
}

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast
{
	int ivalue;
	float fvalue;
	char* svalue;
	node_type nodeType;
	past left;
	past right;
	past if_cond;
	past next;
};

past newAstNode()
{
	past node = malloc(sizeof(ast));
	if(node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past newNum(int value)
{
	past var = newAstNode();
	var -> nodeType = Y_INT;
	var -> ivalue = value;
	//var -> next = next;
	return var;
}

past newExpr(int oper, past left,past right)
{
	past var = newAstNode();
	var->nodeType = BINARY_OPERATOR;
	var->ivalue = oper;
	var->left = left;
	var->right = right;
	return var;
}

//Number: IntConst
past astNumber()
{
	if(tok == num_INT){
		past num = newNum(yylval.int_value);
		advance();
		return num;
	}
	return NULL;
}

/*
   60 Exp: AddExp
*/

past astExp()
{
    return astAddExp();
}

/*
   65 PrimaryExp: Y_LPAR Exp Y_RPAR
   66           | LVal
   67           | num_INT
   68           | num_FLOAT
*/

past astPrimaryExp()
{
	return astNumber();
    // if(tok == Y_LPAR)
    // {
    //     advance();
    //     if(Exp())
    //     {
    //         if(tok == Y_RPAR)
    //         {
    //             return 1;
    //         }
    //     }
    //     return -1;
    // }else if(tok == num_FLOAT || tok == num_INT)
	// {
    //     return tok;
    // }else 
	// 	return LVal();
}

/*
   69 UnaryExp: PrimaryExp
   70         | Y_ID Y_LPAR Y_RPAR
   71         | Y_ID Y_LPAR CallParams Y_RPAR
   72         | Y_ADD UnaryExp
   73         | Y_SUB UnaryExp
   74         | Y_NOT UnaryExp
*/  

past astUnaryExp()
{
	showPath();
    if(tok == Y_ID)
	{
        advance();
        if(tok == Y_LPAR)
        {
            advance();
            if(tok == Y_RPAR)
            {
                advance();
                return 1;
            }else{
                return CallParams();
            }
        }    
    }else if(tok == Y_ADD || tok == Y_SUB || tok == Y_NOT)
    {
        advance();
		past l = astUnaryExp();
		past p = newAstNode();		// create operator node 
		p->left = l;
		p->nodeType = UNARY_OPERATOR;
		return p;
    }
	// return false
}

int UnaryOp()
{
    if(tok == Y_ADD || tok == Y_SUB || tok == Y_NOT)
    {
        advance();
        return tok;
    }else{
        return -1;
    }
}

/*
   75 CallParams: Exp
   76           | Exp Y_COMMA CallParams
   
   FuncRParams → Exp { ',' Exp }
*/

past CallParams()
{
	past r = astExp();
	advance();
	while(tok == Y_COMMA)
	{
		advance();
		past l = astExp();
		
	}

    int Exp_cnt = 0, Comma_cnt = 0;
    while(1)
    {

        Exp_cnt++;
        if(tok == ',')
        {
            advance();
            Comma_cnt++;
        }
    }
}

/*
   77 MulExp: UnaryExp
   78       | MulExp Y_MUL UnaryExp
   79       | MulExp Y_DIV UnaryExp
   80       | MulExp Y_MODULO UnaryExp
*/

past astMulExp()
{
	showPath();
    past l = astUnaryExp();
    while(tok == Y_MUL || tok == Y_DIV || tok == Y_MODULO)
    {
        int oper = tok;
		showYYTEXT();
        advance();
        past r = astUnaryExp();
        // if(oper == Y_MUL)
        // {
        //     l = l * r;
        // }else if(oper == Y_DIV){
        //     l = l / r;
        // }else{
        //     l = l % r;
        // }
		l = newExpr(oper, l, r);
	}
    return l;
}

/*
   81 AddExp: MulExp
   82       | AddExp Y_ADD MulExp
   83       | AddExp Y_SUB MulExp
*/

past astAddExp()
{
	showPath();
    past l = astMulExp();
    while(tok == Y_ADD || tok == Y_SUB)
    {
        int oper = tok;
		showYYTEXT();
        advance();
        past r = astMulExp();
        // if(tok == Y_ADD)
        // {
        //     l = l + r;
        // }else{
        //     l = l - r;
        // }
		l = newExpr(oper, r, l);
    }
    return l;
}

/*
   84 RelExp: AddExp
   85       | AddExp Y_LESS RelExp
   86       | AddExp Y_GREAT RelExp
   87       | AddExp Y_LESSEQ RelExp
   88       | AddExp Y_GREATEQ RelExp
*/

past astRelExp()
{
	showPath();
    past l = astAddExp();
    while(tok == Y_LESS || tok == Y_GREAT || tok == Y_LESSEQ || tok == Y_GREATEQ)
    {
        int oper = tok;
		showYYTEXT();
        advance();
        past r = astAddExp();
        /*switch(tok){
        case Y_LESS:    l = l < r; break;
        case Y_GREAT:   l = l > r; break;
        case Y_LESSEQ:  l = l <= r; break;
        case Y_GREATEQ: l = l >= r; break;
        }*/
		l = newExpr(oper, r, l);
    }
}

/*
   89 EqExp: RelExp
   90      | RelExp Y_EQ EqExp
   91      | RelExp Y_NOTEQ EqExp
*/

past astEqExp()
{
	showPath();
    past l = astRelExp();
    while(tok == Y_EQ || tok == Y_NOTEQ)
    {
        int oper = tok;
		showYYTEXT();
        advance();
        past r = astRelExp();
		l = newExpr(oper, l, r);
    }
    return l;
}
/*
   92 LAndExp: EqExp
   93        | EqExp Y_AND LAndExp
*/
past astLAndExp()
{
	showPath();
    past l = astEqExp();
    while(tok == Y_AND)
    {
        int oper = tok;
		showYYTEXT();
        advance();
        past r = astEqExp();
		l = newExpr(oper, l, r);
    }
    return l;
}

/*
   94 LOrExp: LAndExp
   95       | LAndExp Y_OR LOrExp
*/

past astLOrExp()
{
	showPath();
    past l = astLAndExp();
    while(tok == Y_OR)
    {
        int oper = tok;
		showYYTEXT();
        advance();
        past r = astLAndExp();
        l = newExpr(oper, l, r);
    }
    return l;
}

/*
   96 ConstExp: AddExp
*/

past astConstExp()
{
	showPath();
	past a = astAddExp();
    return a;
}

/*
   97 Type: Y_INT
   98     | Y_FLOAT
   99     | Y_VOID

*/

int main(int argc, char **argv)
{
	while(1)
	{
		path = 0;
		printf("input expression, 'q' to exit:\n");
		advance();
		past r = astLOrExp();
	}
	return 0;
}
