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
#include "Panic.h"
#include <fstream>

// To prevent massive heap allocation, move parser here.
Parser parser;

int main(int argc, char** argv) {
    if(argc != 2 && argc != 3 && argc != 4) {
        printf("%s", mainMessage.c_str());
        return 1;
    }

    printf("Sango Smart Script Compiler v0.9.8\n");
    
    Config::loadConfig();

    char* infileName = new char[MAX_LOCAL_NUM];
    if (infileName[0] == '.' && (infileName[1] == '/' || infileName[1] == '\\'))
        infileName += 2;
    for (int i = 0; i <= strlen(argv[1]); i++) {
        if (argv[1][i] == '/') argv[1][i] = '\\';
        if (argv[1][i] == '.' || argv[1][i] == '\0') {
            strncpy(infileName, argv[1], i);
            infileName[i] = '\0';
            break;
        }
    }
    string str_infileName (infileName);
    string defaultOutfile = str_infileName + ".asm";
    string outfile (argc == 3 && argv[2][0] != '-' ? argv[2] : defaultOutfile);

    std::ofstream out(outfile);
    // FILE* fout = fopen(argc == 3 ? argv[2] : defaultOutfile, "w");

    Lexer* lexer = new Lexer(argv[1]);
    lexer->process();

    parser.init(lexer->tokens, lexer->tokenCount);
    // fprintf(fout, "%s", parser.str().c_str());

    if (!Panic::success) {
        printf("Assembly file generation failed. \n");
        return 1;
    }
    out << parser.str();
    printf("Assembly file generation completed. \n");
    out.close();

    if (argc == 4 || (argc == 3 && argv[2][0] == '-')) {
        if (strcmp(argv[argc - 1], "-auto") == 0 || strcmp(argv[argc - 1], "-a") == 0) {
            // call sg2asm to compile outfileBase.asm to outfileBase.cds and outfileBase.lst
            string sg2asm ("sg2asm " + outfile);
            system(sg2asm.c_str());
            
            string outfileBase = outfile.substr(0, outfile.length() - 4);

            // outfileBase.lst is useless. 
            string del1 ("del \"" + outfileBase + ".lst\"");
            system(del1.c_str());

            // outfileBase.cds will be renamed to outfileBase.so. Replace existing file.
            string rename ("copy \"" + outfileBase + ".cds\" \"" + outfileBase + ".so\" /y");
            system(rename.c_str());
            string del2 ("del \"" + outfileBase + ".cds\"");
            system(del2.c_str());
            
        }
    }
 
    return 0;
}
