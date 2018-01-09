#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <regex>

#include "Token.h"

using std::cmatch;
using std::regex;
using std::string;

class Lexer {

private:
    const int MAX_FILE_CNT = 100;
    const int MAX_FILE_LEN = 1 << 20;

    cmatch cm;
    // regex r_digit{ R"((\d*\.?\d+|\d+\.?\d*)([e][+-]?\d+)?)" };
    regex rDigit { R"((\d+)|(0x)*[0-9A-Fa-f]+(h)*)" }; //Oct, Dec or Hex numbers.
    regex rAlpha { R"([a-zA-Z_]\w*)" }; //Starts with non-number.
    regex rSpace { R"(([ \t]+)|((?:\r\n)+)|(\n+))" }; //Multiple spaces, or new line.
    regex rOperator2chars { R"((->)|(\+\+)|(--)|(<<)|(>>)|(>=)|(<=)|(==)|(!=)|(&&)|(\|\|)|([-+*/%^&|]=)|(<<=)|(>>=)|(/\*)|(\*/)|(//))" };
    regex rOperator1char { R"([-+*/%=~!^&|,;:<>()[\]\{\}])" };
    //regex rOperator2chars { R"()"};
    //regex rOperator1char { R"()"};

    void move(int dist);
    void readFile(string FileName);
    void nextSpace();
    bool nextNumber();
    bool nextString();
    bool nextIdent();
    void nextMacro();
    bool nextOperator();

public:

    struct File {
        string fileName;
        char* fileContent;
        char* input;
        int curln;
        int curcol;
    };

    File* fileStack;
    int fileStackTop;
    File* curfile;

    Token* tokens;
    int tokenCount;
    Token* curtoken;

    Lexer(string rootFileName);
    ~Lexer();

    Token* next();

};

#endif
