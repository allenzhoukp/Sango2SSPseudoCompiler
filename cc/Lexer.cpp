#include "Lexer.h"
#include "Panic.h"

void Lexer::preprocessDefine () {
    //It starts from input pointer.
    //Brute Force, Chernov! Brute Force!
    char* temp = new char[MAX_FILE_LEN];
    for(int i = 0; i < replace.size(); i++) {
        char *p = curfile->input;
        char *pos;
        while((pos = strstr(p, replace[i].first.c_str())) != NULL) {
            strcpy(temp, pos + replace[i].first.length());
            strcpy(pos, replace[i].second.c_str());
            p = pos + replace[i].second.length();
            strcpy(p, temp);
        }
    }
    delete[] temp;
}

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

    //Preprocessing is done for all texts below the #define macro.
    //And of course in the new file included.
    preprocessDefine();

    fclose(fin);
}


void Lexer::nextSpace () {
    if(std::regex_search(curfile->input, cm, rSpace, std::regex_constants::match_continuous)) {
        move(cm[0].str().length());
    }
}

//Henryshow asm compiler does not support float constants.
bool Lexer::nextNumber () {

    // read unsigned int for Hex and Oct numbers.
    // usually the sign doesn't matter if such number exceeds 0x7FFFFFFF - for bit operations only.
    // so curtoken->_number keeps the signed type.

    if(std::regex_search(curfile->input, cm, rHex, std::regex_constants::match_continuous)) {
        //Hex: 0x
        if(strncmp(curfile->input, "0x", 2) == 0)
            sscanf(cm[1].str().c_str(), "%X", &curtoken->_number);
        //Hex: xxxh
        else
            sscanf(cm[2].str().c_str(), "%X", &curtoken->_number);

    } else if(std::regex_search(curfile->input, cm, rDigit, std::regex_constants::match_continuous)) {

        //Oct
        if(*curfile->input == '0')
            sscanf(cm[0].str().c_str(), "%o", &curtoken->_number);
        //Dec
        else
            sscanf(cm[0].str().c_str(), "%d", &curtoken->_number);

    } else {
        Panic::panic("digit not match", curfile->fileName, curfile->curln, curfile->curcol);
        while(!isspace(*curfile->input))
            move(1);
        return false;
    }

    curtoken->content = cm[0].str();
    curtoken->type = TokenType::tokenNum;
    move(curtoken->content.length());
    return true;
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

    // match #define ORIGINAL REPLACED
    } else if (std::regex_search(curfile->input, cm, rDefine,
            std::regex_constants::match_continuous)){
        string original = cm[1].str();
        string replaced = cm[2].str();
        replace.push_back(std::make_pair(original, replaced));
        move(cm[0].str().length());

        preprocessDefine();

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

    if(std::regex_search(curfile->input, cm, rOperator3chars, std::regex_constants::match_continuous) ||
       std::regex_search(curfile->input, cm, rOperator2chars, std::regex_constants::match_continuous) ||
       std::regex_search(curfile->input, cm, rOperator1char, std::regex_constants::match_continuous )) {

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
    //There is problem for std::regex to process long strings.
    //For other tokens there's no problem, but an __asm token can be very long...
    /*
    if(std::regex_search(curfile->input, cm, rAsm, std::regex_constants::match_continuous)) {
        curtoken->type = TokenType::tokenAsm;
        curtoken->content = cm[1].str();
        move(cm[0].str().length());
        return true;
    }
    */

    if(strncmp(curfile->input, "__asm", 5) != 0)
        return false;
    move(5);

    curtoken->type = TokenType::tokenAsm;
    curtoken->content = "";

    while(isspace(*curfile->input)) move(1);
    if(*curfile->input != '{')
        Panic::panic("Expect '{' after '__asm'", curfile->fileName, curfile->curln, curfile->curcol);
    move(1);

    bool disableRec = false;
    while(true) {
        if(*curfile->input == '"' && *(curfile->input - 1) != '\\')
            disableRec = !disableRec;
        else if(strncmp(curfile->input, "/*", 2) == 0)
            disableRec = true;
        else if(strncmp(curfile->input, "//", 2) == 0) {
            disableRec = true;
            move(1); 
            *curfile->input = ';'; //special treatment: // is invalid in __asm block. switch to ;
        } else if(*curfile->input == '\n' || strncmp(curfile->input, "*/", 2) == 0)
            disableRec = false;

        if(!disableRec && *curfile->input == '}' && *(curfile->input - 1) != '\\')
            break;

        curtoken->content += *curfile->input;
        move(1);
    }
    move(1); //skip }
    return true;
}

Lexer::Lexer (string rootFileName) {
    fileStack = new File[MAX_FILE_CNT];
    tokens = new Token[MAX_FILE_LEN];
    fileStackTop = tokenCount = 0;
    readFile(rootFileName);

    //sg2lang here!
    readFile("sg2lang.h");
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

void Lexer::process() {
    while(next()->content != "EOF");
}
