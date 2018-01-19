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
    labelName << "t" << time(0) << "_" << labelNo << "_" << name << endl;
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
            name.erase(i);
            if(i < name.length())
                name[i] = toupper(name[i]);
        }
    }

    return name;
}




//=================Line, Function, Branches==================

void Parser::outputLabel(string name) {
    out << name << ":\n";
}

//match __asm{} block.
//will only replace all locals, params, strings, intvs. Does not check grammar.
//NOTE will replace all local/param names that look the same! Especially in comments...
bool Parser::tryMatchAsm (int& tokenPos) {
    //__asm block will clear out all the unnecessary information, like '__asm{' and '}'.
    //only the actual code will remain.
    Token& token = tokens[tokenPos];
    if(token.type != TokenType::tokenAsm)
        return false;

    string content = token.content;
    std::size_t pos;
    char buf[MAX_LINE_LEN];
    std::smatch sm;

    //replace local variable names
    for(int i = 0; i < currentFunc->localCount; i++) {
        sprintf(buf, "%d", currentFunc->locals[i].no);
        std::regex rLocal ("\\b(" + currentFunc->locals[i].name + ")\\b");
        while(std::regex_search(content, sm, rLocal))
            content.replace(sm.position(1), sm[1].str().length(), string(buf));
    }

    //replace param variable names
    for(int i = 0; i < currentFunc->paramCount; i++) {
        sprintf(buf, "%d", currentFunc->params[i].no);
        std::regex rParam ("\\b(" + currentFunc->params[i].name + ")\\b");
        while(std::regex_search(content, sm, rParam))
            content.replace(sm.position(1), sm[1].str().length(), string(buf));
    }

    //replace strings
    //only possible: PUSHSTR "xxx"
    //Greedy .*: prevent matching \" or a " in the next line
    std::regex rPushstr { R"([Pp][Uu][Ss][Hh][Ss][Tt][Rr]\s+(".*"))" };
    while(std::regex_search(content, sm, rPushstr)) {
        //get rid of ""
        string str = sm[1].str().substr(1, sm[1].str().length() - 2);
        if(getStringNo(str) == -1)
            newString(str); //doesn't exist before: add to string_table

        sprintf(buf, "%d", getStringNo(str)); //...then the stringNo won't be -1.
        //+2: "" are included in the seq to replace!
        content.replace(sm.position(1), str.length() + 2, string(buf));
    }

    //replace intv names
    for(int i = 0; i < intvCount; i++) {
        sprintf(buf, "%d", intvs[i].intvNo);
        std::regex rIntv ("\\b(" + intvs[i].name + ")\\b");
        while(std::regex_search(content, sm, rIntv))
            content.replace(sm.position(1), sm[1].str().length(), string(buf));
    }

    //output asm
    out << content << endl;
}

//TODO: if, while, for and switch are NOT TESTED.
//Temporary dealing with expr() (tree destruction) instead.
bool Parser::tryMatchIf (int& tokenPos) {
    if(!tryMatch(tokenPos, "if"))
        return false;
    string ifStart = emitLabel("if");
    string ifElse = emitLabel("else");
    string ifEnd = emitLabel("if_end");

    match(tokenPos, "(");
    expr(tokenPos, true);
    match(tokenPos, ")");

    out << "\tJZ " << ifElse << "\n";

    outputLabel(ifStart);

    line(tokenPos);

    if(tryMatch(tokenPos, "else")) {
        //without else, why jumping?
        out << "\tJMP " << ifEnd << "\n";
        outputLabel(ifElse);
        line(tokenPos);

    } else  //stil required. U create a jump to "else" before, right?
        outputLabel(ifElse);

    outputLabel(ifEnd);

    return true;
}

bool Parser::tryMatchWhile (int& tokenPos) {

	if(!see(tokenPos, "do") && !see(tokenPos, "while"))
		return false;

    string loopTo = emitLabel("while");
    string breakTo = emitLabel("while_end");
	loopToLabels.push(loopTo);
	breakToLabels.push(breakTo);

	outputLabel(loopTo);

	bool hasDo = tryMatch(tokenPos, "do");

	// do-while body
	if(hasDo)
		line(tokenPos);

	// while brace
	match(tokenPos, "while");
	match(tokenPos, "(");
	expr(tokenPos, true);
	match(tokenPos, ")");

	// condition not satisfied: break
	out << "\t" << "JZ " << breakTo << endl;

	if(hasDo)
		match(tokenPos, ";");

	// while body
	else
		line(tokenPos);

	// loop
	out << "\t" << "JMP " << loopTo << endl;
	outputLabel(breakTo);

	loopToLabels.pop();
	breakToLabels.pop();
	return true;
}

bool Parser::tryMatchFor (int& tokenPos) {
	if(!tryMatch(tokenPos, "for"))
		return false;
	match(tokenPos, "(");

	// init
	// in fact, there should only be variable decl and expr...
	// but i don't know whether the others are valid.
	// NOTE just use a line() here
	line(tokenPos);  //will match ";"

	string loopStart = emitLabel("for");
	string loopContinue = emitLabel("for_continue");
	string breakTo = emitLabel("for_end");
	loopToLabels.push(loopContinue);
	breakToLabels.push(breakTo);

	//loop_start here
	outputLabel(loopStart);

	//condition
	expr(tokenPos, true); //won't match ";"
	match(tokenPos, ";");
	out << "\t" << "JZ " << breakTo << endl;

	//increment
	//backup these contents.
	std::ostringstream increment;
	out.swap(increment); //previous content of out is now in increment
	expr(tokenPos, false); //contents will be outputed to out; won't match ";"
	out.swap(increment); //swap: out = previous content, increment = content just outputed

	match(tokenPos, ")");

	//body
	line(tokenPos);

	//loopContinue here
	outputLabel(loopContinue);

	//append increment
	out << increment.str();

	//go to loop start
	out << "\t" << "JZ " << loopStart << endl;

	//breakTo here
	outputLabel(breakTo);

	loopToLabels.pop();
	breakToLabels.pop();
	return true;
}

bool Parser::tryMatchSwitch (int& tokenPos) {
	if(!tryMatch(tokenPos, "switch"))
		return false;
	match(tokenPos, "(");

	//variable
	expr(tokenPos, true);

	match(tokenPos, ")");

	//I don't think someone can avoid that.
	match(tokenPos, "{");

	//Break to
	string switchEnd = emitLabel("switch_end");
	string defaultCase = emitLabel("switch_default");
	//write a 'continue' in switch may indeed mean something!
	breakToLabels.push(switchEnd);

	//Use a lot of ostringstream for backing up...
	std::ostringstream *content = new std::ostringstream[MAX_FUNC_LEN];
	string *labels = new string[MAX_FUNC_LEN];
	int branchCount = 0;

	while(!tryMatch(tokenPos, "}")) {

		bool isDefault = tryMatch(tokenPos, "default");
		if(!isDefault) {
			match(tokenPos, "case");

			//immediate value
			ExpressionNode* immediate;
			object(immediate, tokenPos);
			require(tokenPos,
				immediate->type == ExpNodeType::intConst,
				"expected an integer constant here");
			int value = immediate->intValue;
            delete immediate;

            char buf[MAX_FUNC_LEN];
			sprintf(buf, "switch_case_%d", value);
			labels[branchCount] = emitLabel(string(buf));

			out << "\t" << "JCOND " << value << ", " << labels[branchCount] << endl;

		// default: pop stack & jmp
		} else {
			out << "\t" << "POP" << endl;
			out << "\t" << "JMP " << defaultCase << endl;
			labels[branchCount] = defaultCase;
		}

		match(tokenPos, ":");

		//case content
		//backup...
		out.swap(content[branchCount]);
		while(!see(tokenPos, "case") &&
				!see(tokenPos, "default") &&
				!see(tokenPos,  "}"))
			line(tokenPos);
		out.swap(content[branchCount]);

		branchCount++;

		//default has to be the last case!
		if(isDefault) {
			match(tokenPos, "}");
			break;
		}
	}

	//output label and content
	for(int i = 0; i < branchCount; i++) {
		outputLabel(labels[i]);
		out << content[i].str();
	}

	breakToLabels.pop();
	delete[] labels;
	delete[] content;
	return true;
}

// in total:
// branches: if, while, for, switch
// __asm block
// return; break; continue; goto;
// label decl
// variable decl
// expr
void Parser::line(int& tokenPos) {
    //TODO FINISH THOSE RATS!
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
    tryMatchAsm(tokenPos);

    printf("%s", out.str().c_str());

    tokenPos++;

}
