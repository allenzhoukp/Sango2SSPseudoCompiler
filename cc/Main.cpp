// --------------------
// Sango2 Smart Script Pseudo Code Compiler v0.9.0
// This project is written under the following reference:
//  - Expression Analysis: Mini-c by Sam Nipps (https://github.com/Fedjmike/mini-c);
//  - Lexer: CParser by bajdcc (https://github.com/bajdcc/CParser).
// The script code generated can be recognized and compiled by the assembly compiler written by Henryshow.
// Copyright (c) 2018 Allen Zhou
// --------------------

#include "Lexer.h"
#include "Parser.h"
#include "Localization.h"

int main(int argc, char** argv) {
    if(argc != 2 && argc != 3) {
        printf("%s", mainMessage.c_str());
        return 1;
    }

    FILE* fout = fopen(argc == 3 ? argv[2] : "ss.asm", "w");

    Lexer* lexer = new Lexer(argv[1]);
    lexer->process();

    Parser* parser = new Parser(lexer->tokens, lexer->tokenCount);
    fprintf(fout, "%s", parser->str().c_str());

    return 0;
}
