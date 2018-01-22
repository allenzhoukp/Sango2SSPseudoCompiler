#include "Parser.h"
#include "Panic.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <regex>

using std::endl;

//==========Auxillary function============

bool Parser::isPtr(int type) {
    //return type >= 10;
    return (type >> 16) != 0;
}

bool Parser::isInteger(int type) {
    return isPtr(type) ||
        type == DataTypes::typeInt || type == DataTypes::typeUInt ||
        type == DataTypes::typeShort || type == DataTypes::typeUShort ||
        type == DataTypes::typeByte || type == DataTypes::typeUByte;
}

bool Parser::isStruct(int type) {
    return !isPtr(type) && type >= DataTypes::typeStructBase;
}

bool Parser::isStructPtr(int type) {
    return isPtr(type) && (type - (1 << 16)) >= DataTypes::typeStructBase;
}

int Parser::getStructIdByType(int type) {
    return (type & 0xFFFF) - DataTypes::typeStructBase;
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

/* @Deprecated
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

    //struct *
    } else if(nextContent == "*") {
        typeName = content + " *";
        StructInfo* info = getStructInfoByName(content);
        if(info->id != -1) {
            result = DataTypes::typeStructPtr + info->id;
            tokmove = 2;
        }
    }
    //TODO struct

    if(match)
        tokenPos += tokmove;

    return result;
}
*/

//This look far more easier to understand. Damn it. Viva la brute force.
int Parser::getType(int& tokenPos, bool match, string& typeName) {
    string content = tokens[tokenPos].content;
    string combinedContent = tokens[tokenPos].content + " " + tokens[tokenPos + 1].content;
    int type = -1;
    int tokmove = 1;
    if(content == "void")
        type = DataTypes::typeVoid;
    else if(content == "int")
        type = DataTypes::typeInt;
    else if(content == "float")
        type = DataTypes::typeFloat;
    else if(content == "string")
        type = DataTypes::typeString;
    else if(content == "short")
        type = DataTypes::typeShort;
    else if(content == "byte")
        type = DataTypes::typeByte;
    else if(content == "unsigned")
        type = DataTypes::typeUInt;
    else if(combinedContent == "unsigned int") {
        type = DataTypes::typeUInt;
        tokmove = 2;
    } else if(combinedContent == "unsigned short") {
        type = DataTypes::typeUShort;
        tokmove = 2;
    } else if(combinedContent == "unsigned byte") {
        type = DataTypes::typeUByte;
        tokmove = 2;
    } else {
        type = getStructInfoByName(content)->id;
        if(type == -1)
            return -1;
        type += DataTypes::typeStructBase;
    }

    int ptrCount = 0;
    while(tokens[tokenPos + tokmove + ptrCount].content == "*")
        ptrCount++;

    typeName = "";
    for(int i = 0; i < tokmove + ptrCount; i++) {
        if(i != 0) typeName += " ";
        typeName += tokens[tokenPos + i].content;
    }

    if(match)
        tokenPos += tokmove + ptrCount;

    return type | (ptrCount << 16);
}

int Parser::getType(int& tokenPos, bool match) {
    string s;
    return getType(tokenPos, match, s);
}

bool Parser::validateType(int target, int given) {
    if(isInteger(target))
        target = DataTypes::typeInt;
    if(isInteger(given))
        given = DataTypes::typeInt;
    return target == given;
}

void Parser::newLocal(int type, string name) {
    Var& var = currentFunc->locals[currentFunc->localCount];
    var.type = type;
    var.name = name;
    var.no = currentFunc->localCount + 3; //local no starts from 3.
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
        return currentFunc->locals[number - 3];
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

void Parser::newLabel (string name) {
    labels.insert(name);
    labelNo++;
}

string Parser::emitLabel (string name) {
    std::ostringstream labelName;
    labelName << "t" << time(0) << "_" << labelNo << "_" << name;
    name = labelName.str();
    newLabel(name);
    return name;
}

bool Parser::findLabel (string name) {
    return labels.find(name) != labels.end();
}

int Parser::getStringNo (string str) {
    if(stringTable.find(str) != stringTable.end())
        return stringTable[str];
    return -1;
}

string Parser::getString (int no) {
    for(auto it = stringTable.begin(); it != stringTable.end(); ++it)
        if(it->second == no)
            return it->first;
    return "";
}

void Parser::newString (string str) {
    stringTable.emplace(str, stringCount++);
}

StructInfo* Parser::getStructInfoByid (int id) {
    for(int i = 0; i < structInfoCount; i++)
        if(structInfos[i].id == id)
            return &(structInfos[i]);
    structInfos[structInfoCount].id = -1;
    return &structInfos[structInfoCount];
}

StructInfo* Parser::getStructInfoByName (string name) {
    for(int i = 0; i < structInfoCount; i++)
        if(toCamel(structInfos[i].name) == toCamel(name))
            return &structInfos[i];
    structInfos[structInfoCount].id = -1;
    return &structInfos[structInfoCount];
}

string Parser::toCamel (string name) {
    bool containLower = false;
    for(int i = 0; i < name.length(); i++){
        if(islower(name[i])){
            containLower = true;
            break;
        }
    }

    if(containLower) return name;

    //First letter unchanged.
    //i.e. SANGO_OBJECT => SangoObject, _WNDCLASS => _wndclass
    for(int i = 1; i < name.length(); i++) {
        name[i] = tolower(name[i]);
        if(name[i] == '_') {
            name.erase(i, 1);
            if(i < name.length())
                name[i] = toupper(name[i]);
        }
    }

    return name;
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
    loadStructTable();

    //NOTE Test
    currentFunc = new Function;
    vector<Var> paramList;
    paramList.push_back((Var){DataTypes::typeInt, "param"});
    initParam(paramList);

    int tokenPos = 0;
    line(tokenPos);
    printf("%s", out.str().c_str());

    tokenPos++;

}
