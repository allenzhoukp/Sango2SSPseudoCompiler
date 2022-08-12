#include "Parser.h"
#include "Panic.h"
#include "Localization.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <regex>

using std::endl;

//============Create Expression Tree=============

Parser::ExpressionNodePool::ExpressionNodePool() {
    clear();
    for(int i = 0; i < MAX_LINE_LEN * 10; i++)
        pool[i].params = NULL;
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
    if (x->params == NULL)
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
        errmsg << ErrMsg::parameterNotMatch1 << syscall->paramCount <<
            ErrMsg::parameterNotMatch2 << paramCount;
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

        require(tokenPos, isInteger(x->params[0]->resultType),
            ErrMsg::getGlobalIntParam);

        match(tokenPos, ")");
    }
    else if(tryMatch(tokenPos, "SetGlobal")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeVoid;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 2);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, isInteger(x->params[0]->resultType),
            ErrMsg::setGlobalIntParam);
        match(tokenPos, ",");
        expr(x->params[1], tokenPos, 1);
        require(tokenPos, isInteger(x->params[1]->resultType),
            ErrMsg::setGlobalIntParam);
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
        require(tokenPos, isInteger(x->params[0]->resultType),
            ErrMsg::getIntvIntParam);
        match(tokenPos, ")");
    }
    else if(tryMatch(tokenPos, "SetIntv")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeVoid;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 2);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, isInteger(x->params[0]->resultType),
            ErrMsg::setGlobalIntParam);
        match(tokenPos, ",");
        expr(x->params[1], tokenPos, 1);
        require(tokenPos, isInteger(x->params[1]->resultType),
            ErrMsg::setGlobalIntParam);
        match(tokenPos, ")");
    }

    //delay(), wait(), isRunning() (DELAY, INST_45, INST_46)
    else if(tryMatch(tokenPos, "Delay")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = DataTypes::typeVoid;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 1);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, isInteger(x->params[0]->resultType),
            ErrMsg::delayIntParam);
        match(tokenPos, ")");
    }
    else if(tryMatch(tokenPos, "Wait") || tryMatch(tokenPos, "IsRunning")) {
        if(x != NULL) x = expNodePool.newNode();
        x->resultType = name == "Wait" ? DataTypes::typeVoid : DataTypes::typeInt;

        match(tokenPos, "(");
        initExpNodeParamArray(x, 1);
        expr(x->params[0], tokenPos, 0);
        require(tokenPos, x->params[0]->resultType == DataTypes::typeString,
            name + ErrMsg::instRequireStringParam);
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
        if(x == NULL)
            x = expNodePool.newNode();

        Function* func = funcNameMapping[name];
        x->type = ExpNodeType::funcCall;
        x->func = func;
        x->resultType = func->returnType;
        x->isLvalue = false;
        tokenPos++;

        initExpNodeParamArray(x, func->paramCount);

        match(tokenPos, "(");
        int paramCount = 0;

        if(!see(tokenPos, ")")) do {
            expr(x->params[paramCount], tokenPos, 0);
            require(tokenPos,
                validateType(func->params[paramCount].type, x->params[paramCount]->resultType),
                ErrMsg::paramTypeNotMatch);
            paramCount++;

        } while (tryMatch(tokenPos, ","));

        std::ostringstream err;
        err << ErrMsg::funcParamNotMatch1 << func->name << ErrMsg::funcParamNotMatch2 << func->paramCount <<
            ErrMsg::funcParamNotMatch3 << paramCount;
        require(tokenPos, paramCount == func->paramCount,
             err.str());

        match(tokenPos, ")");
        return true;
    }
    return false;
};


bool Parser::tryMatchExefuncCall(ExpressionNode* &x, int& tokenPos){
    string content = tokens[tokenPos].content;
    if(getLocalNoByName(content) != -1) {
        Var var = getLocalByName(content);
        if (var.type != DataTypes::typeExefuncPtr)
            return false;

        // has to be a function call, otherwise it is just local lvalue
        if (!see(tokenPos + 1, "("))
            return false;

        if(x == NULL)
            x = expNodePool.newNode();

        x->type = ExpNodeType::exefuncCall;
        x->name = content;
        x->localVar = var;
        x->isLvalue = false;
        x->resultType = var.exefunc->returnType;

        tokenPos++; //skip syscall name

        initExpNodeParamArray(x, var.exefunc->paramCount);

        match(tokenPos, "(");
        int paramCount = 0;

        if(!see(tokenPos, ")")) do {
            expr(x->params[paramCount], tokenPos, 0);
            require(tokenPos,
                validateType(var.exefunc->params[paramCount].type, x->params[paramCount]->resultType),
                ErrMsg::paramTypeNotMatch);
            paramCount++;

        } while (tryMatch(tokenPos, ","));

        std::ostringstream err;
        err << ErrMsg::exefuncParamNotMatch1 << content << ErrMsg::funcParamNotMatch2 << var.exefunc->paramCount <<
            ErrMsg::funcParamNotMatch3 << paramCount;
        require(tokenPos, paramCount == var.exefunc->paramCount,
             err.str());

        match(tokenPos, ")");
        return true;
    }
    return false;
};


StructMember Parser::matchMember (int& tokenPos, int structType) {
    StructInfo* info = getStructInfoByid(getStructIdByType(structType));
    require(tokenPos, info->id != -1,
        ErrMsg::invalidStructPtr);
    int memberIndex = -1;
    for(int i = 0; i < info->memberCount; i++) {
        if(info->members[i].name == tokens[tokenPos].content) {
            memberIndex = i;
            break;
        }
    }
    require(tokenPos, memberIndex != -1,
        ErrMsg::structMemberNotFound1 + tokens[tokenPos].content + ErrMsg::structMemberNotFound2 + info->name + "'");

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
int Parser::dotAndArrayInStruct (ExpressionNode* &nodePos, int& tokenPos, int structOrArrayType) {
    bool isResultAnArray = false;
    if(tryMatch(tokenPos, ".")) {
        //oldOffset + newOffset = right operand of operator ->
        StructMember dotMember = matchMember(tokenPos, structOrArrayType);
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

        if(structOrArrayType == DataTypes::typeInt || structOrArrayType == DataTypes::typeUInt ||
           //structOrArrayType == DataTypes::typeFloat || // for now we cannot read float...
           isPtr(structOrArrayType))
            y->right->intValue = 4;
        else if(structOrArrayType == DataTypes::typeShort || structOrArrayType == DataTypes::typeUShort)
            y->right->intValue = 2;
        else if(structOrArrayType == DataTypes::typeByte || structOrArrayType == DataTypes::typeUByte)
            y->right->intValue = 1;
        else if(isStruct(structOrArrayType) &&
                getStructInfoByid(getStructIdByType(structOrArrayType))->id != -1)
            y->right->intValue = getStructInfoByid(getStructIdByType(structOrArrayType))->size;
        else
            require(tokenPos, false, ErrMsg::invalidArrayInStructtype);

        expr(y->left, tokenPos, 0);
        match(tokenPos, "]");

    } else {
        require(tokenPos, false, ErrMsg::directAccess);
    }

    if(isStruct(structOrArrayType) || isResultAnArray)
        return dotAndArrayInStruct (nodePos, tokenPos, structOrArrayType);
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
        if(left->type == ExpNodeType::local && left->localVar.isArray) {
            //The right operand of [] is the index.
            expr(x->right, tokenPos, 0);
            match(tokenPos, "]");

            x->op = "[]";
            x->type = ExpNodeType::binaryOp;
            x->resultType = left->resultType;
            x->isLvalue = true; //indeed.

        //For pointers, it behaves quite like ->.
        //[] is appended only to a pointer (although can be any pointers except void pointers),
        //and the result is *(pointer + offset).
        //For struct pointers, [] results in a struct type. This will incur dotAndArrayInStruct().
        //  However, this situation is NOT array in struct so it still have priority 8 (back).
        //In all, it is a special form of ->.
        } else if (isPtr(left->resultType) && left->resultType != DataTypes::typeVoidPtr) {
            int type = left->resultType - (1 << 16);
            int size;
            StructInfo* info = NULL;
            if(type == DataTypes::typeInt || type == DataTypes::typeUInt)
                size = 4;
            else if(type == DataTypes::typeShort || type == DataTypes::typeUShort)
                size = 2;
            else if(type == DataTypes::typeByte || type == DataTypes::typeUByte /* || type == DataTypes::typeString */)
                size = 1;
            else if(isPtr(type))
                size = 4;
            else {
                info = getStructInfoByid(getStructIdByType(type));
                size = info->size;
            }

            //value in [] will result in offset (value * size). * become x->right and size become y.
            x->right = expNodePool.newNode();
            x->right->type = ExpNodeType::binaryOp;
            x->right->op = "*";
            x->right->isLvalue = false;
            x->right->resultType = DataTypes::typeInt;

            ExpressionNode* y = expNodePool.newNode();
            x->right->right = y;
            y->type = ExpNodeType::intConst;
            y->intValue = size;
            y->isLvalue = false;
            y->resultType = DataTypes::typeInt;

            expr(x->right->left, tokenPos, 0);

            //->
            x->type = ExpNodeType::binaryOp;
            x->resultType = type;
            x->isLvalue = true; //well... it surely does. Imagine pListRoot[10].dwMp = 100;
            x->op = "->";

            match(tokenPos, "]");

            //ptr is a struct: cannot be the result. Find dots.
            if(isStruct(type))
                x->resultType = dotAndArrayInStruct(x->right, tokenPos, type);

        } else
            require(operatorTokenPos, false,
                ErrMsg::notSupportedArray);



    // Indeed complicated. New operator introduced for this!
    } else if (tryMatch(tokenPos, "->")) {

        require(operatorTokenPos,
            isStructPtr(left->resultType),
            ErrMsg::invalidArrowOp);

        StructMember directMember = matchMember(tokenPos, left->resultType);
        ExpressionNode* directMemberNode = createMemberNode(directMember);

        //struct: must have a . operator (cannot directly access a structure!)
        //array: must have a [] operator (byte[] does not mean string for now.)

        //Here, directly record the actual type in the resultType of ->.
        //Or it won't be easy to make an inference.
        if(isStruct(directMember.type) || directMember.isArray)
            x->resultType = dotAndArrayInStruct(directMemberNode, tokenPos, directMember.type);
        else
            x->resultType = directMember.type;

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
        // When taking the value of the address, subtract 1 << 16 from the type.

        x->type = ExpNodeType::binaryOp;
        x->op = "->";
        x->isLvalue = true; //Indeed.
        x->right = directMemberNode;

    } else if (tryMatch(tokenPos, "++") || tryMatch(tokenPos, "--")) {

        require(operatorTokenPos,
            left->isLvalue == true,
            ErrMsg::ppmmLvalue);

        //set global sign.
        if(tokens[operatorTokenPos].content == "++")
            ppNodes.push(x);
        else
            mmNodes.push(x);

        x->op = "b";
        x->type = ExpNodeType::unaryOp;
        x->resultType = left->resultType;
        x->isLvalue = false; //you can't do something like (i++) = 2;

    } else return;

    //Put the new node into the position.
    x->left = left;
    nodePos = x;

    //Recursively getting new back operators.
    //Once not found, it falls in the "else" branch and this return to object().
    back(nodePos, tokenPos);
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

    // v0.9.6 added: float constants
    } else if (token.type == TokenType::tokenFloat) {
        x->type = ExpNodeType::floatConst;
        x->floatValue = token._float;
        x->resultType = DataTypes::typeFloat;
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

        //exefunc calls (local function pointers)
        } else if (tryMatchExefuncCall(x, tokenPos)) {
            x->isLvalue = false;

        //locals
        } else if(getLocalNoByName(token.content) != -1) {
            x->type = ExpNodeType::local;
            x->localVar = getLocalByName(token.content);
            x->resultType = getLocalByName(token.content).type;
            x->isLvalue = true;
            tokenPos++;

            if(x->localVar.isArray && !see(tokenPos, "["))
                require(tokenPos, false,
                    ErrMsg::localArrayDirectAccess);

        //INTVs
        } else if(getIntvNoByName(token.content) != -1) {
            x->type = ExpNodeType::intv;
            x->intvVar = getIntvByName(token.content);
            //x->resultType = getIntvByName(token.content).type;
            x->resultType = DataTypes::typeInt;
            x->isLvalue = true; //INTV can be lvalue! SETINV!
            tokenPos++;

        //Syscalls, Insts, Function calls.
        } else if(tryMatchSyscall(x, tokenPos) ||
                  tryMatchInst(x, tokenPos) ||
                  tryMatchFunccall(x, tokenPos) ){
           x->isLvalue = false;

        } else {
            require(tokenPos, false, ErrMsg::unknownIdentifier + token.content + "'");
            tokenPos++;
        }


    } else if (tryMatch(tokenPos, "(")) {
        expr(x, tokenPos, 0);
        match(tokenPos, ")");

    } else {
        require(tokenPos, false, ErrMsg::expectedExpr + token.content + "'");
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
            ErrMsg::negativeString);

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
            ErrMsg::opNotRequireInt);

        x->resultType = DataTypes::typeInt;
        return;

    } else if (tryMatch(tokenPos, "++") || tryMatch(tokenPos, "--")){
        unary(x->left, tokenPos);

        x->type = ExpNodeType::unaryOp;
        x->op = tokens[currentTokenPos].content;

        //Lvalue required!
        require(currentTokenPos,
            x->left->isLvalue,
            ErrMsg::ppmmLvalue);

        require(currentTokenPos,
            x->left->resultType != DataTypes::typeString,
            ErrMsg::ppmmStringOperand);

        x->isLvalue = false; //You can't do something like (++i) = 6.
        x->resultType = x->left->resultType; //Float, int or ptr.
        return;

    } else if (tryMatch(tokenPos, "*")) {
        unary(x->left, tokenPos);

        require(currentTokenPos,
            isPtr(x->left->resultType),
            ErrMsg::starOnPointers);

        require(currentTokenPos,
            !isStructPtr(x->left->resultType) && x->left->resultType != DataTypes::typeVoidPtr,
            ErrMsg::starOnInvalidPointer);

        x->type = ExpNodeType::unaryOp;
        x->op = "*";
        x->isLvalue = true; //indeed.

        //Directly get the result type by subtracting a * in the type.
        x->resultType = x->left->resultType - (1 << 16);

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

            //Failed cast: float/string -> ptr, everything -> void
            //Don't give actual instruction for now.
            require(currentTokenPos,
                typeCode == x->left->resultType ||
                    (typeCode != DataTypes::typeVoid && 
                    !(isPtr(typeCode) && (x->left->resultType == DataTypes::typeFloat || 
                                          x->left->resultType == DataTypes::typeString))),
                ErrMsg::invalidTypeCast + typeName + ")");

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
            if(left->resultType == DataTypes::typeString || right->resultType == DataTypes::typeString)
                require(operatorTokenPos,
                    left->resultType == DataTypes::typeString && right->resultType == DataTypes::typeString,
                    ErrMsg::bothStringRequired1 + x->op + ErrMsg::bothStringRequired2);

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
                // convert all integers into int
                else if(isInteger(left->resultType))
                    x->resultType = DataTypes::typeInt;
                else
                    x->resultType = left->resultType;
            }

        // int, float, but not string.
        // *,/,-
        } else if(x->op == "*" || x->op == "/" || x->op == "-" ||
                  x->op == "*=" || x->op == "/=" || x->op == "-=") {
            require(operatorTokenPos,
                left->resultType != DataTypes::typeString && right->resultType != DataTypes::typeString,
                ErrMsg::invalidStringOperand);

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
                ErrMsg::invalidStringOperand);

            require(operatorTokenPos,
                left->resultType != DataTypes::typeFloat && right->resultType != DataTypes::typeFloat,
                x->op == "%"
                    ? ErrMsg::invalidModFloat
                    : ErrMsg::invalidFloatOperand);

            x->resultType = DataTypes::typeInt;

        //x->op == "="
        } else
            x->resultType = left->resultType;

        //The-most-weirdo: assignments (requires lvalue)
        if(level == 0)
            require(operatorTokenPos, left->isLvalue,
                ErrMsg::assignmentLvalue);

    }
}
