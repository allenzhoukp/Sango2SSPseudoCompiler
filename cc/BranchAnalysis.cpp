#include "Parser.h"
#include "Localization.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <regex>

using std::endl;

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
    tokenPos++;
    return true;
}

//TODO: if, while, for and switch are NOT TESTED.
//Temporary dealing with matchExpr() (tree destruction) instead.
bool Parser::tryMatchIf (int& tokenPos) {
    if(!tryMatch(tokenPos, "if"))
        return false;
    string ifStart = emitLabel("if");
    string ifElse = emitLabel("else");
    string ifEnd = emitLabel("if_end");

    match(tokenPos, "(");
    matchExpr(tokenPos, true, DataTypes::typeInt);
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
	matchExpr(tokenPos, true, DataTypes::typeInt);
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
	matchExpr(tokenPos, true, DataTypes::typeInt); //won't match ";"
	match(tokenPos, ";");
	out << "\t" << "JZ " << breakTo << endl;

	//increment
	//backup these contents.
	std::ostringstream increment;
	out.swap(increment); //previous content of out is now in increment
	matchExpr(tokenPos, false); //contents will be outputed to out; won't match ";"
	out.swap(increment); //swap: out = previous content, increment = content just outputed

	match(tokenPos, ")");

	//body
	line(tokenPos);

	//loopContinue here
	outputLabel(loopContinue);

	//append increment
	out << increment.str();

	//go to loop start
	out << "\t" << "JMP " << loopStart << endl;

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
	matchExpr(tokenPos, true, DataTypes::typeInt);

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
			ExpressionNode* immediate = NULL;
			object(immediate, tokenPos);
			require(tokenPos,
				immediate->type == ExpNodeType::intConst,
				ErrMsg::requireIntegerConstant);
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

bool Parser::tryMatchLabelDecl (int& tokenPos) {
    if(tokens[tokenPos + 1].content != ":")
        return false;

    //Shouldn't match existing label
    //NOTE it is not possible to totally avoid conflict. Those lines in __asm block remains unknown to us.
    require(tokenPos, !findLabel(tokens[tokenPos].content),
        ErrMsg::labelRedefined);

    newLabel(tokens[tokenPos].content);
    tokenPos += 2;
    return true;
}

bool Parser::tryMatchJumps (int& tokenPos) {
    if(tryMatch(tokenPos, "break")) {
        out << "\t" << "JMP " << breakToLabels.top() << endl;

    } else if (tryMatch(tokenPos, "continue")) {
        out << "\t" << "JMP " << loopToLabels.top() << endl;

    } else if (tryMatch(tokenPos, "goto")) {
        if(!findLabel(tokens[tokenPos].content)) {
            printf(ErrMsg::labelNotFound,
                tokens[tokenPos].fileName, tokens[tokenPos].lineNo, tokens[tokenPos].columnNo, tokens[tokenPos].content.c_str());
        }
        out << "\t" << "JMP " << tokens[tokenPos].content << endl;
        tokenPos++;

    } else if (tryMatch(tokenPos, "return")) {
        if(currentFunc->returnType != DataTypes::typeVoid) {
            matchExpr(tokenPos, true, currentFunc->returnType);
            //int popToStack = (currentFunc->paramCount == 0 ? -2 : (-1 - currentFunc->paramCount));
            //out << "\t" << "POPN " << popToStack << endl;
            //out << "\t" << "RETN " << (-2 - popToStack) << endl;
            out << "\t" << "INST_01 " << currentFunc->paramCount << endl;

        } else {
            out << "\t" << "RETN " << currentFunc->paramCount << endl;
        }
        lineReturned = true;

    } else return false;

    match(tokenPos, ";");
    return true;
}

bool Parser::tryMatchLocalDecl (int& tokenPos) {
    int newTokenPos = tokenPos;
    int type = getType(newTokenPos, true);
    if(type == 0 || type == -1) //should not be void or -1.
        return false;
    tokenPos = newTokenPos - (type >> 16); //ignore all stars! You can decl int and int* and int** and etc. at the same time.
    type &= 0xFFFF;

    do {
        //get count of stars
        int star = 0;
        while(tryMatch(tokenPos, "*")) star++;

        //identifier token required
        require(tokenPos, tokens[tokenPos].type == TokenType::tokenIdent,
            ErrMsg::localRequireIdentifier);

        //void * required
        if(type == 0)
            require(tokenPos, star != 0,
                ErrMsg::voidLocal);

        //create local
        string name = tokens[tokenPos].content;
        int localType = type | (star << 16);
        require(tokenPos, getLocalNoByName(name) == -1,
            ErrMsg::localRedefined + name);
        tokenPos++;

        if(tryMatch(tokenPos, "=")) {
            newLocal(localType, name);
            matchExpr(tokenPos, true, localType);
            out << "\t" << "POPN " << getLocalNoByName(name) << " ; " << name << endl;

        } else if (tryMatch(tokenPos, "[")) {
            ExpressionNode* immediate = NULL;
            object(immediate, tokenPos);
            require(tokenPos, immediate->type == ExpNodeType::intConst,
                ErrMsg::requireIntegerConstant);

            newLocalArray(localType, name, immediate->intValue);
            match(tokenPos, "]");

        } else
            newLocal(localType, name);

    } while (tryMatch(tokenPos, ","));

    match(tokenPos, ";");
    return true;
}

bool Parser::tryMatchAsynccall (int& tokenPos) {
    if(!tryMatch(tokenPos, "asynccall"))
        return false;

    //Get function reference
    string name = tokens[tokenPos].content;
    int funcNameID = getStringNo(name);

    require(tokenPos,
        funcNameID != -1 && funcNameMapping.find(name) != funcNameMapping.end(),
        ErrMsg::functionNotFound1 + name + ErrMsg::functionNotFound2);

    Function* func = funcNameMapping[name];

    //output asynccall name
    out << "\t" << "PUSHSTR " << funcNameID << " ; \"" << name << "\"" << endl;

    //caller body
    tokenPos++; //skip name
    match(tokenPos, "(");

    //get params
    int param = 0;
    if(!see(tokenPos, ")")) do {
        matchExpr(tokenPos, true, func->params[0].type);
        param++;
    } while (tryMatch(tokenPos, ","));

    std::ostringstream errmsg;
    errmsg << ErrMsg::funcParamNotMatch1 << func->name << ErrMsg::funcParamNotMatch2 << func->paramCount <<
        ErrMsg::funcParamNotMatch3 << param;
    require(tokenPos, param == func->paramCount, errmsg.str());

    match(tokenPos, ")");
    match(tokenPos, ";");

    //CALLBS! filled with 0
    while(param < 4) {
        out << "\t" << "PUSH 0" << endl;
        param++;
    }

    //Choose the inst to use.
    if(param == 4)
        out << "\t" << "CALLBS" << endl;
    else
        out << "\t" << "SYSCALL 0x31" << (param - 5) << ", (" << (param + 1) << " | (0 << 16)) ; CALLBS" << endl;

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
    //clear returned flag
    lineReturned = false;

    // all except normal line (blocks and line with an expression body)
    if(!(tryMatchAsm(tokenPos) ||
         tryMatchIf(tokenPos) ||
         tryMatchWhile(tokenPos) ||
         tryMatchFor(tokenPos) ||
         tryMatchSwitch(tokenPos) ||
         tryMatchJumps(tokenPos) ||
         tryMatchLocalDecl(tokenPos) ||
         tryMatchLabelDecl(tokenPos) ||
         tryMatchAsynccall(tokenPos))) {

        //blocks
        if(tryMatch(tokenPos, "{")) {
            while(!see(tokenPos, "}"))
                line(tokenPos);

            match(tokenPos, "}");

        //normal line
        } else {
            matchExpr(tokenPos, false);
            match(tokenPos, ";");
        }
    }
}

//Functions.
void Parser::matchFunc(int& tokenPos) {

    currentFunc = &funcs[funcCount++];

    string typeName;

    //match return type
    currentFunc->returnType = getType(tokenPos, true, typeName);
    require(tokenPos, currentFunc->returnType != -1,
        "invalid return type: " + typeName);

    //NOTE added a special sign "naked". If such sign seen, the function body must be an __asm token.
    bool naked = false;
    if(tryMatch(tokenPos, "naked"))
        naked = true;

    //Downward support: still accepts "function" token.
    tryMatch(tokenPos, "function");

    //Check name conflict and add to function name mapping
    currentFunc->name = tokens[tokenPos].content;
    require(tokenPos, tokens[tokenPos].type == TokenType::tokenIdent,
        ErrMsg::funcNameIdentifier);
    require(tokenPos, funcNameMapping.find(currentFunc->name) == funcNameMapping.end(),
        ErrMsg::funcNameConflict + currentFunc->name);
    funcNameMapping[currentFunc->name] = currentFunc;

    if(getStringNo(currentFunc->name) == -1)
        newString(currentFunc->name); //important: this is for CALLBS use.

    newLabel(currentFunc->name); //Function name is a label as well, though highly not recommended to jump to it...
    out << currentFunc->name << ":" << endl; //Output function name
    tokenPos++;

    match(tokenPos, "(");

    //Match parameters
    vector<Var> params;
    if(!see(tokenPos, ")")) do {
        Var var;

        //typeName
        var.type = getType(tokenPos, true, typeName);
        require(tokenPos - 1, var.type != -1,
            ErrMsg::invalidTypeName + typeName);

        //parameter name validity and conflicts
        require(tokenPos, tokens[tokenPos].type == TokenType::tokenIdent,
            ErrMsg::localRequireIdentifier);

        var.name = tokens[tokenPos].content;
        for(auto it = params.begin(); it != params.end(); ++it){
            if(it->name == var.name){
                require(tokenPos, false,
                    ErrMsg::localRedefined + var.name);
                break;
            }
        }

        //add to params
        params.push_back(var);
        tokenPos++;

    } while (tryMatch(tokenPos, ","));

    //push params vector into params array of this function
    initParam(params);
    match(tokenPos, ")");

    //match callsign
    if(tryMatch(tokenPos, "callsign")) {
        require(tokenPos, tokens[tokenPos].type == TokenType::tokenNum,
            ErrMsg::requireIntegerConstant);
        currentFunc->callsign = tokens[tokenPos]._number;
        tokenPos++;
    }

    //Naked method: Everything is done by the user!
    if(naked) {
        bool asmMatched = tryMatchAsm(tokenPos);
        require(tokenPos, asmMatched,
            ErrMsg::nakedRequireAsm);

    } else {
        //Backup the output - remember the STACK instruction in the front?
        std::ostringstream functionContent;
        out.swap(functionContent);

        //function body.
        //Now it is possible to avoid bracks! Although really weird...
        line(tokenPos);

        //Restore the original output.
        out.swap(functionContent);
        //Adjust stack position
        out << "\t" << "STACK " << currentFunc->localCount + 2 << endl;

        //Output function body
        out << functionContent.str();

        //An extra RETN at the end of funtion when there's no return value
        if(currentFunc->returnType != DataTypes::typeVoid) {
            require(tokenPos, lineReturned,
                ErrMsg::noExplicitReturn);
        } else {
            out << "\t" << "RETN " << currentFunc->paramCount << endl;
        }
    }

    out << endl;
}
