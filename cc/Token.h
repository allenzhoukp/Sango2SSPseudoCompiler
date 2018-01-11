#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::string;

struct Token {
    enum TokenType_{
        tokenErr, tokenOperator, tokenIdent, tokenNum, tokenStr, tokenAsm
    } type;

    string content;
    string fileName;
    int lineNo;
    int columnNo;

    int _number;
};
typedef Token::TokenType_ TokenType;

#endif
