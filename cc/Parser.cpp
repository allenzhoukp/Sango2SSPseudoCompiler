#include "Parser.h"
#include "Panic.h"

#include <cstdio>
#include <cstring>
#include <sstream>

void Parser::loadSyscallTable () {
    FILE* fin = fopen("SYSCALL_TABLE.txt", "r");
    Syscall syscall;
    char name[MAX_LINE_LEN];

    if(fin == NULL) {
        printf("Unable to load SYSCALL_TABLE.txt!");
        exit(1);
    }

    while(fscanf(fin, "%x %d %d %s", &syscall.code, &syscall.paramCount,
                      &syscall.returnType, name) != EOF) {
        syscall.name = string(name);
        syscalls[syscallCount] = syscall;
        syscallNameMapping[name] = &syscalls[syscallCount];
        syscallCount++;
    }
    fclose(fin);
}

void Parser::loadIntvTable () {
    FILE* fin = fopen("INTV_TABLE.txt", "r");
    Intv intv;
    char name[MAX_LINE_LEN];

    if(fin == NULL)
        return;

    while(fscanf(fin, "%s %x", name, &intv.intvNo) != EOF) {
        intv.name = string(name);
        intvs[intvCount++] = intv;
    }
    fclose(fin);
}

//==========Auxillary function============

bool Parser::isPtr(int type) {
    return type >= 10;
}

void Parser::require (int tokenPos, bool condition, string errmsg) {
    if(!condition)
        Panic::panic(errmsg, tokens[tokenPos].fileName,
            tokens[tokenPos].lineNo, tokens[tokenPos].columnNo);
}

bool Parser::see (int tokenPos, string s) {
    return tokens[tokenPos].content == s;
}

// match a token. If current token does not match s, panic.
void Parser::match (int& tokenPos, string s) {
    if(!see(tokenPos, s))
        Panic::panic("Error: expected " + s + ", found '" + tokens[tokenPos].content + "'",
            tokens[tokenPos].fileName, tokens[tokenPos].lineNo, tokens[tokenPos].columnNo);

    tokenPos++;
}

// try to match a token. If not, nothing happens.
bool Parser::tryMatch (int& tokenPos, string s) {
    if(see(tokenPos, s)) {
        tokenPos++;
        return true;
    }
    return false;
}

//get type code from token position.
//if match == true, tokenPos will be moved.
//the name of the type will be written to typeName.
int Parser::getType (int& tokenPos, bool match, string& typeName) {

    typeName = "";
    int result = -1;
    int tokmove = 1;
    string content = tokens[tokenPos].content;

    //prevent exceeding.
    string nextContent = tokenPos + 1 < tokenCount ? tokens[tokenPos + 1].content : "";

    //void, int, string, float and their ptrs.
    if(typeNameToCode.find(content) != typeNameToCode.end()) {

        //void*, string*
        //NOTE no float * support for now.
        if(nextContent == "*" && content != "int" && content != "float") {
            typeName = content + " *";
            result = 10 + typeNameToCode[content];
            tokmove = 2;
        //int *
        } else if (nextContent == "*" && content == "int"){
            typeName = "int *";
            result = DataTypes::typeIntPtr;
            tokmove = 2;
        //void, int, string, float
        } else {
            typeName = content;
            result = typeNameToCode[content];
        }

    } else if(content == "unsigned") {
        //unsigned *
        if(nextContent == "*") {
            typeName = "unsigned *";
            result = DataTypes::typeUIntPtr;
            tokmove = 2;

        //unsigned int *, unsigned short *, unsigned byte *
        } else if(numericNameToCode.find(nextContent) != numericNameToCode.end()) {
            string thirdContent = tokenPos + 2 < tokenCount ? tokens[tokenPos + 2].content : "";
            if(thirdContent == "*") {
                typeName = "unsigned " + nextContent + " *";
                result = 110 + numericNameToCode[nextContent] + 1;
                tokmove = 3;
            }
        }

    //short *, byte *
    } else if(numericNameToCode.find(content) != numericNameToCode.end()) {
        if(nextContent == "*") {
            typeName = content + " *";
            result = 110 + numericNameToCode[nextContent];
            tokmove = 2;
        }

    //TODO struct *
    } else if(nextContent == "*") {
        typeName = content + " *";
        result = DataTypes::typeStructPtr;
        tokmove = 2;
    }

    if(match)
        tokenPos += tokmove;

    return result;
}

int Parser::getType(int& tokenPos, bool match) {
    string s;
    return getType(tokenPos, match, s);
}

bool Parser::validateType(int target, int given) {
    if(isPtr(target))
        target = DataTypes::typeInt;
    if(isPtr(given))
        given = DataTypes::typeInt;
    return target == given;
}

void Parser::newLocal(int type, string name) {
    Var& var = currentFunc->locals[currentFunc->localCount];
    var.type = type;
    var.name = name;
    var.no = currentFunc->localCount + 1; //local no starts from 1.
    currentFunc->numberMapping.emplace(name, var.no);
    currentFunc->localCount++;
}

void Parser::initParam(vector<Var> params) {
    for(int i = 0; i < params.size(); i++) {
        params[i].no = -(params.size() - i + 1);  //e.g. 7 params, first (idx=0) param has number -8. idx=1, no=-7, etc.

        currentFunc->params[i] = params[i]; //maintain order in params[]
        currentFunc->numberMapping.emplace(params[i].name, params[i].no);
    }
    currentFunc->paramCount = params.size();
}

//Returns -1 if not found.
int Parser::getLocalNoByName(string name) {
    if(currentFunc->numberMapping.find(name) != currentFunc->numberMapping.end())
        return currentFunc->numberMapping[name];
    return -1;
}

//Returns a name="NULL" Var if not found.
Var Parser::getLocalByName(string name) {
    int number = getLocalNoByName(name);
    //positive for locals, negative for params.
    //local no starts from 1.
    //param no starts from -2.
    if(number > 0)
        return currentFunc->locals[number - 1];
    else if(number <= -2) //params[i].no = -paramsCount + i - 1 => i = paramsCount + number + 1
        return currentFunc->params[currentFunc->paramCount + number + 1];
    else {
        Var var;
        var.name = "NULL";
        return var;
    }
}

//returns -1 if not found.
int Parser::getIntvNoByName(string name) {
    //All converted to camel names.
    //e.g. INTV_ATTACKER_MAJOR -> attackerMajor
    for(int i = 0; i < intvCount; i++)
        if(intvs[i].name == name)
            return intvs[i].intvNo;

    return -1;
}

//returns an Intv w/name = "NULL" if not found.
Intv Parser::getIntvByName(string name) {
    for(int i = 0; i < intvCount; i++)
        if(intvs[i].name == name)
            return intvs[i];

    Intv intv;
    intv.name = "NULL";
    return intv;
}

//============Create Expression Tree=============

Parser::ExpressionNodePool::ExpressionNodePool() {
    clear();
}

ExpressionNode* Parser::ExpressionNodePool::newNode() {
    return &pool[len++];
}

void Parser::ExpressionNodePool::clear() {
    len = 0;
    for(int i = 0; i < MAX_LINE_LEN * 10; i++) {
        pool[i].left = pool[i].right = NULL;
        pool[i].isLvalue = false;
    }
}

void Parser::initExpNodeParamArray (ExpressionNode* x, int len) {
    x->params = new ExpressionNode*[MAX_FUNC_LEN + 1];
    memset(x->params, 0, sizeof(ExpressionNode *) * (MAX_FUNC_LEN + 1));
}

bool Parser::tryMatchSyscall(ExpressionNode* &x, int& tokenPos){
    string content = tokens[tokenPos].content;
    if(syscallNameMapping.find(content) != syscallNameMapping.end()) {
        Syscall* syscall = syscallNameMapping[content];

        if(x == NULL)
            x = expNodePool.newNode();

        x->type = ExpNodeType::syscall;
        x->name = content;
        x->isLvalue = false;
        x->resultType = syscall->returnType;

        tokenPos++; //skip syscall name
        match(tokenPos, "(");

        initExpNodeParamArray(x, syscall->paramCount);
        int paramCount = 0;

        if(!see(tokenPos, ")")) do {
            expr(x->params[paramCount++], tokenPos, 0);
        } while (tryMatch(tokenPos, ","));

        match(tokenPos, ")");

        std::ostringstream errmsg;
        errmsg << "the number of parameters does not match. Required " << syscall->paramCount <<
            " params, seen " << paramCount;
        require(tokenPos - 1,
            paramCount == syscall->paramCount, errmsg.str());

        return true;

    }
    return false;
};

//GetGlobal(), SetGlobal(), GetIntv(), SetIntv(), Delay(), Wait(), IsRunning()
//INST_4F,     INST_52,     PUSHINVR,  INST_53,   DELAY,   INST_45,INST_46
bool Parser::tryMatchInst(ExpressionNode* &x, int& tokenPos){

    //For instructions, looks like the only appropriate way is to write it in a hard-wire way.

    string name = tokens[tokenPos].content;

    //Global Vars (INST_4F, INST_52)
    //For now, ONLY INT globals are supported.
    //If we want to know the exact type, the problem is that - we can't infer type from the index
    //   we can only know in runtime. Originally ODIN knows all the globals and they work like locals.
    //   So looks like we can only implement things like that when we manage to figure out
    //   all the existing globals - especially their types.
    //TODO Globals support
    if(tryMatch(tokenPos, "GetGlobal")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeInt;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 1);
        expr(x->params[0], tokenPos, 0);

        require(tokenPos, x->params[0]->resultType == DataTypes::typeInt,
            "GetGlobal() requires an integer parameter");

        match(tokenPos, ")");
    }
    else if(tryMatch(tokenPos, "SetGlobal")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeVoid;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 2);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, x->params[0]->resultType == DataTypes::typeInt,
            "SetGlobal() requires integer parameters");
        match(tokenPos, ",");
        expr(x->params[1], tokenPos, 1);
        require(tokenPos, x->params[1]->resultType == DataTypes::typeInt,
            "SetGlobal() requires integer parameters");
        match(tokenPos, ")");
    }

    //INTVs (PUSHINVR, INST_53)
    //Two ways to use Intvs: use GetIntv/SetIntv,
    //    or write it in INTV_LIST.txt and use it like a local.
    //Intvs can only have integer value (even in the original code, most perhaps)
    else if(tryMatch(tokenPos, "GetIntv")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeInt;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 1);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, x->params[0]->resultType == DataTypes::typeInt,
            "GetIntv() requires an integer parameter");
        match(tokenPos, ")");
    }
    else if(tryMatch(tokenPos, "SetIntv")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeVoid;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 2);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, x->params[0]->resultType == DataTypes::typeInt,
            "SetGlobal() requires integer parameters");
        match(tokenPos, ",");
        expr(x->params[1], tokenPos, 1);
        require(tokenPos, x->params[1]->resultType == DataTypes::typeInt,
            "SetGlobal() requires integer parameters");
        match(tokenPos, ")");
    }

    //delay(), wait(), isRunning() (DELAY, INST_45, INST_46)
    else if(tryMatch(tokenPos, "Delay")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeVoid;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 1);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, x->params[0]->resultType == DataTypes::typeInt,
            "Delay() requires an integer parameter");
        match(tokenPos, ")");
    }
    else if(tryMatch(tokenPos, "Wait") || tryMatch(tokenPos, "IsRunning")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = name == "Wait" ? DataTypes::typeVoid : DataTypes::typeInt;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 1);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, x->params[0]->resultType == DataTypes::typeString,
            name + "() requires a string parameter");
        match(tokenPos, ")");
    }
    else
        return false;

    x->type = ExpNodeType::inst;
    x->isLvalue = false;
    x->name = name;

    return true;
};

bool Parser::tryMatchFunccall(ExpressionNode* &x, int& tokenPos){
    string name = tokens[tokenPos].content;
    if(funcNameMapping.find(name) != funcNameMapping.end()) {
        if(x = NULL)
            x = expNodePool.newNode();

        Function* func = funcNameMapping[name];
        x->type = ExpNodeType::funcCall;
        x->name = name;
        x->resultType = func->returnType;
        x->isLvalue = false;

        initExpNodeParamArray(x, func->paramCount);

        match(tokenPos, "(");
        int paramCount = 0;

        if(!see(tokenPos, ")")) do {
            expr(x->params[paramCount], tokenPos, 0);
            require(tokenPos,
                validateType(func->params[paramCount].type, x->params[paramCount]->resultType),
                "parameter type not match");

        } while (tryMatch(tokenPos, ","));

        std::ostringstream err;
        err << "function " << func->name << " requires " << func->paramCount <<
            " parameter(s), found " << paramCount;
        require(tokenPos, paramCount == func->paramCount,
             err.str());

        match(tokenPos, ")");
        return true;
    }
    return false;
};

//Back: operators after an object w/highest priority [], ->, ++, --
void Parser::back (ExpressionNode* &nodePos, int& tokenPos) {
    ExpressionNode* x = expNodePool.newNode();
    ExpressionNode* left = nodePos;

    int operatorTokenPos = tokenPos;

    if(tryMatch(tokenPos, "[")) {

        //Currently, only direct locals supported
        //TODO direct intvs (not for instruction-call-intvs, i.e. GetINV())
        require(operatorTokenPos,
            left->type == ExpNodeType::local,
            "Operator [] can only be applied to local variables currently");

        //The right operand of [] is the index.
        expr(x->right, tokenPos, 0);
        match(tokenPos, "]");

        x->op = "[]";
        x->type = ExpNodeType::binaryOp;
        x->resultType = left->resultType;
        x->isLvalue = true; //indeed.

    } else if (tryMatch(tokenPos, "->")) {

        require(operatorTokenPos,
            left->resultType == DataTypes::typeStructPtr,
            "Operator -> can only be applied to struct *");

        //TODO match structures

        x->op = "->";
        x->isLvalue = true;

    } else if (tryMatch(tokenPos, "++") || tryMatch(tokenPos, "--")) {

        require(operatorTokenPos,
            left->isLvalue == true,
            "Operator ++/-- requires an lvalue operand");

        //set global sign.
        if(tokens[operatorTokenPos].content == "++")
            ppNode = x;
        else
            mmNode = x;

        x->op = "b";
        x->type = ExpNodeType::unaryOp;
        x->resultType = left->resultType;
        x->isLvalue = false; //you can't do something like (i++) = 2;

    } else return;

    //Put the new node into the position.
    x->left = left;
    nodePos = x;
}

//Object: integer constants, string constants, braces, locals, function calls, syscalls, instructions, etc.
void Parser::object (ExpressionNode* &x, int& tokenPos) {

    if(x == NULL)
        x = expNodePool.newNode();
    Token& token = tokens[tokenPos];

    // integer constants
    if(token.type == TokenType::tokenNum) {
        x->type = ExpNodeType::intConst;
        x->intValue = token._number;
        x->resultType = DataTypes::typeInt;
        x->isLvalue = false;
        tokenPos++;

    // string constants
    } else if (token.type == TokenType::tokenStr) {
        x->type = ExpNodeType::strConst;
        x->strValue = token.content;
        x->resultType = DataTypes::typeString;
        x->isLvalue = false;
        tokenPos++;

    // identifiers: vars, INTVs, function calls, syscalls, instructions
    } else if (token.type == TokenType::tokenIdent) {

        //true, false (constants)
        if(token.content == "true" || token.content == "false") {
            x->type = ExpNodeType::intConst;
            x->intValue = token.content == "true" ? 1 : 0;
            x->resultType = DataTypes::typeInt;
            x->isLvalue = false;
            tokenPos++;

        //null (constants)
        } else if(token.content == "NULL" || token.content == "null") {
            x->type = ExpNodeType::intConst;
            x->intValue = 0;
            x->resultType = DataTypes::typeVoidPtr;
            x->isLvalue = false;
            tokenPos++;

        //locals
        } else if(getLocalNoByName(token.content) != -1) {
            x->type = ExpNodeType::local;
            x->localVar = getLocalByName(token.content);
            x->resultType = getLocalByName(token.content).type;
            x->isLvalue = true;
            tokenPos++;

        //INTVs
        } else if(getIntvNoByName(token.content) != -1) {
            x->type = ExpNodeType::intv;
            x->intvVar = getIntvByName(token.content);
            x->resultType = getIntvByName(token.content).type;
            x->isLvalue = true; //INTV can be lvalue! SETINV!
            tokenPos++;

        //Syscalls, Insts, Function calls.
        } else if(tryMatchSyscall(x, tokenPos) ||
                  tryMatchInst(x, tokenPos) ||
                  tryMatchFunccall(x, tokenPos) ){
           x->isLvalue = false;

        } else {
            require(tokenPos, false, "Unknown identifier '" + token.content + "'");
            tokenPos++;
        }


    } else if (tryMatch(tokenPos, "(")) {
        expr(x, tokenPos, 0);
        match(tokenPos, ")");

    } else {
        require(tokenPos, false, "Expected an expression, found '" + token.content + "'");
        tokenPos++;
    }

    back(x, tokenPos);
}

//Unary: unary operators !, -, ~, ++, --, *, (type cast)
void Parser::unary(ExpressionNode* &x, int& tokenPos){

    if(x == NULL)
        x = expNodePool.newNode();

    int currentTokenPos = tokenPos;

    //!, -, ~, ++, --, *, (type cast)
    if (tryMatch(tokenPos, "!")) {

        unary(x->left, tokenPos);

        x->type = ExpNodeType::unaryOp;
        x->op = "!";
        x->resultType = DataTypes::typeInt; //logic not MUST results to int.
        x->isLvalue = false;
        return;

    } else if (tryMatch(tokenPos, "-")) {

        unary(x->left, tokenPos); //get the object first.

        x->type = ExpNodeType::unaryOp;
        x->op = "-";
        x->isLvalue = false;

        //result type may be float or int. Pointers treated as int.
        //you can't place a negative in front of a string.
        require(currentTokenPos, x->left->resultType != DataTypes::typeString,
            "A negative sign cannot be placed in front of a string value.");

        if(x->left->resultType == DataTypes::typeFloat)
            x->resultType = DataTypes::typeFloat;
        else
            x->resultType = DataTypes::typeInt;

        return;

    } else if (tryMatch(tokenPos, "~")) {

        unary(x->left, tokenPos);

        x->type = ExpNodeType::unaryOp;
        x->op = "~";
        x->isLvalue = false;

        //bit operations require integer operands.
        require(currentTokenPos,
            x->left->resultType != DataTypes::typeString && x->left->resultType != DataTypes::typeFloat,
            "Operator ~ requires an integer operand.");

        x->resultType = DataTypes::typeInt;
        return;

    } else if (tryMatch(tokenPos, "++") || tryMatch(tokenPos, "--")){
        unary(x->left, tokenPos);

        x->type = ExpNodeType::unaryOp;
        x->op = tokens[currentTokenPos].content;

        //Lvalue required!
        require(currentTokenPos,
            x->left->isLvalue,
            "Operator ++/-- requires an lvalue operand (local variables, global variables, INTVs, etc.)");

        require(currentTokenPos,
            x->left->resultType != DataTypes::typeString,
            "Operator ++/-- cannot be used on a string");

        x->isLvalue = false; //You can't do something like (++i) = 6.
        x->resultType = x->left->resultType; //Float, int or ptr.
        return;

    } else if (tryMatch(tokenPos, "*")) {
        unary(x->left, tokenPos);

        require(currentTokenPos,
            isPtr(x->left->resultType),
            "Operator * can only be applied on pointers");

        require(currentTokenPos,
            x->left->resultType < DataTypes::typeStructPtr && x->left->resultType != DataTypes::typeVoidPtr,
            "Operator * cannot be applied on a struct or void pointer. \n"
            "Note Use operator -> to access struct members");

        x->type = ExpNodeType::unaryOp;
        x->op = "*";
        x->isLvalue = true; //indeed.

        switch(x->left->resultType) {

        case DataTypes::typeIntPtr:
        case DataTypes::typeUIntPtr:
        case DataTypes::typeShortPtr:
        case DataTypes::typeUShortPtr:
        case DataTypes::typeBytePtr:
        case DataTypes::typeUBytePtr:
            x->resultType = DataTypes::typeInt;
            break;

        case DataTypes::typeStringPtr:
            x->resultType = DataTypes::typeString;
            break;

        default:
            x->resultType = DataTypes::typeInt; //Only here when error.
        }

        return;

    } else if (see(tokenPos, "(")) {
        //cast
        int newTokenPos = tokenPos + 1;
        string typeName = "";
        int typeCode = getType(newTokenPos, true, typeName);

        if(typeCode != -1) {
            tokenPos = newTokenPos;
            match(tokenPos, ")");
            x->type = ExpNodeType::unaryOp;

            // I don't know why to_string(int) doesn't work. Lazy compiler author?
            std::ostringstream os;
            os << "(" << typeCode << ")";
            x->op = os.str();

            x->isLvalue = false;
            x->resultType = typeCode;

            //fix: should be unary() (type cast has quite a high priority!)
            unary(x->left, tokenPos);

            //Failed cast: float -> ptr, string -> ptr, string -> float, string -> int, everything -> void
            //Don't give actual instruction for now.
            require(currentTokenPos,
                typeCode == x->left->resultType || typeCode == DataTypes::typeVoid ||
                (!((isPtr(typeCode) && x->left->resultType == DataTypes::typeFloat) || x->left->resultType == DataTypes::typeString)),
                "invalid type cast to (" + typeName + ")");

            return;
        }
        // else not a cast. go to next line.
    }
    //else
    object(x, tokenPos);

}

// 0: =, +=, -=, *=, /=, %=, &=, |=, ^=
// 1: &&, ||
// 2: &, |, ^
// 3: ==, !=, <, <=, >, >=
// 4: <<, >>
// 5: +, -
// 6: *, /, %
// 7: unary()

void Parser::expr(ExpressionNode* &x, int& tokenPos, int level){

    if (level == 7) {
        unary(x, tokenPos);
        return;
    }

    expr (x, tokenPos, level + 1);

    while (level == 6 ? see(tokenPos, "*") || see(tokenPos, "/") || see(tokenPos, "%") :
           level == 5 ? see(tokenPos, "+") || see(tokenPos, "-") :
           level == 4 ? see(tokenPos, "<<") || see(tokenPos, ">>") :
           level == 3 ? see(tokenPos, "==") || see(tokenPos, "!=") ||
                        see(tokenPos, ">") || see(tokenPos, "<") || see(tokenPos, ">=") || see(tokenPos, "<=") :
           level == 2 ? see(tokenPos, "&") || see(tokenPos, "|") || see(tokenPos, "^") :
           level == 1 ? see(tokenPos, "&&") || see(tokenPos, "||") :
              see(tokenPos, "=") || see(tokenPos, "+=") || see(tokenPos, "-=") || see(tokenPos, "*=") ||
              see(tokenPos, "/=") || see(tokenPos, "&=") || see(tokenPos, "|=") || see(tokenPos, "^=") ||
              see(tokenPos, "<<=") || see(tokenPos, ">>=")) {

        // the original node in position of x, is now the left operand of this operator.
        // e.g. a + b * c, w/o "* c", b is the right operand of +; w/"* c", (b * c) is the right operand of +,
        //      and the node for operator * is placed at (node for operator +) -> right.
        ExpressionNode* left = x;
        x = expNodePool.newNode();
        x->left = left;

        x->op = tokens[tokenPos].content;
        x->type = ExpNodeType::binaryOp;
        x->isLvalue = (level == 0 ? true : false);

        int operatorTokenPos = tokenPos;
        tokenPos++; //skip operator

        expr (x->right, tokenPos, level + 1);
        ExpressionNode* right = x->right;

        // For the following, all pointers are automatically converted into int.

        // int, float, string
        if(x->op == "+" || x->op == "==" || x->op == "!=" || x->op == "+=") {

            // for strings: can only compare string w/string, and link string w/string
            if(left->resultType == DataTypes::typeString)
                require(operatorTokenPos,
                    right->resultType == DataTypes::typeString,
                    "Can't apply operator " + x->op + " between a string and a non-string");

            // logic result.
            // int == float -> float == float
            if(x->op == "==" || x->op == "!=")
                x->resultType = DataTypes::typeInt;

            // +
            else {
                //string + string = string
                if(left->resultType == DataTypes::typeString)
                    x->resultType = DataTypes::typeString;

                // int + int = int, float + int or float = float
                else
                    x->resultType = (left->resultType == DataTypes::typeFloat || right->resultType == DataTypes::typeFloat)
                        ? DataTypes::typeFloat : DataTypes::typeInt;
            }

        // int, float, but not string.
        // *,/,-,>,>=,<,<=
        } else if(x->op == "*" || x->op == "/" || x->op == "-" || level == 3 ||
                  x->op == "*=" || x->op == "/=" || x->op == "-=") {
            require(operatorTokenPos,
                left->resultType != DataTypes::typeString && right->resultType != DataTypes::typeString,
                "Invalid operand type: string");

            // If one of operands has float value, then the result is float. int otherwise.
            x->resultType = (left->resultType == DataTypes::typeFloat || right->resultType == DataTypes::typeFloat)
                ? DataTypes::typeFloat : DataTypes::typeInt;

        // only int
        } else if(x->op == "%" || level == 4 || level == 2 || level == 1 ||
                  x->op == "%=" || x->op == "<<=" || x->op == ">>=" || x->op == "&=" || x->op == "|=" || x->op == "^=") {
            require(operatorTokenPos,
                left->resultType != DataTypes::typeString && right->resultType != DataTypes::typeString,
                "Invalid operand type: string");

            require(operatorTokenPos,
                left->resultType != DataTypes::typeFloat && right->resultType != DataTypes::typeFloat,
                x->op == "%"
                    ? "Invalid operand type: float (Sango2 does not support % instructions for float)"
                    : "Invalid operand type: float");

            x->resultType = DataTypes::typeInt;

        } //else x->op == "="

        //The-most-weirdo: assignments (requires lvalue)
        if(level == 0)
            require(operatorTokenPos, left->isLvalue,
                "assignment operator requires an lvalue as left operand");

    }
}


Parser::Parser(Token* tokenList, int tokenCnt_) {
    tokens = tokenList;
    tokenCount = tokenCnt_;

    typeNameToCode.emplace("void", DataTypes::typeVoid);
    typeNameToCode.emplace("int", DataTypes::typeInt);
    typeNameToCode.emplace("float", DataTypes::typeFloat);
    typeNameToCode.emplace("string", DataTypes::typeString);

    numericNameToCode.emplace("int", 0);
    numericNameToCode.emplace("short", 2);
    numericNameToCode.emplace("byte", 4);

    loadSyscallTable();
    loadIntvTable();

    //NOTE Test
    currentFunc = new Function;
    vector<Var> paramList;
    paramList.push_back((Var){DataTypes::typeInt, "param"});
    initParam(paramList);
    newLocal(DataTypes::typeInt, "i");
    newLocal(DataTypes::typeString, "str");
    newLocal(DataTypes::typeIntPtr, "ptr");

    ExpressionNode* root = NULL;
    int tokenPos = 0;
    expr(root, tokenPos, 0);

    tokenPos++;

}
