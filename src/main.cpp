#include <iostream>
#include <string.h>

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

// Lexer
enum Token{
    tok_eof = -1,
    tok_def = -2, tok_extern = -3,
    tok_identifier = -4, tok_number = -5
};
static std::string IdentifierStr;
static double NumVal;

// gettok - Return the next token from standard input
static int gettok(){
    static char LastChar = ' ';
    
    // Skip any whitespace
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) { // [a-zA-Z][a-zA-Z09]*
        IdentifierStr = LastChar;
        while (isalnum(LastChar = getchar()))
            IdentifierStr += LastChar;
        
        if (IdentifierStr == "def") return tok_eof;
        if (IdentifierStr == "extern") return tok_extern;
        return tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do{
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#'){
        do LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        if (LastChar != EOF)
            return gettok();
    }

    if (LastChar == EOF)
        return tok_eof;
    
    char ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int main() {
    std::cout << "Hello world!" << std::endl;
}
