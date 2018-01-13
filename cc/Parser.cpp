#include "Parser.h"
#include "Panic.h"

#include <cstdio>
#include <cstring>
#include <sstream>

bool Parser::isPtr(int type) {
    return type >= 10;
}

void Parser::require (int& tokenPos, bool condition, string errmsg) {
    if(!condition)
        Panic::panic(errmsg, tokens[tokenPos].fileName,
            tokens[tokenPos].lineNo, tokens[tokenPos].columnNo);
}

bool Parser::see (int& tokenPos, string s) {
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
        currentFunc->params[params.size() - i - 1] = params[i]; //idx in currentFunc->params[] is -no - 2.
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
    else if(number <= -2)
        return currentFunc->params[-number - 2];
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


bool Parser::tryMatchSyscall(ExpressionNode* &x, int& tokenPos){
    //TODO syscall
    return false;
};

bool Parser::tryMatchInst(ExpressionNode* &x, int& tokenPos){
    //TODO inst
    return false;
};

bool Parser::tryMatchFunccall(ExpressionNode* &x, int& tokenPos){
    //TODO funccall
    return false;
};

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

        //locals
        if(getLocalNoByName(token.content) != -1) {
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

    //TODO not tested.
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

    //TODO not tested.
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

//TODO build framework
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
