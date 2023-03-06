#include <iostream>

#include "Parser.h"

std::string program1 = 
"# Compute the x'th fibonacci number.\n"
"def fib(x)\n"
"  if x < 3 then\n"
"    1\n"
"  else\n"
"    fib(x-1)+fib(x-2)\n"
"# This expression will compute the 40th number.\n"
"fib(40)\n";

std::string program2 =
"extern sin(arg);\n"
"extern cos(arg);\n"
"extern atan2(arg1 arg2);\n"
"atan2(sin(.4), cos(42))"
;

static void HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition\n");
    } else {
        getNextToken();
    }
}

static void HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        getNextToken();
    }
}

static void MainLoop() {
    while(1){
        fprintf(stderr, "ready>");
        switch (CurTok) {
        case tok_eof:   return;
        case ';':       getNextToken(); break;
        case tok_def:   HandleDefinition(); break;
        case tok_extern:   HandleExtern(); break;
        default:   HandleTopLevelExpression(); break;
        }
    }
}

int main() {
    // Install standard binary operators
    // 1 is lowest precedence
    BinopPrecedence['<'] = 10;
    BinopPrecedence['>'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;
    BinopPrecedence['/'] = 40;

    fprintf(stderr, "ready> ");
    getNextToken();

    MainLoop();
}
