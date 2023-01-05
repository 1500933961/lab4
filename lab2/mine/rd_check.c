#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tok.h"

extern int yylex();
extern int yylval;
extern char* yytext;

int factor();
int expr();
int term();

int tok;

void advance()
{
    tok = yylex();
    printf("tok: %s\n", yytext);
}


/*  
    1 CompUnit: Decl CompUnit
    2         | FuncDef CompUnit
    3         | Decl
    4         | FuncDef
*/
int CompUnit()
{
    int l = 0;

}


/*
    5 Decl: ConstDecl
    6     | VarDecl
*/

int Decl()
{
    if(tok == Y_CONST)
    {
        return ConstDecl();
    }else {
        return VarDecl();
    }
}

/*
    7 ConstDecl: Y_CONST Type ConstDef Y_SEMICOLON
    8          | Y_CONST Type ConstDefs Y_SEMICOLON
    
    ConstDecl: Y_CONST Type ConstDef {Y_COMMA ConstDef} Y_SEMICOLON
*/

int ConstDecl()
{
    if(tok == Y_CONST)
    {
        advance();
        if(tok == Y_INT || tok == Y_FLOAT)
        {
            if(ConstDef())
            {

            }
        }
    }
}

/*
    9 ConstDefs: ConstDef Y_COMMA ConstDef
   10          | ConstDefs Y_COMMA ConstDef
*/

int ConstDefs()
{

}

/*
   11 ConstDef: Y_ID Y_ASSIGN ConstInitVal
   12         | Y_ID ConstExps Y_ASSIGN ConstInitVal

   13 ConstExps: Y_LSQUARE ConstExp Y_RSQUARE
   14          | Y_LSQUARE ConstExp Y_RSQUARE ConstExps

   15 ConstInitVal: ConstExp
   16             | Y_LBRACKET Y_RBRACKET
   17             | Y_LBRACKET ConstInitVal Y_RBRACKET
   18             | Y_LBRACKET ConstInitVal ConstInitVals Y_RBRACKET

   19 ConstInitVals: Y_COMMA ConstInitVal
   20              | Y_COMMA ConstInitVal ConstInitVals

   21 VarDecl: Type VarDef Y_SEMICOLON
   22        | Type VarDef VarDecls Y_SEMICOLON

   23 VarDecls: Y_COMMA VarDef
   24         | Y_COMMA VarDef VarDecls

   25 VarDef: Y_ID
   26       | Y_ID Y_ASSIGN InitVal
   27       | Y_ID ConstExps
   28       | Y_ID ConstExps Y_ASSIGN InitVal

   29 InitVal: Exp
   30        | Y_LBRACKET Y_RBRACKET
   31        | Y_LBRACKET InitVal Y_RBRACKET
   32        | Y_LBRACKET InitVal InitVals Y_RBRACKET

   33 InitVals: Y_COMMA InitVal
   34         | Y_COMMA InitVal InitVals

   35 FuncDef: Type Y_ID Y_LPAR Y_RPAR Block
   36        | Type Y_ID Y_LPAR FuncParams Y_RPAR Block

   37 FuncParams: FuncParam
   38           | FuncParams Y_COMMA FuncParam

   39 FuncParam: Type Y_ID
   40          | Type Y_ID Y_LSQUARE Y_RSQUARE
   41          | Type Y_ID ArraySubscripts
   42          | Type Y_ID Y_LSQUARE Y_RSQUARE ArraySubscripts
*/

/*
   43 Block: Y_LBRACKET BlockItems Y_RBRACKET
   44      | Y_LBRACKET Y_RBRACKET
*/

int BLock()
{
    if(tok == Y_LBRACKET)
    {
        advance();
        if(tok == Y_RBRACKET)
        {
            return 1;
        }else{
            BlockItems();
            if(tok == Y_RBRACKET)
            return 1;
        }
    }
    return -1;
}


/*
   45 BlockItems: BlockItem
   46           | BlockItem BlockItems
*/

int BlockItems()
{
    while()
}

/*
   47 BlockItem: Decl
   48          | Stmt
*/

int BlockItem()
{
    if(tok == Y_CONST || tok == Y_INT)
    {
        return Decl();
    }else{
        return Stmt();
    }
}

/*
   49 Stmt: LVal Y_ASSIGN Exp Y_SEMICOLON
   50     | Y_SEMICOLON
   51     | Exp Y_SEMICOLON
   52     | Block
   53     | Y_WHILE Y_LPAR LOrExp Y_RPAR Stmt
   54     | Y_IF Y_LPAR LOrExp Y_RPAR Stmt
   55     | Y_IF Y_LPAR LOrExp Y_RPAR Stmt Y_ELSE Stmt
   56     | Y_BREAK Y_SEMICOLON
   57     | Y_CONTINUE Y_SEMICOLON
   58     | Y_RETURN Exp Y_SEMICOLON
   59     | Y_RETURN Y_SEMICOLON
*/

int Stmt()
{
    if(tok == Y_WHILE)    // while ( L0rExo ) stmt
    {
        advance();
        if(tok == Y_LPAR)
        {
            advance();
            if(L0rExp())
            {
                if(tok == Y_LPAR)
                {
                    advance();
                    if(Stmt())
                    {
                        return 1;
                    }
                }
            }
        }
    }else if(tok = Y_IF)  // if ( L0rExp ) Stmt else Stmt
    {
        advance();
        if(tok == Y_LPAR)
        {
            advance();
            if(L0rExp())
            {
                advance();
                if(tok == Y_RPAR)
                {
                    if(Stmt())
                    {
                        if(tok == Y_ELSE)
                        {
                            advance();
                            if(!Stmt()) return -1;
                        }
                        return 1;
                    }
                }
            }
        }
    }else if(tok == Y_BREAK)  // break ;
    {
        advance();
        if(tok == Y_SEMICOLON)
        {
            advance();
            return 1;
        }
    }else if(tok == Y_CONTINUE)   // continue ;
    {
        advance();
        if(tok == Y_CONTINUE)
        {
            advance();
            return 1;
        }
    }else if(tok == Y_RETURN) // return [Exp] ;
    {
        advance();
        if(tok == Y_SEMICOLON)
        {
            return 1;
        }
        if(Exp())
        {
            if(tok == Y_SEMICOLON)
            {
                return 1;
            }
        }
    }else if(tok == Y_LBRACKET)   // { }
    {
        return Block();
    }else if(tok == Y_ID)     // identifier = Exp ;
    {
        if(LVal())
        {
            if(tok == Y_EQU)
            {
                advance();
                if(Exp())
                {
                    if(tok == Y_SEMICOLON)
                    {
                        return 1;
                    }
                }
            }
        }
    }else if(Exp())
    {
        if(tok == Y_SEMICOLON)
        {
            advance();
            return 1;
        }
    }else if(tok == Y_SEMICOLON)
    {
        return 1;
    }
    return -1;
}

/*
   60 Exp: AddExp
*/

int Exp()
{
    return AddExp();
}

/*
   61 LVal: Y_ID
   62     | Y_ID ArraySubscripts
*/

int LVal()
{
    if(tok == Y_ID)
    {
        advance();
        while(tok == Y_LSQUARE)
        {
            advance();
            int l = Exp();
            if(tok == Y_LSQUARE)
            {
                advance();
                return 1;
            }
        }
        return tok;
    }
    return -1;
}

/*
   63 ArraySubscripts: Y_LSQUARE Exp Y_RSQUARE
   64                | Y_LSQUARE Exp Y_RSQUARE ArraySubscripts
*/


int ArraySubscripts()
{
    if(tok == Y_LSQUARE)
    {
        advance();
        if(Exp())
        {
            if(tok == Y_RSQUARE)
            {
                advance();
                if(tok == Y_LSQUARE)
                {
                    advance();
                    return ArraySubscripts();
                }
                return 1;
            }
        }
    }
    return -1;
}


/*
   65 PrimaryExp: Y_LPAR Exp Y_RPAR
   66           | LVal
   67           | num_INT
   68           | num_FLOAT
*/

int PrimaryExp()
{
    if(tok == Y_LPAR)
    {
        advance();
        if(Exp())
        {
            if(tok == Y_RPAR)
            {
                return 1;
            }
        }
        return -1;
    }else if(tok == num_FLOAT)
    {
        return tok;
    }else if(tok == num_INT)
    {
        return tok;
    }else return LVal();
}

/*
   69 UnaryExp: PrimaryExp
   70         | Y_ID Y_LPAR Y_RPAR
   71         | Y_ID Y_LPAR CallParams Y_RPAR
   72         | Y_ADD UnaryExp
   73         | Y_SUB UnaryExp
   74         | Y_NOT UnaryExp
*/  

int UnaryExp()
{
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
        return UnaryExp();
    }
    return -1;
}

int Unaryop()
{
    if(tok == '+' || tok == '-' || tok == '!')
    {
        advance();
        return tok;
    }else{
        return = -1;
    }
}

/*
   75 CallParams: Exp
   76           | Exp Y_COMMA CallParams
*/

int CallParams()
{
    int Exp_cnt = 0, Comma_cnt = 0;
    while(Exp())
    {
        Exp_cnt++;
        if(tok == ',')
        {
            advance();
            Comma_cnt++;
        }
    }
    return (Exp_cnt - Comma_cnt) == 1 ? 1 : 0;
}

/*
   77 MulExp: UnaryExp
   78       | MulExp Y_MUL UnaryExp
   79       | MulExp Y_DIV UnaryExp
   80       | MulExp Y_MODULO UnaryExp
*/

int MulExp()
{
    int l = tok;
    while(tok == Y_MUL || tok == Y_DIV || tok == Y_MODULO)
    {
        int oper = tok;
        advance();
        int r = UnaryExp();
        if(oper == Y_MUL)
        {
            l = l * r;
        }else if(oper == Y_DIV){
            l = l / r;
        }else{
            l = l % r;
        }
        return l;
    }
}

/*
   81 AddExp: MulExp
   82       | AddExp Y_ADD MulExp
   83       | AddExp Y_SUB MulExp
*/
int AddExp()
{
    int l = tok;
    while(tok == Y_ADD || tok == Y_SUB)
    {
        int oper = tok;
        advance();
        int r = MulExp();
        if(tok == Y_ADD)
        {
            l = l + r;
        }else{
            l = l - r;
        }
        return l;
    }
}
/*
   84 RelExp: AddExp
   85       | AddExp Y_LESS RelExp
   86       | AddExp Y_GREAT RelExp
   87       | AddExp Y_LESSEQ RelExp
   88       | AddExp Y_GREATEQ RelExp
*/
int RelExp()
{
    int l = tok;
    while(tok == Y_LESS || tok == Y_GREAT || tok == Y_LESSEQ || tok == Y_GREATEQ)
    {
        int oper = tok;
        advance();
        int r = AddExp();
        switch(tok){
        case Y_LESS:    l = l < r; break;
        case Y_GREAT:   l = l > r; break;
        case Y_LESSEQ:  l = l <= r; break;
        case Y_GREATEQ: l = l >= r; break;
        }
    }
}
/*
   89 EqExp: RelExp
   90      | RelExp Y_EQ EqExp
   91      | RelExp Y_NOTEQ EqExp
*/
int EqExp()
{
    int l = tok;
    while(tok == Y_EQ || tok == Y_NOTEQ)
    {
        int oper = tok;
        advance();
        int r = RelExp();
        if(tok == Y_EQ)
        {
            l = l == q;
        }else{
            l = l!=q;
        }        
    }
    return l;
}
/*
   92 LAndExp: EqExp
   93        | EqExp Y_AND LAndExp
*/
int LAndExp()
{
    int l = EqExp();
    while(tok == Y_AND)
    {
        int oper = tok;
        advance();
        int r = EqExp();
        if(oper == Y_AND)
        {
            l = l && r;
        }
    }
    return l;
}

/*
   94 LOrExp: LAndExp
   95       | LAndExp Y_OR LOrExp
*/
int L0rExp()
{
    int l = LAndExp();
    while(tok == Y_OR)
    {
        int oper = tok;
        advance();
        int r = LAndExp();
        if(oper == Y_OR)
        {
            l = l || r;
        }
        //l = 1;
    }
    return l;
}

/*
   96 ConstExp: AddExp
*/
int ConstExp()
{
    return AddExp();
}


/*
   97 Type: Y_INT
   98     | Y_FLOAT
   99     | Y_VOID

*/