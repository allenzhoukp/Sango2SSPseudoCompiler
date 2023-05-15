#include "Parser.h"
#include "Panic.h"
#include "Localization.h"

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
    return isPtr(type) && (type - (1 << 16) < (1 << 16)) && (type - (1 << 16) >= DataTypes::typeStructBase);
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
        Panic::panic(ErrMsg::require1 + s + ErrMsg::require2 + tokens[tokenPos].content + "'",
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
    var.isArray = false;
    currentFunc->numberMapping.emplace(name, var.no);
    currentFunc->localCount++;
}

void Parser::newLocalArray(int type, string name, int len) {
    Var& var = currentFunc->locals[currentFunc->localCount];
    var.type = type;
    var.name = name;
    var.no = currentFunc->localCount + 3; //local no starts from 3.
    currentFunc->numberMapping.emplace(name, var.no);
    currentFunc->localCount++;

    var.isArray = true;
    char buf[MAX_LINE_LEN];
    for(int i = 0; i < len; i++){
        sprintf(buf, "%d", i);
        newLocal(type, "__" + name + "_" + string(buf));
    }
}

void Parser::newLocalExefunc(Exefunc* type, string name) {
    Var& var = currentFunc->locals[currentFunc->localCount];
    var.type = DataTypes::typeExefuncPtr;
    var.exefunc = type;
    var.name = name;
    var.no = currentFunc->localCount + 3; //local no starts from 3.
    currentFunc->numberMapping.emplace(name, var.no);
    currentFunc->localCount++;
}

void Parser::initParam(vector<Var> params) {
    for(int i = 0; i < params.size(); i++) {
        params[i].no = -(params.size() - i + 1);  //e.g. 7 params, first (idx=0) param has number -8. idx=1, no=-7, etc.
        params[i].isArray = false;

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
    if (configs["use_timed_label"] == "1") 
        labelName << "t" << time(0) / 60 << "_" << labelNo << "_" << name;
    else
        labelName << "l" << labelNo << "_" << name;
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
    stringTable.emplace(str, stringCount);
    stringCount++;
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

// Aux: unescape a sequence.
std::string Parser::unescape(const string& s)
{
    string res, ss;
    string::const_iterator it = s.begin();
    bool wChar = false;
    while (it != s.end())
    {
        char c = *it++;

        if (!wChar) {

            if (c < 0 && c != -0x80) {
                wChar = true;

            } else if (c == '\\' && it != s.end()) {

                switch (*it++) {
                case '\\': c = '\\'; break;
                case 'n': c = '\n'; break;
                case 't': c = '\t'; break;
                case 'a': c = '\a'; break;
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'r': c = '\r'; break;
                case 'v': c = '\v'; break;
                case '\'': c = '\''; break;
                case '\"': c = '\"'; break;
                case '\?': c = '?'; break;
                case 'x':
                    ss = "  ";
                    ss[0] = *it++; ss[1] = *it++;
                    c = stoi(ss, 0, 16);
                break;
                default: 
                    // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
                    continue;
                }
            }
            
        } else {
            wChar = false;
        }
        res += c;
    }
    return res;
}

void Parser::outputStringDefs () {
    vector<string> strings;
    strings.resize(stringCount);
    for(auto it = stringTable.begin(); it != stringTable.end(); ++it)
        strings[it->second] = it->first;
    // v0.9.7: try to change the string defs
    for(int i = 0; i < stringCount; i++) {
        out << "STRING_" << i << ": " << " ; \"" << strings[i] << "\""<< std::hex << endl;
        string unescaped = unescape(strings[i]);
        const char * s = unescaped.c_str();
        // out << strings[i] << endl;
        // out << s << endl;
        int dwpos = 0;
        int dw = 0;
        for (; *s; s++) {
            dw |= (*(unsigned char *)s) << dwpos;
            dwpos += 8;
            if (dwpos == 32) {
                out << "         db 0x" << dw << endl;
                dw = dwpos = 0;
            }
        }
        // finalize
        out << "         db 0x" << dw << std::dec << endl;
    }
    out << endl;
}

void Parser::outputMagicTable () {
    out << "magic_table_addr:" << endl;
    for(int i = 0; i < funcCount; i++)
        out << "MAGIC_" << i << ": procedure "
            << funcs[i].name << " "
            << funcs[i].paramCount << " "
            << "STRING_" << getStringNo(funcs[i].name) << " "
            << funcs[i].callsign << endl;
    out << endl;
    out << "magic_table_end:" << endl;
    out << endl;
}

void Parser::outputStringTable () {
    out << "string_table_addr:" << endl;
    for(int i = 0; i < stringCount; i++)
        out << "DB STRING_" << i << endl;
    out << endl;
    out << "string_table_end:" << endl;
}

// Reduced redundant labels - Henryshow compiler only support 
// 3 consecutive labels. Might raise problems.
string Parser::reduceLabels (string output) {

    if (configs["reduce_redundant_labels"] != "1")
        return output;

    // construct regexs
    string strRegexLabel;
    if (configs["use_timed_label"] == "1")
        strRegexLabel = R"((t\d+_\d+_\w+):\s*)";
    else 
        strRegexLabel = R"((l\d+_\w+):\s*)";
    std::regex rLabel { strRegexLabel };
    std::regex rConsecutiveLabels { "(" + strRegexLabel + "\\n){2,}" };
    std::smatch sm, sm2;

    // find all consecutive labels

    while(std::regex_search(output, sm, rConsecutiveLabels)) {
        string consecutive = sm.str();
        string firstLabel = "";
        // get all labels in the consecutive sequence
        while(std::regex_search(consecutive, sm2, rLabel)) {
            // replace all non-first labels into first labels
            // (will go through all jumps)
            if (firstLabel == "") firstLabel = sm2[1];
            else {
                string currentLabel = sm2[1];
                size_t start_pos = 0;
                while((start_pos = output.find(currentLabel, start_pos)) != string::npos){
                    output.replace(start_pos, currentLabel.length(), firstLabel);
                    start_pos += firstLabel.length();
                }
            }
            consecutive = sm2.suffix();
        }
        // replace the label into a single one
        std::regex_search(output, sm, rConsecutiveLabels);
        output.replace(sm.position(), sm.str().length(), firstLabel + ":\n");

    }
    return output;
}


Parser::Parser(){};

void Parser::init(Token* tokenList, int tokenCnt_) {
    tokens = tokenList;
    tokenCount = tokenCnt_;


    typeNameToCode.emplace("void", DataTypes::typeVoid);
    typeNameToCode.emplace("int", DataTypes::typeInt);
    typeNameToCode.emplace("float", DataTypes::typeFloat);
    typeNameToCode.emplace("string", DataTypes::typeString);

    numericNameToCode.emplace("int", 0);
    numericNameToCode.emplace("short", 2);
    numericNameToCode.emplace("byte", 4);

    loadConfig();
    loadSyscallTable();
    loadIntvTable();
    loadStructTable();

    //asm header
    out << "include \"predefine.inc\"" << endl;
    out << "org 40h" << endl;
    out << endl;

    //body: functions
    int tokenPos = 0;
    while(tokenPos < tokenCount)
        matchFunc(tokenPos);

    //asm trailer
    outputStringDefs();
    outputMagicTable();
    outputStringTable();

    out << "file_end:" << endl;
    out << "DB \"Script Object Creator: Henryshow\"" << endl;
    out << "DB \"Pseudo Compiler Author: Allen Zhou\"" << endl;

    //TEST
    //printf("%s", out.str().c_str());
}

string Parser::str () {
    return reduceLabels(out.str());
}
