#include <string.h>

class ExprAST {
public:
    ~ExprAST() {};
};

// number, like "1.0"
class NumberExprAST : public ExprAST {
    double Val;
public:
    NumberExprAST(double val) : Val(val) {}
};

// variable, like "a"
class VariableExprAST : public ExprAST {
    std::string Name;
public:
    VariableExprAST(const std::string &name) : Name(name) {}
};

// binary operator "a + b"
class BinaryExprAst : public ExprAST {
    char Op;
    ExprAST *LHS, *RHS;
public:
    BinaryExprAst(char op, ExprAST *lhs, ExprAST *rhs)
        : Op(op), LHS(lhs), RHS(rhs) {}
};

// function calls "sin(a)"
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<ExprAST*> Args;
public:
    CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
        : Callee(callee), Args(args) {}
}

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    public:
    PrototypeAST(const std::string &name, const std::vector<std::string> &args)
        : Name(name), Args(args) ()
}
