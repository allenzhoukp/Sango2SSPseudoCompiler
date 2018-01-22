#ifndef PARSER_OBJECTS_H
#define PARSER_OBJECTS_H

#include <string>
#include <map>

using std::string;
using std::map;

const int MAX_LOCAL_NUM = 1024;
const int MAX_PARAM_NUM = 32;
const int MAX_MEMBER_COUNT = 1024;

namespace DataTypes{
    const int typeVoid = 0;
    const int typeInt = 1; const int typeString = 3; const int typeFloat = 2;
    const int typeShort = 4; const int typeByte = 5;
    const int typeUInt = 6; const int typeUShort = 7; const int typeUByte = 8;
    const int typeStructBase = 10;

    const int typeVoidPtr = 0 | (1 << 16);

    const int typeIntPtr = typeInt | (1 << 16); const int typeShortPtr = typeShort | (1 << 16); const int typeBytePtr = typeByte | (1 << 16);
    const int typeUIntPtr = typeUInt | (1 << 16); const int typeUShortPtr = typeUShort | (1 << 16); const int typeUBytePtr = typeUByte | (1 << 16);

    //NOTE For numeric * we have PUSHINV, for string * we have SYSCALL 0x200.
    //But nothing for float * for now.
    const int typeStringPtr = typeString | (1 << 16); //const int typeFloatPtr = 12;

    const int typeStructPtr = typeStructBase | (1 << 16);
    //All types of different structures are as follows: typeStructPtr + TYPE_ID
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
    int paramCount;
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

// these types are a little bit different.
// int, byte, short, float, string,
// string *, struct *.
// Specially, there is struct (like SOLDIER_INFO in BATTLE_USER_INFO) and arrays. That's it.

struct StructMember {
    string name;
    int offset;
    int type;
    string typeName;
    bool isArray;

    static const int STRUCT = 500; //DataType base for struct.
    static const int typeUInt = 411;
    static const int typeShort = 412;
    static const int typeByte = 414;
    static const int typeUShort = 413;
    static const int typeUByte = 415;
};

struct StructInfo {
    int id;
    string name;
    int size;
    int memberCount;
    StructMember members[MAX_MEMBER_COUNT];
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

    string name;    //for inst
    Function* func; //for funcCall
    Syscall* sys; //for syscall
    ExpressionNode** params;

    //for operators
    string op;
    ExpressionNode *left;
    ExpressionNode *right;

    //for operator ->
    int accessType;
};
typedef ExpressionNode::Type_ ExpNodeType;

#endif
