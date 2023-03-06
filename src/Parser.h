#include <cstdio>
#include <vector>
#include <map>

#include "Lexer.h"
#include "SyntaxAST.h"


// CurTok - provide a simple token buffer
static int CurTok;
static int getNextToken(){
    return CurTok = gettok();
}

// BinopPrecedence
static std::map<char, int> BinopPrecedence;

// Get the precedence of the pending binary opr
static int GetTokPrecedence(){
    if (!isascii(CurTok))
        return -1;
    
    // Make sure it's a declared binop
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) return -1;
    return TokPrec;
}

// Error info
ExprAST *Error(const char *str) 
{ fprintf(stderr, "Error: %s\n", str);return 0; }
PrototypeAST *ErrorP(const char *str) {Error(str); return 0;}
FunctionAST *ErrorF(const char *str) {Error(str); return 0;}


static ExprAST *ParseExpression();

// numberexpr ::= number
static ExprAST *ParseNumberExpr(){
    ExprAST *result = new NumberExprAST(NumVal);
    getNextToken();
    return result;
}

// parenexpr ::= '(' expression ')'
static ExprAST *ParseParenExpr(){
    getNextToken();
    ExprAST *V = ParseExpression();
    if (!V) return 0;
    if (CurTok != ')')
        return Error("expected ')'");
    getNextToken();
    return V;
}

/*
    identifierexper
        ::= identifier
        ::= identifier '(' expression ')'
*/
static ExprAST *ParseIdentifierExpr(){
    std::string IdName = IdentifierStr;

    getNextToken();

    if (CurTok != '(')
        return new VariableExprAST(IdName);

    getNextToken(); // '('
    std::vector<ExprAST*> Args;
    if (CurTok != ')') {
        while(1){
            ExprAST *Arg = ParseExpression();
            if (!Arg) return 0;
            Args.push_back(Arg);

            if (CurTok == ')') break;

            if (CurTok != ',')
                return Error("Expected ')' or '.' in argumeent list");
            getNextToken();
        }
    }
    // eat the ')'
    getNextToken();

    return new CallExprAST(IdName, Args);
}

/*
    primary
        ::= identifierexpr
        ::= numberexpr
        ::= parenexpr
*/
static ExprAST *ParsePrimary() {
    switch (CurTok) {
    case tok_identifier:return ParseIdentifierExpr();
    case tok_number:    return ParseNumberExpr();
    case '(':           return ParseParenExpr();
    default: return Error("unknown token when expecting an expression");
    }
}

static ExprAST *ParseBinOpRHS(int ExprPrec, ExprAST *LHS){
    while (1){
        int TokPrec = GetTokPrecedence();

        if (TokPrec < ExprPrec)
            return LHS;

        // now it's a binop
        int BinOp = CurTok;
        getNextToken();

        ExprAST *RHS = ParsePrimary();
        if (!RHS) return 0;

        int NexPrec = GetTokPrecedence();
        if (TokPrec < NexPrec){
            RHS = ParseBinOpRHS(TokPrec + 1, RHS);
            if (RHS == 0) return 0;
        }

        // Merge LHS/RHS
        LHS = new BinaryExprAST(BinOp, LHS, RHS);
    }// end loop
}

/*
    expression ::= primary binoprhs
    a * b (primary: a; binoprhs: *b)
*/
static ExprAST *ParseExpression(){
    ExprAST *LHS = ParsePrimary();
    if (!LHS) return 0;

    return ParseBinOpRHS(0, LHS);
}

/*
    prototype
        ::= id '(' id* ')'
*/
static PrototypeAST *ParsePrototype() {
    if (CurTok != tok_identifier)
        return ErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return ErrorP("Expected '(' in prototype");
    
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return ErrorP("Expected ')' in prototype");
    
    // sucess
    getNextToken(); // eat ')'

    return new PrototypeAST(FnName, ArgNames);
}

// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition() {
    getNextToken(); // "def"
    PrototypeAST *Proto = ParsePrototype();
    if (Proto == 0) return 0;

    if (ExprAST *E = ParseExpression())
        return new FunctionAST(Proto, E);

    return 0;
}

// external ::= 'extern' prototype
static PrototypeAST *ParseExtern() {
    getNextToken();
    return ParsePrototype();
}

// toplevelexpr ::= expression
static FunctionAST *ParseTopLevelExpr(){
    if (ExprAST *E = ParseExpression()){
        PrototypeAST *Proto = new PrototypeAST("", std::vector<std::string>());
        return new FunctionAST(Proto, E);
    }
    return 0;
}