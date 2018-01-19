#include "Parser.h"
#include "Panic.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <regex>

using std::endl;

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
        x->sys = syscall;
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
        x->func = func;
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

StructMember Parser::matchMember (int& tokenPos, int structType) {
    StructInfo* info = getStructInfoByid(structType - DataTypes::typeStructPtr);
    require(tokenPos, info->id != -1,
        "Invalid struct * type");
    int memberIndex = -1;
    for(int i = 0; i < info->memberCount; i++) {
        if(info->members[i].name == tokens[tokenPos].content) {
            memberIndex = i;
            break;
        }
    }
    require(tokenPos, memberIndex != -1,
        "struct member '" + tokens[tokenPos].content + "' not found in struct '" + info->name + "'");

    tokenPos++;
    return info->members[memberIndex];
}

ExpressionNode* Parser::createMemberNode (StructMember member) {
    ExpressionNode* memberNode = expNodePool.newNode();
    memberNode->type = ExpNodeType::intConst;
    memberNode->isLvalue = false;
    memberNode->intValue = member.offset;
    memberNode->resultType = member.type;
    return memberNode;
}

// Dot and struct array: match the right operand of operator ->.
// Returns the type of the final offset.
// When . and [](for struct member) are included, they became the highest priority, and the result can be written as an integer.
// e.g. p->pSoldierInfo[soldierID].pObject; p is of type BattleUserInfo *, and the result will be
//      *(p + OFFSET(pSoldierInfo) + soldierID * sizeof(SoldierInfo) + OFFSET(pObject)) (if it is an rvalue).
//      Notice that it is the same as *(p + some_offset). So it could be written as p->some_offset.
//      (That's why . and [](for struct member) has higher priority than -> !)
//      First call for array, result is SoldierInfo(a structure);
//      The result is a structure (not a pointer) which cannot be directly accessed, so recurse for the dot.
//      Final result is the type of pObject (SangoObject *).
int Parser::dotAndStructArray (ExpressionNode* &nodePos, int& tokenPos, int structOrArrayType) {
    bool isResultAnArray = false;
    if(tryMatch(tokenPos, ".")) {
        //oldOffset + newOffset = right operand of operator ->
        StructMember dotMember = matchMember(tokenPos, structOrArrayType - StructMember::STRUCT);
        ExpressionNode* dotMemberNode = createMemberNode(dotMember);
        ExpressionNode* left = nodePos;
        ExpressionNode* x = expNodePool.newNode();
        x->type = ExpNodeType::binaryOp;
        x->op = "+";
        x->resultType = DataTypes::typeInt;
        x->isLvalue = false;
        x->left = left;
        x->right = dotMemberNode;
        nodePos = x;

        structOrArrayType = dotMember.type;
        isResultAnArray = dotMember.isArray;

    } else if(tryMatch(tokenPos, "[")) {
        //arrayHeadOffset + size of array type * index = right operand of operator ->
        ExpressionNode* x = expNodePool.newNode();
        x->left = nodePos;
        nodePos = x;

        x->type =ExpNodeType::binaryOp;
        x->op = "+";
        x->resultType = DataTypes::typeInt;
        x->isLvalue = false;

        ExpressionNode* y = expNodePool.newNode();
        y->type = ExpNodeType::binaryOp;
        y->op = "*";
        y->resultType = DataTypes::typeInt;
        y->isLvalue = false;
        x->right = y;

        y->right = expNodePool.newNode();
        y->right->type = ExpNodeType::intConst;
        y->right->resultType = DataTypes::typeInt;
        y->right->isLvalue = false;

        if(structOrArrayType == DataTypes::typeInt || structOrArrayType == StructMember::typeUInt ||
           structOrArrayType == DataTypes::typeFloat ||
           isPtr(structOrArrayType == DataTypes::typeInt))
            y->right->intValue = 4;
        else if(structOrArrayType == StructMember::typeShort || structOrArrayType == StructMember::typeUShort)
            y->right->intValue = 2;
        else if(structOrArrayType == StructMember::typeByte || structOrArrayType == StructMember::typeUByte)
            y->right->intValue = 1;
        else if(structOrArrayType >= StructMember::STRUCT &&
                getStructInfoByid(structOrArrayType - DataTypes::typeStructPtr)->id != -1)
            y->right->intValue = getStructInfoByid(structOrArrayType - DataTypes::typeStructPtr)->size;
        else
            require(tokenPos, false, "invalid type for a struct-member array");

        expr(y->left, tokenPos);
        match(tokenPos, "]");

    } else {
        require(tokenPos, false, "invalid member access: structures or arrays in structures cannot be directly accessed");
    }

    if(structOrArrayType >= StructMember::STRUCT || isResultAnArray)
        return dotAndStructArray (nodePos, tokenPos, structOrArrayType);
    else
        return structOrArrayType;
}

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

    // Indeed complicated. New operator introduced for this!
    } else if (tryMatch(tokenPos, "->")) {

        require(operatorTokenPos,
            left->resultType >= DataTypes::typeStructPtr,
            "Operator -> can only be applied to struct *");

        StructMember directMember = matchMember(left->resultType);
        ExpressionNode* directMemberNode = createMemberNode(directMember);

        //struct: must have a . operator (cannot directly access a structure!)
        //array: must have a [] operator (byte[] does not mean string for now.)
        if(directMember.type >= StructMember::STRUCT || directMember.isArray)
            x->ptrMemberType = dotAndStructArray(directMemberNode, tokenPos, directMember.type);
        else
            x->ptrMemberType = directMember.type;

        //TODO !!! looks like there is severe problem here, for the difference between resultType and ptrMemberType.
        // this is indeed confusing. The more confusing thing is, there is even difference between string and string * in memory!
        // We need to BE SURE what ptrMemberType of operator -> is.
        //
        // Let's take an example. In struct A there is some member of int *, and let's name it i.
        // The result type of p->i should be int *. And for the pointer (p + offset(i)), it has an int ** type!
        //
        // In this case, in the exp tree, the result type for p->i must be int*. So resultType should be int*.
        // In analysis, when it comes to ->, we need to tell outputGetValue()/outputSetValue() that, int* is an int.
        // So it is clear: when member type is pointer, float, string, or things you can understand, just set resultType for dat.
        // When member type is short, byte, or unsigned stuffs, you need to record something to tell the output which func is needed.
        // THE BEST WAY?
        // Include all types. 0=void, 1=int, 2=string, 3=float, 4=short, 5=byte, 6=uint, 7=ushort, 8=ubyte. 10 + struct id = structures.
        // For pointer, use the high bits. 3 | (2 << 16) is float **.
        // Problem? Huge paraphrasing. Add an isNumeric() (returns type>>16 != 0 || type == int || type == ....). Change all type checks.

        //all numerics goes to int!
        x->resultType = x->ptrMemberType;
        if(x->resultType == StructMember::typeUInt ||
           x->resultType == StructMember::typeShort || x->resultType == StructMember::typeUShort ||
           x->resultType == StructMember::typeByte || x->resultType == StructMember::typeUByte)
           x->resultType = DataTypes::typeInt;

        x->type = ExpNodeType::binaryOp;
        x->op = "->";
        x->isLvalue = true; //Indeed.

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
        // + and comparisons
        if(x->op == "+" || x->op == "+=" || level == 3) {

            // for strings: can only compare string w/string, and link string w/string
            if(left->resultType == DataTypes::typeString)
                require(operatorTokenPos,
                    right->resultType == DataTypes::typeString,
                    "Can't apply operator " + x->op + " between a string and a non-string");

            // logic result.
            // int == float -> float == float
            if(level == 3)
                x->resultType = DataTypes::typeInt;

            // +
            else {
                //string + string = string
                if(left->resultType == DataTypes::typeString)
                    x->resultType = DataTypes::typeString;

                // int + int = int, float + int or float = float
                else if(x->op != "+=")
                    x->resultType = (left->resultType == DataTypes::typeFloat || right->resultType == DataTypes::typeFloat)
                        ? DataTypes::typeFloat : DataTypes::typeInt;

                // follow the type of the left
                else
                    x->resultType = left->resultType;
            }

        // int, float, but not string.
        // *,/,-
        } else if(x->op == "*" || x->op == "/" || x->op == "-" ||
                  x->op == "*=" || x->op == "/=" || x->op == "-=") {
            require(operatorTokenPos,
                left->resultType != DataTypes::typeString && right->resultType != DataTypes::typeString,
                "Invalid operand type: string");

            if(x->op[1] != '=')
                // If one of operands has float value, then the result is float. int otherwise.
                x->resultType = (left->resultType == DataTypes::typeFloat || right->resultType == DataTypes::typeFloat)
                    ? DataTypes::typeFloat : DataTypes::typeInt;
            else
                // Follow the type of the left.
                x->resultType = left->resultType;

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
