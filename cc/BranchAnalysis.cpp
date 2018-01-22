#include "Parser.h"

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
    tokenPos++;
    return true;
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
			ExpressionNode* immediate = NULL;
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

bool Parser::tryMatchLabelDecl (int& tokenPos) {
    if(tokens[tokenPos + 1].content != ":")
        return false;

    //Shouldn't match existing label
    //NOTE it is not possible to totally avoid conflict. Those lines in __asm block remains unknown to us.
    require(tokenPos, !findLabel(tokens[tokenPos].content),
        "label redefined");

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
            printf("Warning %s, %d:%d: label %s not found in all non-asm blocks. Proceed at your own risk! \n",
                tokens[tokenPos].fileName, tokens[tokenPos].lineNo, tokens[tokenPos].columnNo, tokens[tokenPos].content.c_str());
        }
        out << "\t" << "JMP " << tokens[tokenPos].content << endl;
        tokenPos++;

    } else if (tryMatch(tokenPos, "return")) {
        if(currentFunc->returnType != DataTypes::typeVoid) {
            expr(tokenPos, true);
            int popToStack = (currentFunc->paramCount == 0 ? -2 : (-1 - currentFunc->paramCount));
            out << "\t" << "POPN " << popToStack << endl;
            out << "\t" << "RETN " << (-2 - popToStack) << endl;

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
            "invalid local variable name: it should only contains letters, numbers and '_', and should not start with numbers");

        //void * required
        if(type == 0)
            require(tokenPos, star != 0,
                "invalid local variable type: void");

        //create local
        string name = tokens[tokenPos].content;
        int localType = type | (star << 16);
        require(tokenPos, getLocalNoByName(name) == -1,
            "there has already been a local variable named " + name);
        newLocal(localType, name);
        tokenPos++;

        if(tryMatch(tokenPos, "=")) {
            expr(tokenPos, true);
            out << "\t" << "POPN " << getLocalNoByName(name) << " ; " << name << endl;

        } else if (tryMatch(tokenPos, "[")) {
            ExpressionNode* immediate = NULL;
            object(immediate, tokenPos);
            require(tokenPos, immediate->type == ExpNodeType::intConst,
                "array length should be an explicit integer constant");

            //it requires a name! Or tryMatchAsm() will be stuck by regex searching.
            char buf[MAX_LINE_LEN];
            for(int i = 0; i < immediate->intValue; i++){
                sprintf(buf, "%d", i);
                newLocal(localType, "__" + name + "_" + string(buf));
            }
            match(tokenPos, "]");
        }

    } while (tryMatch(tokenPos, ","));

    match(tokenPos, ";");
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
         tryMatchLabelDecl(tokenPos))) {

        //blocks
        if(tryMatch(tokenPos, "{")) {
            while(!see(tokenPos, "}"))
                line(tokenPos);

            match(tokenPos, "}");

        //normal line
        } else {
            expr(tokenPos, false);
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

    //Downward support: still accepts "function" token.
    tryMatch(tokenPos, "function");

    //Check name conflict and add to function name mapping
    currentFunc->name = tokens[tokenPos].content;
    require(tokenPos, tokens[tokenPos].type == TokenType::tokenIdent,
        "invalid function name: it should only contains letters, numbers and '_', and should not start with numbers");
    require(tokenPos, funcNameMapping.find(currentFunc->name) == funcNameMapping.end(),
        "there has already been a function named " + currentFunc->name);
    funcNameMapping[currentFunc->name] = currentFunc;
    tokenPos++;

    match(tokenPos, "(");

    //Match parameters
    vector<Var> params;
    if(!see(tokenPos, ")")) do {
        Var var;

        //typeName
        var.type = getType(tokenPos, true, typeName);
        require(tokenPos - 1, var.type != -1,
            "invalid type name: " + typeName);

        //parameter name validity and conflicts
        require(tokenPos, tokens[tokenPos].type == TokenType::tokenIdent,
            "invalid parameter name: it should only contains letters, numbers and '_', and should not start with numbers");

        var.name = tokens[tokenPos].content;
        for(auto it = params.begin(); it != params.end(); ++it){
            if(it->name == var.name){
                require(tokenPos, false,
                    "there has already been a parameter or local variable named " + var.name);
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
            "callsign of a function must be an integer");
        currentFunc->callsign = tokens[tokenPos]._number;
        tokenPos++;
    }

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
            "no explicit return at the end of the function body");
    } else {
        out << "\t" << "RETN " << currentFunc->paramCount << endl;
    }

}
