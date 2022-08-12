#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <set>

#include "Token.h"

using std::cmatch;
using std::regex;
using std::string;
using std::vector;


class Lexer {

private:
    const int MAX_FILE_CNT = 100;
    const int MAX_FILE_LEN = 1 << 20;

    vector<std::pair<string, string> > replace;

    std::set<string> fileIncluded;

    cmatch cm;
    // regex r_digit{ R"((\d*\.?\d+|\d+\.?\d*)([e][+-]?\d+)?)" };
    regex rHex { R"((?:0x([0-9A-Fa-f]+))|(([0-9A-Fa-f]+)h))" };
    regex rDigit { R"((\d+))" }; //Oct, Dec or Hex numbers.
    regex rFloat { R"(((\d*\.\d+|\d+\.)([eE][+-]\d+)?[Ff]?))" }; //Float numbers. No sign to avoid messing up
    regex rAlpha { R"([a-zA-Z_]\w*)" }; //Starts with non-number.
    regex rSpace { R"(([ \t]+)|((?:\r\n)+)|(\n+))" }; //Multiple spaces, or new line.
    regex rComment { R"((//[^\n]*\n)|(/\*[\s\S]*?\*/))" };
    regex rInclude { R"(#include[ \t]*[<"]([^<>\|:""\*\?]+)[">]\s*)" };
    regex rDefine { R"(#define[ \t]+(\S+)[ \t]+(.+?)\s*\n)" };
    regex rOtherMacro { R"(#[^\n]*\n)" };
    regex rAsm { R"(__asm\s*\{([^\}]*)\})" };
    regex rOperator3chars { R"((<<=)|(>>=))" };
    regex rOperator2chars { R"((->)|(\+\+)|(--)|(<<)|(>>)|(>=)|(<=)|(==)|(!=)|(&&)|(\|\|)|([-+*/%^&|]=)|(<<=)|(>>=)|(/\*)|(\*/)|(//))" };
    regex rOperator1char { R"([-+*/%=~!^&|,.;:<>()[\]\{\}])" };
    //regex rOperator2chars { R"()"};
    //regex rOperator1char { R"()"};

    char* preprocessDefineTemp = new char[MAX_FILE_LEN];
    void preprocessDefineExisting();
    void preprocessDefineNew(std::pair<string, string> r);

    void move(int dist);
    void readFile(string FileName);
    void nextSpace();
    bool nextNumber();
    bool nextString();
    bool nextIdent();
    void nextMacro();
    bool nextOperatorOrComment();
    bool nextAsm();

    Token* next();

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

    void process();

};

#endif
