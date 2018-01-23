#include "Lexer.h"
#include "Parser.h"
#include <iostream>

using namespace std;

int main() {
    Lexer* lexer = new Lexer("Testcc.txt");
    while(lexer->next()->content != "EOF");

    // for(int i = 0; i < lexer->tokenCount; i++) {
    //     cout << lexer->tokens[i].content << " type=" << lexer->tokens[i].type;
    //     if(lexer->tokens[i].type == TokenType::tokenNum)
    //         cout << ", num=" << lexer->tokens[i]._number;
    //     cout << endl;
    // }

    Parser* parser = new Parser(lexer->tokens, lexer->tokenCount);
    printf("%s", parser->str().c_str());

    return 0;
}
