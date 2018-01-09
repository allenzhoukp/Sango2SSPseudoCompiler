#include "Lexer.h"
#include "Panic.h"

void Lexer::move (int dist) {
    while(dist--) {
        if(*(curfile->input) == '\0')
            return;

        //goes to the next line
        if(*(curfile->input) == '\n') {
            curfile->curln++;
            curfile->curcol = 1;
        } else {
            curfile->curcol++;
        }

        curfile->input++;
    }
}

void Lexer::readFile (string fileName) {
    File file;
    file.fileName = fileName;
    FILE* fin = fopen(fileName.c_str(), "r");

    file.fileContent = new char[MAX_FILE_LEN];
    file.input = file.fileContent;
    while((*file.input = fgetc(fin)) != EOF)
        file.input++;
    *(++file.input) = '\0';
    file.input = file.fileContent;

    file.curln = file.curcol = 1;

    fileStack[fileStackTop] = file;
    curfile = &fileStack[fileStackTop++];

    fclose(fin);
}


void Lexer::nextSpace () {
    if(std::regex_search(curfile->input, cm, rSpace, std::regex_constants::match_continuous)) {
        move(cm[0].str().length());
    }
}

//Henryshow asm compiler does not support float constants.
bool Lexer::nextNumber () {
    if(std::regex_search(curfile->input, cm, rDigit, std::regex_constants::match_continuous)) {
        curtoken->content = cm[0].str();
        curtoken->type = TokenType::tokenNum;
        move(curtoken->content.length());

        if(cm[3].str() == "h")
            curtoken->content = "0x" + cm[2].str();

        curtoken->_number = atoi(curtoken->content.c_str());
        return true;
    }
    Panic::panic("digit not match", curfile->fileName, curfile->curln, curfile->curcol);
    while(!isspace(*curfile->input))
        move(1);
    return false;
}

//Char treated as string!
//Everything is handled by Henryshow asm complier. All string formats are unchanged.
bool Lexer::nextString () {
    move(1); //skip " or '
    curtoken->content = "";

    while(true) {
        //Only exception: notice \". Move one more time.
        if(*curfile->input == '\\') {
            curtoken->content += *curfile->input;
            move(1);
            //Jumps to end-if. Will move two times.
            //If there is \", the second " will not be judged by the following else-if.

        } else if (*curfile->input == '"' || *curfile->input == '\''){
            move(1);
            return true;
        }

        curtoken->content += *curfile->input;
        curtoken->type = TokenType::tokenStr;
        move(1);

    }
    Panic::panic("string not match", curfile->fileName, curfile->curln, curfile->curcol);
    while(!isspace(*curfile->input))
        move(1);
    return false;
}

//Everything including things like if, else, while, for, int, string, float, asynccall, etc.
//are considered as identifier temporary.
bool Lexer::nextIdent () {
    if(std::regex_search(curfile->input, cm, rAlpha, std::regex_constants::match_continuous)) {
        curtoken->content = cm[0].str();
        move(curtoken->content.length());
        curtoken->type = TokenType::tokenIdent;
        return true;
    }
    Panic::panic("ident not match", curfile->fileName, curfile->curln, curfile->curcol);
    while(!isspace(*curfile->input))
        move(1);
    return false;
}

//Only valid macro is #include. Everything else treated as comments.
void Lexer::nextMacro () {
    move(1); //Skip '#'

    if(strncmp(curfile->input, "include", 7) == 0) {
        move(7);

        //skip spaces
        while(*curfile->input == ' ' || *curfile->input == '\t')
            move(1);

        //#include format
        if(*curfile->input != '<' && *curfile->input != '"') {
            Panic::panic("Invalid #include macro. Usage: #include \"filename\" or #include <filename>",
                curfile->fileName, curfile->curln, curfile->curcol);
            return;
        }

        //Skip left
        move(1);

        //Get filename
        string fileName = "";
        while(*curfile->input != '>' && *curfile->input != '"') {
            fileName += *curfile->input;
            move(1);
        }

        //Skip right
        move(1);

        //readFile() will adjust stack.
        //stack pop will be done in next().
        readFile(fileName);

    //Treat as comments
    } else {
        //go through towards new line
        while(*curfile->input != '\n')
            move(1);

        //skip '\n'
        move(1);
    }

}

bool Lexer::nextOperator() {
    if((std::regex_search(curfile->input, cm, rOperator2chars, std::regex_constants::match_continuous) ||
        std::regex_search(curfile->input, cm, rOperator1char, std::regex_constants::match_continuous))) {

        //comments!
        if(cm[0].str() == "//") {
            move(2);
            while(*curfile->input != '\n')
                move(1);
            move(1);

        } else if (cm[0].str() == "/*") {
            while(strncmp(curfile->input, "*/", 2) != 0)
                move(1);
            move(2);

        //Operators
        } else {
            curtoken->type = TokenType::tokenOperator;
            curtoken->content = cm[0].str();
            move(cm[0].str().length());
            return true;
        }

        return false;
    }
    Panic::panic("Invalid operator", curfile->fileName, curfile->curln, curfile->curcol);
    while(!isspace(*curfile->input))
        move(1);
    return false;
}

Lexer::Lexer (string rootFileName) {
    fileStack = new File[MAX_FILE_CNT];
    tokens = new Token[MAX_FILE_LEN];
    fileStackTop = tokenCount = 0;
    readFile(rootFileName);
}

Lexer::~Lexer () {
    while(fileStackTop)
        delete[] fileStack[--fileStackTop].fileContent;
    delete[] fileStack;
    delete[] tokens;
}

Token* Lexer::next() {

    curtoken = &tokens[tokenCount];
    curtoken->type = TokenType::tokenErr;

    //Skip spaces and macros
    while(true) {
        if(isspace(*curfile->input))
            nextSpace();

        else if(*curfile->input == '#')
            nextMacro();

        //End-of-file: pop stack.
        //empty stack: end.
        else if(*curfile->input == '\0' || *curfile->input == EOF) {
            fileStackTop--;

            if(fileStackTop == 0) {
                curtoken->content = "EOF";
                return curtoken;
            }

            curfile = &fileStack[fileStackTop - 1];
        }

        else
            break;
    }

    //result: if there is a new token found
    char ch = *curfile->input;
    bool result = false;

    if(isalpha(ch) || ch == '_')
        result = nextIdent();

    else if(isdigit(ch))
        result = nextNumber();

    else if(ch == '\'' || ch == '"')
        result = nextString();

    else
        result = nextOperator();

    //There is new token: return it. tokenCount points at a new blank token.
    if(result) {
        tokenCount++;
        return curtoken;

    //No new token: recursion (may happen when panic, or there is comments).
    //tokenCount does not move.
    } else return next();

}
