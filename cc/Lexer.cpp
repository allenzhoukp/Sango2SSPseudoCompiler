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

        // read unsigned int for Hex and Oct numbers.
        // usually the sign doesn't matter if such number exceeds 0x7FFFFFFF - for bit operations only.
        // so curtoken->_number keeps the signed type.
        if(cm[4].str() == "h" || cm[2].str() == "0x")
            sscanf(cm[3].str().c_str(), "%X", &curtoken->_number);
        else if(curtoken->content[0] == '0')
            sscanf(cm[0].str().c_str(), "%O", &curtoken->_number);
        else
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
    // match #include
    if(std::regex_search(curfile->input, cm, rInclude,
            std::regex_constants::match_continuous)) {
        string fileName = cm[1].str();
        move(cm[0].str().length());
        readFile(fileName);

    // other macros treated as comments: skip
    } else if (std::regex_search(curfile->input, cm, rOtherMacro,
            std::regex_constants::match_continuous)){
        move(cm[0].str().length());

    }
}

bool Lexer::nextOperatorOrComment() {

    //comments
    if(std::regex_search(curfile->input, cm, rComment, std::regex_constants::match_continuous)) {
        move(cm[0].str().length());
        return false;
    }

    if((std::regex_search(curfile->input, cm, rOperator2chars, std::regex_constants::match_continuous) ||
        std::regex_search(curfile->input, cm, rOperator1char, std::regex_constants::match_continuous))) {

        curtoken->type = TokenType::tokenOperator;
        curtoken->content = cm[0].str();
        move(cm[0].str().length());
        return true;
    }

    Panic::panic("Invalid operator", curfile->fileName, curfile->curln, curfile->curcol);
    while(!isspace(*curfile->input))
        move(1);
    return false;
}

bool Lexer::nextAsm() {
    if(std::regex_search(curfile->input, cm, rAsm, std::regex_constants::match_continuous)) {
        curtoken->type = TokenType::tokenAsm;
        curtoken->content = cm[1].str();
        move(cm[0].str().length());
        return true;
    }
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
        else if(*curfile->input == '\0' || *curfile->input == EOF) {
            fileStackTop--;

            //empty stack: end. Return an EOF token.
            //  (This token is w/index = tokenCount, so won't be counted in token list.)
            if(fileStackTop == 0) {
                curtoken->content = "EOF";
                return curtoken;
            }

            curfile = &fileStack[fileStackTop - 1];
        }

        else
            break;
    }

    //get token's position info (source file, line, column)
    curtoken->fileName = curfile->fileName;
    curtoken->lineNo = curfile->curln;
    curtoken->columnNo = curfile->curcol;

    //result: if there is a new token found
    char ch = *curfile->input;
    bool result = false;

    if(strncmp(curfile->input, "__", 2) == 0 && nextAsm())
        result = true;

    else if(isalpha(ch) || ch == '_')
        result = nextIdent();

    else if(isdigit(ch))
        result = nextNumber();

    else if(ch == '\'' || ch == '"')
        result = nextString();

    else
        result = nextOperatorOrComment();

    //There is new token: return it. tokenCount points at a new blank token.
    if(result) {
        tokenCount++;
        return curtoken;

    //No new token: recursion (may happen when panic, or there is comments).
    //tokenCount does not move.
    } else return next();

}
