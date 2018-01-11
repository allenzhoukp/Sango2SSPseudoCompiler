#include "Parser.h"
#include "Panic.h"

#include <cstdio>
#include <cstring>

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
//if match == true, when failed to find a type, panic.
//the name of the type will be written to typeName.
int Parser::getType (int& tokenPos, bool match, string& typeName) {

    typeName = "";

    string content = tokens[tokenPos].content;

    //prevent exceeding.
    string nextContent = tokenPos + 1 < tokenCount ? tokens[tokenPos + 1].content : "";

    //void, int, string, float.
    if(typeNameToCode.find(content) != typeNameToCode.end()) {

        //void*, string*, float*.
        if(nextContent == "*" && content != "int") {
            return 10 + typeNameToCode[content];
        }
        typeName = content;

        return typeNameToCode[content];
    }

    if(content == "unsigned") {
        //unsigned *
        if(nextContent == "*") {
            typeName = "unsigned *";
            return DataTypes::typeUIntPtr;
        }

        //unsigned int *, unsigned short *, unsigned byte *
        if(numericNameToCode.find(nextContent) != numericNameToCode.end()) {
            string thirdContent = tokenPos + 2 < tokenCount ? tokens[tokenPos + 2].content : "";
            if(thirdContent == "*") {
                typeName = "unsigned " + nextContent + " *";
                return 110 + numericNameToCode[nextContent] + 1;
            }
        }

        if(match)
            require(tokenPos, false, "Error: invalid type name: unsigned " + nextContent + ". \n" +
                "\t Note: for unsigned int/short/byte, only their pointers are supported.");

        return -1;
    }

    //int *, short *, byte *
    if(numericNameToCode.find(content) != numericNameToCode.end()) {
        if(nextContent == "*") {
            typeName = content + " *";
            return 110 + numericNameToCode[nextContent];
        }

        if(match)
            require(tokenPos, false, "Error: invalid type name: " + content + ". \n" +
                    "\t Note: for short/byte, only their pointers are supported.");

        return -1;
    }

    //TODO struct *
    if(nextContent == "*") {
        typeName = content + " *";
        return DataTypes::typeStructPtr;
    }

    if(match)
        require(tokenPos, false, "Error: invalid type name: '" + content + "'");

    return -1;
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

ExpressionNode* Parser::ExpressionNodePool::newNode() {
    return &pool[len++];
}

void Parser::ExpressionNodePool::clear() {
    len = 0;
    memset(pool, 0, sizeof(pool));
    //NOTE this implies isLvalue=false in default.
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

    //...TODO [], ->, ++, --.
    return;

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

    //TODO back() ([], ++, --, ->)
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

    } else if (tryMatch(tokenPos, "*")) {
        unary(x->left, tokenPos);
        //TODO get addr

    //TODO not tested.
    } else if (see(tokenPos, "(")) {
        //cast
        int newTokenPos = tokenPos + 1;
        string typeName = "";
        int typeCode = getType(newTokenPos, false, typeName);

        if(typeCode != -1) {
            tokenPos = newTokenPos;
            match(tokenPos, ")");
            x->type = ExpNodeType::unaryOp;

            // I don't know why to_string(int) doesn't work. Lazy compiler author?
            char buf[10];
            sprintf(buf, "(%d)", typeCode);
            x->op = buf;

            x->isLvalue = false;
            x->resultType = typeCode;

            expr(x->left, tokenPos, 0);

            //Failed cast: float -> ptr, string -> ptr, string -> float, string -> int
            //Don't give actual instruction for now.
            require(currentTokenPos,
                typeCode == x->left->resultType ||
                    (!((isPtr(typeCode) && x->left->resultType == DataTypes::typeFloat)
                        || x->left->resultType == DataTypes::typeString)),
                "invalid type cast to (" + typeName + ")");

            return;
        }
        // else not a cast. go to next line.
    }
    //else
    object(x, tokenPos);

}

void Parser::expr(ExpressionNode* &x, int& tokenPos, int priority){
    //TODO expr()
    //O WTF...
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

    ExpressionNode* root = expNodePool.newNode();
    int tokenPos = 0;
    unary(root, tokenPos);

    tokenPos++;

}
