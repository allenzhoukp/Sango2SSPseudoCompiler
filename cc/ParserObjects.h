#ifndef PARSER_OBJECTS_H
#define PARSER_OBJECTS_H

#include <string>
#include <map>

using std::string;
using std::map;

const int MAX_LOCAL_NUM = 1024;
const int MAX_PARAM_NUM = 32;

namespace DataTypes{
    const int typeVoid = 0;
    const int typeInt = 1; const int typeString = 2; const int typeFloat = 3;

    const int typeVoidPtr = 10;

    const int typeIntPtr = 110; const int typeShortPtr = 112; const int typeBytePtr = 114;
    const int typeUIntPtr = 111; const int typeUShortPtr = 113; const int typeUBytePtr = 115;

    const int typeStringPtr = 12; const int typeFloatPtr = 13;

    const int typeStructPtr = 140;
    //All types of different structures are as follows: 140 + TYPE_ID
};

struct Var {
    int type;
    string name;
    int no;
};

struct Syscall {
    string name;
    int code;
    int returnType;
    Var params[MAX_PARAM_NUM];
    int paramCount = 0;
};

struct Function {
    int returnType = DataTypes::typeVoid;
    string name;
    int callsign = 0;

    Var locals[MAX_LOCAL_NUM];
    Var params[MAX_PARAM_NUM];
    int paramCount = 0;
    int localCount = 0;

    map<string, int> numberMapping;
};

struct Intv {
    int type;
    string name;
    int intvNo;
};

struct ExpressionNode {
    enum Type_{
        intConst, strConst,
        local, intv,
        inst, funcCall, syscall,
        unaryOp, binaryOp
    } type;

    int resultType;
    bool isLvalue = false;

    int intValue; //for intConst
    string strValue; //for strConst
    Var localVar; //for local
    Intv intvVar; //for INTV

    //for inst/funccall/syscall
    string name;
    ExpressionNode** params;

    //for operators
    string op;
    ExpressionNode *left;
    ExpressionNode *right;
};
typedef ExpressionNode::Type_ ExpNodeType;

#endif
