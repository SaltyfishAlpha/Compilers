#include <string>

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
class BinaryExprAST : public ExprAST {
    char Op;
    ExprAST *LHS, *RHS;
public:
    BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
        : Op(op), LHS(lhs), RHS(rhs) {}
};

// function calls "sin(a)"
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<ExprAST*> Args;
public:
    CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
        : Callee(callee), Args(args) {}
};

// function "name(args...) body"

// function head
class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
    public:
    PrototypeAST(const std::string &name, const std::vector<std::string> &args)
        : Name(name), Args(args) {}
};

// function define
class FunctionAST {
    PrototypeAST *Proto;
    ExprAST *Body;
public:
    FunctionAST(PrototypeAST *proto, ExprAST *body)
        :Proto(proto), Body(body) {}
};
