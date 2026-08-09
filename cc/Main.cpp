// --------------------
// Sango Smart Script Pseudo Code Compiler v0.9.9
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
#include <cstdio>
#include <cstring>
#include <fstream>

// To prevent massive heap allocation, move parser here.
Parser parser;

namespace {

bool isAutoOption(const char* argument) {
    return strcmp(argument, "-auto") == 0 || strcmp(argument, "-a") == 0;
}

string replaceExtension(const string& path, const string& extension) {
    const string::size_type separator = path.find_last_of("/\\");
    const string::size_type dot = path.find_last_of('.');
    if (dot != string::npos
            && (separator == string::npos || dot > separator)) {
        return path.substr(0, dot) + extension;
    }
    return path + extension;
}

bool fileExistsAndIsNotEmpty(const string& path) {
    std::ifstream input(path.c_str(), std::ios::binary | std::ios::ate);
    return input && input.tellg() > 0;
}

bool fileExists(const string& path) {
    std::ifstream input(path.c_str(), std::ios::binary);
    return static_cast<bool>(input);
}

bool assemble(const string& assemblyPath) {
    const string outputBase = replaceExtension(assemblyPath, "");
    const string objectPath = outputBase + ".so";
    const string temporaryObjectPath = outputBase + ".cds";
    const string listingPath = outputBase + ".lst";

    // A stale assembler artifact must never make a failed invocation look
    // successful. Keep an existing .so until a new .cds has been produced.
    std::remove(temporaryObjectPath.c_str());
    std::remove(listingPath.c_str());

    const string command = "sg2asm.exe \"" + assemblyPath + "\"";
    fflush(stdout);
    const int assemblerResult = system(command.c_str());
    if (assemblerResult != 0
            || !fileExistsAndIsNotEmpty(temporaryObjectPath)) {
        fprintf(stderr, "Assembly failed; no new object file was produced.\n");
        return false;
    }

    if (fileExists(objectPath)
            && std::remove(objectPath.c_str()) != 0) {
        fprintf(stderr, "Unable to replace output object: %s\n",
                objectPath.c_str());
        return false;
    }
    if (std::rename(temporaryObjectPath.c_str(), objectPath.c_str()) != 0) {
        fprintf(stderr, "Unable to rename assembler output to: %s\n",
                objectPath.c_str());
        return false;
    }

    std::remove(listingPath.c_str());
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    if(argc != 2 && argc != 3 && argc != 4) {
        printf("%s", mainMessage.c_str());
        return 1;
    }

    bool autoAssemble = false;
    string outfile = replaceExtension(argv[1], ".asm");
    if (argc == 3) {
        if (isAutoOption(argv[2])) {
            autoAssemble = true;
        } else if (argv[2][0] == '-') {
            printf("%s", mainMessage.c_str());
            return 1;
        } else {
            outfile = argv[2];
        }
    } else if (argc == 4) {
        if (argv[2][0] == '-' || !isAutoOption(argv[3])) {
            printf("%s", mainMessage.c_str());
            return 1;
        }
        outfile = argv[2];
        autoAssemble = true;
    }

    printf("Sango Smart Script Compiler v0.9.9\n");

    Config::loadConfig();

    Lexer* lexer = new Lexer(argv[1]);
    lexer->process();

    parser.init(lexer->tokens, lexer->tokenCount);

    if (!Panic::success) {
        printf("Assembly file generation failed. \n");
        return 1;
    }

    std::ofstream out(outfile.c_str());
    if (!out) {
        fprintf(stderr, "Unable to create assembly output: %s\n",
                outfile.c_str());
        return 1;
    }
    out << parser.str();
    out.close();
    if (!out) {
        fprintf(stderr, "Unable to write assembly output: %s\n",
                outfile.c_str());
        return 1;
    }
    printf("Assembly file generation completed. \n");

    if (autoAssemble && !assemble(outfile)) {
        return 1;
    }

    return 0;
}
