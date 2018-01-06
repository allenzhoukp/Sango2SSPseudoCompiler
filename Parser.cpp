// --------------------
// Sango2 Smart Script Pseudo Code Compiler v0.4
// This project is written with the help of Sam Nipps' mini-c project.
// The script code generated is according to the format created by Henryshow.
// Copyright (c) 2018 Allen Zhou
// --------------------

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <regex>
#include <fstream>

using namespace std;

const int PTR_SIZE = 4;
const int WORD_SIZE = 4;

const int MAXLEN = 64;
const int MAX_LINE_LEN = 2048;
const int MAX_FILE_LEN = 1 << 20;

const int MAXVAR = 512;

const string SYSCALL_TABLE = "SYSCALL_TABLE.txt";
const string CONFIG_FILE = "CONFIG.txt";





//====== Lexer ======

//input_src keeps the origin file content
char input_src[MAX_FILE_LEN];

struct Lexer {

    //input points at the position to read
    char *input;

    int curln;
    char curch;

    //token read
    //watch out for buffer. It has to be dealt properly.
    char* buffer;
    int buflength;
    int token_type;

} lexer;

const int TOKEN_OTHER = 0;
const int TOKEN_IDENT = 1;
const int TOKEN_INT = 2;
const int TOKEN_CHAR = 3;
const int TOKEN_STR = 4;

char next_char () {
    if (lexer.curch == '\n')
        lexer.curln++;

    lexer.curch = *lexer.input;
    lexer.input++;
    return lexer.curch;
}

bool prev_char (char before) {
    lexer.input--;
    lexer.curch = before;
    return false;
}

void eat_char () {
    //The compiler is typeless, so as a compromise indexing is done
    //in word size jumps, and pointer arithmetic in byte jumps.
    (lexer.buffer + lexer.buflength++)[0] = lexer.curch;
    next_char();
}

void next () {
    //Skip whitespace
    while (lexer.curch == ' ' || lexer.curch == '\r' || lexer.curch == '\n' || lexer.curch == '\t')
        next_char();

    //Treat preprocessor lines as line comments
    //NOTE: /**/ is not supported
    if (   lexer.curch == '#'
        || (lexer.curch == '/' && (next_char() == '/' || prev_char('/')))) {
        while (lexer.curch != '\n' && lexer.input != '\0')
            next_char();

        //Restart the function (to skip subsequent whitespace, comments and pp)
        next();
        return;
    }

    lexer.buflength = 0;
    lexer.token_type = TOKEN_OTHER;

    //Identifier, keyword or integer literal
    if (isalpha(lexer.curch) || isdigit(lexer.curch) || lexer.curch == '_') {
        lexer.token_type = isalpha(lexer.curch) ? TOKEN_IDENT : TOKEN_INT;
        if(lexer.curch == '_')
            lexer.token_type = TOKEN_IDENT;

        while (lexer.token_type == TOKEN_IDENT ? (isalnum(lexer.curch) || lexer.curch == '_') && lexer.input != '\0'
                                    : isalnum(lexer.curch) && lexer.input != '\0')
            eat_char();

    //Character and String
    //The original mini-c does not support string.
    //However it is impossible since we use a lot of prompt() in System.so
    //So I try to add support for that.
    } else if (lexer.curch == '\'' || lexer.curch == '"') {
        lexer.token_type = lexer.curch == '"' ? TOKEN_STR : TOKEN_CHAR;

        char delimiter = lexer.curch;
        eat_char();

        while (lexer.curch != delimiter && lexer.input != '\0') {
            if (lexer.curch == '\\')
                eat_char();

            eat_char();
        }

        eat_char();

    //Operator -, --, -=, ->
    } else if (lexer.curch == '-' ) {
        eat_char();

        if(lexer.curch == '>' || lexer.curch == '-' || lexer.curch == '=')
            eat_char();

    //Operator (, (*), ((*)), (->), ((->))
    } else if (lexer.curch == '(') {
        int token_len = 1;

        if (strncmp(lexer.input, "(*)", 3) == 0)
            token_len = 3;
        else if (strncmp(lexer.input, "((*))", 5) == 0)
            token_len = 5;
        else if (strncmp(lexer.input, "(->)", 4) == 0)
            token_len = 4;
        else if (strncmp(lexer.input, "((->))", 6) == 0)
            token_len = 6;

        for(int i = 0; i < token_len; i++)
            eat_char();

    //Two char operators
    } else if (   lexer.curch == '+' || lexer.curch == '|' || lexer.curch == '&'
               || lexer.curch == '=' || lexer.curch == '!' || lexer.curch == '>' || lexer.curch == '<') {
        eat_char();

        // ++, --, ||, &&, ==, >>, << (No !!)
        // +=, -=, |=, &=, ==, !=, >=, <=
        if ((lexer.curch == lexer.buffer[0] && lexer.curch != '!') || lexer.curch == '=')
            eat_char();

    } else
        eat_char();

    *(lexer.buffer + lexer.buflength++) = '\0';
}

set<string> included_file_list;

bool include_file (const char* filename) {
    //prevent redundant include
    string strFilename (filename);
    if(included_file_list.find(strFilename) != included_file_list.end())
        return false;
    included_file_list.insert(strFilename);

    //input file
    char* remaining_input_backup = new char[MAX_FILE_LEN];
    strcpy(remaining_input_backup, lexer.input);

    *(lexer.input++) = '\n'; //add a new line - avoid file included being skipped by lexer

    // backup the include. The next scan for #include preprocess starts right after the include macro
    // (so that #include in the included files is supported.)
    char* end_of_include_macro = lexer.input;

    FILE* file_included = fopen(filename, "r");
    while((*lexer.input = (char) fgetc(file_included)) != EOF)
        lexer.input++;
    fclose(file_included);

    strcpy(lexer.input, remaining_input_backup);

    // recover the input to the end of #include macro.
    lexer.input = end_of_include_macro;

    delete[] remaining_input_backup;
    return true;
}

void preprocess_include () {
    while(*lexer.input != '"')
        lexer.input++;
    lexer.input++; //skip first '"'

    //copy filename
    char *filename = new char[MAX_LINE_LEN];
    char *q = filename;
    while(*lexer.input != '"') {
        *q = *lexer.input;
        lexer.input++;
        q++;
    }
    *q = '\0';
    lexer.input++; //skip last '"'

    include_file(filename);

    delete[] filename;
}

void lex_init (const char* filename) {
    //Read all file contents into input_src
    FILE *fin = fopen(filename, "r");
    lexer.input = input_src;
    while((*lexer.input = (char) fgetc(fin)) != EOF)
        lexer.input++;

    //NOTE: Default include: sg2lang.h
    lexer.input = input_src;
    include_file("sg2lang.h");

    //include files
    for(lexer.input = input_src; *lexer.input != '\0'; lexer.input++)
        if(*lexer.input == '#')
            preprocess_include();

    //Get the lexer into a usable state for the parser
    lexer.input = input_src;
    lexer.curln = 1;
    lexer.buffer = new char[MAXLEN];
    next_char();
    next();
}




//==== Parser helper functions ====

int error_count;

//prompt error
void error (const char* format) {
    printf("%d: error: ", lexer.curln);
    printf(format, lexer.buffer);
    error_count++;
    printf("\n");

    // Prevent infinite loop
    if (error_count >= 20)
        exit(1);
}

//if requirement is not satisfied, prompt error.
void require (bool condition, const char* format) {
    if (!condition)
        error(format);
}

//see if lexer.buffer == required token
bool see (const char* look) {
    return !strcmp(lexer.buffer, look);
}

//if lexer.buffer != required token, then we will be waiting for it.
bool waiting_for (const char* look) {
    return !see(look) && lexer.input != '\0';
}

void match (const char* look) {
    if (!see(look)) {
        printf("%d: error: expected '%s', found '%s'\n", lexer.curln, look, lexer.buffer);
        error_count++;

        // Prevent infinite loop
        if (error_count >= 20)
            exit(1);
    }

    next();
}

bool try_match (const char* look) {
    bool saw = see(look);

    if (saw)
        next();

    return saw;
}





//======Data Structure: Variables, Strings, Functions, Labels and SYSCALLs======
//NOTE some instructions keep a function form, like INST_45(wait) and SETINV.
//They are treated like functions with special judgement.

//Data for a funtion
struct Function {
    string name;
    int magic_id;
    int string_id;
    int callsign;
    int argument_count;
    bool has_return_value;
};

struct Syscall {
    string name;
    int argument_count;
    int return_type;
    int id;
};

//Read from config file
map<string, Syscall> syscall_table;

map<string, Function> function_table;
map<string, int> string_table;
map<string, int> label_table;

int string_table_len = 0;
int magic_table_len = 0;
int label_len = 0;

map<string, int> locals;
int local_no = 1;
int param_no = -2;

void init_syscall (const char* config_filename) {
    FILE* fin = fopen(config_filename, "r");
    if (fin == NULL)
        return;

    Syscall syscall;
    char c_name[MAXLEN];

    while (fscanf(fin, "%x %d %d %s", &syscall.id, &syscall.argument_count,
                        &syscall.return_type, c_name) != EOF){
        string strName(c_name);
        syscall.name = strName;
        syscall_table[strName] = syscall;
    }

    fclose(fin);
}

void init_config (const char* config_filename) {
    FILE* fin = fopen(config_filename, "r");

    if (fin == NULL)
        return;

    char config_line[MAXLEN];

    while (!feof(fin)) {

        fgets(config_line, MAXLEN, fin);
        regex pattern("([a-zA-Z0-9_]+)\\s*=\\s*(\\d+).*?");
        cmatch char_matches;

        if (regex_search(config_line, char_matches, pattern)) {
            if (char_matches.str(1) == "string_table_start")
                string_table_len = atoi(char_matches.str(2).c_str());

            else if (char_matches.str(1) == "magic_table_start")
                magic_table_len = atoi(char_matches.str(2).c_str());

        }
    }
    fclose(fin);
}

void init_symbols () {
    init_syscall(SYSCALL_TABLE.c_str());
    init_config(CONFIG_FILE.c_str());
}

void new_fn (const char* ident, bool has_return_value, int callsign = 0) {
    string name (ident);

    Function function;
    function.name = name;
    function.magic_id = magic_table_len ++;
    function.callsign = callsign;
    function.has_return_value = has_return_value;

    //Warning: the function name string may has been declared!
    if (string_table.find(name) != string_table.end()) { //string exists
        function.string_id = string_table[name];
    } else {
        function.string_id = string_table_len ++;
        string_table[name] = function.string_id;
    }
    function_table[name] = function;
}

int new_local (const char* ident) {
    string name (ident);

    require(locals.find(name) == locals.end(),
        "Local variable '%s' redefined");

    //Local variable starts from stack position 1
    locals[name] = local_no;
    return local_no++;
}

int new_param (const char* ident) {
    string name (ident);

    require(locals.find(name) == locals.end(),
        "Param '%s' redefined");

    //Params temporary starts from -2 to -inf.
    //It should starts from (-param_count - 1) to -2, but temporary it is reversed.
    locals[name] = param_no;
    return param_no--;
}

int new_label (const char* ident) {
    string name(ident);
    label_table[ident] = label_len;
    return label_len ++;
}

char* new_generated_label (const char* name) {
    char* ident = new char[MAXLEN];
    sprintf(ident, "t%x_%d_%s", time(0), label_len, name);
    new_label(ident);
    return ident;
}

//Enter the scope of a new function
void new_scope () {
    local_no = 1;
    param_no = -2;
}

//Find the local by stack number.
//Returns "" when not found.
string stack_lookup (int stack) {
    for(auto it = locals.begin(); it != locals.end(); ++it)
        if(it->second == stack)
            return it->first;
    return "";
}

string label_lookup (int label_no) {
    for(auto it = label_table.begin(); it != label_table.end(); ++it)
        if(it->second == label_no)
            return it->first;
    return "";
}


//====== Code generator ======


// The target code is different; it is like generating an
//     inversed poland expression.
// There is no function declaration; it is printed in the bottom as
//     so-called "magic_table".
// The only lvalue avaliable are locals;
//     an expression won't generate an lvalue.
//     (SetData_NotAligned still requires a call.)
// If the complier find a identifier that is not declared,
//     it will try the following steps:
//     - Function CALL: will CALL the function
//     - Async Function CALLBS: TODO: Not supported yet.
//          (I will try to make use of string_table or support __asm{}.)
//     - Otherwise an unknown constant. The complier
//          will assume that it has already been defined in
//          predefine.inc or somewhere else.
//          This will become PUSH XXXXXX in the output.
// Most other features remain unchanged.

char output_base[MAX_FILE_LEN];
char* output = output_base;


void expr (int);

bool try_match_instruction () {

    if (see("GetGlobalVariable") ||
        see("GetINV")) {

        string function_name (lexer.buffer);
        next();

        match("(");

        require(lexer.token_type == TOKEN_INT || lexer.token_type == TOKEN_IDENT,
            "'%s' is not an explicit integer index");

        output += sprintf(output,
              function_name == "GetGlobalVariable" ? "\tINST_09 %s\n" : "\tPUSHINV %s\n",
              lexer.buffer);
        next();

        match(")");

    } else if (see("SetGlobalVariable") || see("SetINV")) {
        string function_name (lexer.buffer);
        next();

        match("(");
        expr(1);
        match(",");

        require(lexer.token_type == TOKEN_INT || lexer.token_type == TOKEN_IDENT,
            "'%s' is not an explicit integer index");

        output += sprintf(output,
            function_name == "SetGlobalVariable" ? "\tSETARG %s\n" : "\tSETINV %s\n",
            lexer.buffer);
        next();

        match(")");

    } else if (try_match("Delay")) {
        match("(");
        expr(1);
        match(")");

        output += sprintf(output, "\tDELAY\n");

    } else if (try_match("Wait")) {
        match("(");

        string name (lexer.buffer);
        require(function_table.find(name) != function_table.end() &&
                string_table.find(name) != string_table.end(),
            "'%s' is not a function name");
        next();

        match(")");

        output += sprintf(output, "\tPUSHSTR %d\n"
                                  "\tINST_45\n", string_table[name]);
    } else
        return false;

    return true;
}
// No more backward argument list.
// args are now in order -(param_count + 1) to -2 from the first to last.
/*
int backward_argument_list () {
    // Normal CALL requires arguments to be pushed backwards
    // Use a proxy output for each argument, and push them into real
    // output in reverse order.
    stack<char*> arguments;
    char* real_output = output;

    do {
        char* argument = new char[MAX_LINE_LEN];
        arguments.push(argument);
        output = argument;

        expr(1);

    } while (try_match(","));

    int argument_count = arguments.size();

    output = real_output;
    while(!arguments.empty()) {
        char* argument = arguments.top();
        arguments.pop();
        output += sprintf(output, "%s", argument);
        delete[] argument;
    }

    return argument_count;
}
*/
bool try_match_syscall () {
    if (lexer.token_type != TOKEN_IDENT)
        return false;

    string name (lexer.buffer);

    if(syscall_table.find(name) == syscall_table.end())
        return false;

    next();
    match("(");

    if(waiting_for(")")) do {
        expr(1);
    } while (try_match(","));

    match(")");

    Syscall syscall = syscall_table[name];

    output += sprintf(output, "\tSYSCALL 0x%X, (%d | (%d << 16)) ; %s\n",
        syscall.id, syscall.argument_count, syscall.return_type, name.c_str());
    return true;
}

bool try_match_function () {
    if (lexer.token_type != TOKEN_IDENT)
        return false;

    string name (lexer.buffer);

    // backup the lexer.input.
    // A normal function call may point to a function not in the table
    //   (e.g. call for DownBrightness() is very common in Magic.asm)
    // It is necessary to see if there is a left brace.
    Lexer lexer_backup = lexer;

    next();
    bool condition = function_table.find(name) != function_table.end() || see("(");

    lexer = lexer_backup;

    strcpy(lexer.buffer, name.c_str());
    lexer.buflength = name.size();
    lexer.token_type = TOKEN_IDENT;

    if(condition) {
        next();

        match("(");

        // no more backward argument pushing.
        bool has_argument = waiting_for(")");
        if(has_argument) do {
            expr(1);
        } while (try_match(","));

        match(")");

        // A function with return value and no argument
        // requires a dummmy stack slot to accept the return value.
        if(!has_argument && function_table[name].has_return_value)
            output += sprintf(output, "\tPUSH 0\n");

        output += sprintf(output, "\tCALL %s\n", name.c_str());
    }

    return condition;
}

// Locals, integers, strings and normal function calls.
// This function is a mixture of function factor() and object() in mini-c.
void object () {
    string name (lexer.buffer);

    if(see("true") || see("false")) {
        output += sprintf(output, "\tPUSH %s\n", see("true") ? "1" : "0");
        next();

    } else if (lexer.token_type == TOKEN_IDENT) {

        // Local variable
        if (locals.find(name) != locals.end()) {
            int local = locals[name];

            // Array
            if(try_match("[")) {
                expr(1);
                match("]");
                output += sprintf(output, "\tPUSHNR %d\n", local);

            } else
                output += sprintf(output, "\tPUSHARG %d\n", local);

            next();


        // Instructions and normal function call
        // The order is necessary, since instructions are NOT in function_table.
        // # I hate name polllution.
        } else if (try_match_instruction () ||
                   try_match_syscall () ||
                   try_match_function () ) {
            return;

        // Unknown constant (assume defined in predefine.inc or somewhere else)
        } else {
            output += sprintf(output, "\tPUSH %s\n", lexer.buffer);
            next();
        }

    // Integers
    } else if (lexer.token_type == TOKEN_INT) {
        output += sprintf(output, "\tPUSH %s\n", lexer.buffer);
        next();

    // "" and '' are treated equally.
    // Strings
    } else if (lexer.token_type == TOKEN_CHAR || lexer.token_type == TOKEN_STR) {
        // Get rid of delimiter
        string content = name.substr(1, name.size() - 2);

        if(string_table.find(content) == string_table.end())
            string_table[content] = string_table_len ++;

        output += sprintf(output, "\tPUSHSTR %d\n", string_table[content]);

        next();

    //An object can be braced
    } else if (try_match("(")) {
        expr(1);
        match(")");

    } else {
        error("expected an expression, found '%s'\n");
    }
}

void unary () {
    if (try_match("!")) {
        //Recurse to allow chains of unary operations, LIFO order
        unary();

        output += sprintf(output, "\tZERO\n");

    } else if (try_match("-")) {
        unary();
        output += sprintf(output, "\tNEG\n");

    } else if (try_match("~")) {
        unary();
        output += sprintf(output, "\tNOT\n");

    } else {
        //This function call compiles itself
        object();

        //TODO ++ and -- are not supported for now.
    }
}

// Expression priority level:
// 6: unary operators, () and objects
// 5: binary operators <<, >>, &, |, ^ (xor)
// 4: mul, div and mod
// 3: add and sub
// 2: comparators ==, !=, <, <=, >, >=
// 1: logic operators && ||

void expr (int level) {

    if (level == 6) {
        unary();
        return;
    }

    expr(level + 1);

    while (  level == 5 ? see("<<") || see(">>") || see("&") || see("|") || see("^")
           : level == 4 ? see("*") || see("/") || see ("%")
           : level == 3 ? see("+") || see("-")
           : level == 2 ? see("==") || see("!=") || see("<") || see(">=") || see(">") || see("<=")
           : false) {

        const char* instr =
                      see("<<") ? "SHL" : see(">>") ? "SHR" : see("&") ? "AND" :
                      see("|") ? "OR" : see("^") ? "XOR" :

                      see("+") ? "ADD" : see("-") ? "SUB" : see("*") ? "MUL" :
                      see("/") ? "DIV" : see("%") ? "MOD" :

                      see("==") ? "" : see("!=") ? "Z" : see("<") ? "L" :
                      see("<=") ? "LE" : see(">") ? "G" : "GE";

        next();
        expr(level + 1);

        if (level <= 5 && level >= 3)
            output += sprintf(output, "\t%s\n", instr);

        else
            output += sprintf(output, "\tCMP%s\n", instr);
    }

    //NOTE logic shortcircuit is not supported.
    // e.g. if(expression_A && expression_B)
    // if expression_A is false, expression_B will still be calculated.
    if (level == 1) while (see("||") || see("&&")) {

        bool is_and = see("&&");

        next();
        expr(level + 1);

        output += sprintf(output, is_and ? "\tORNZ\n" : "\tORZ\n");
    }

}




// Necessary information for return, break and continue.
int curfn_argument_count;
stack<char*> loop_to_labels;
stack<char*> break_to_labels;

// NOTE value assigment operation does not have a value.
// This function deals mainly with assignments. User's label declaration is dealt as well.
void normal_line () {
    string name (lexer.buffer);

    // backup the input and output.
    Lexer lexer_backup = lexer;
    char* output_backup = output;

    bool local_exists = locals.find(name) != locals.end();

    next();

    // See if lvalue is an array slot.
    // SETNR requires the value pushed in first.
    // backup the index.
    bool is_array = false;
    char index[MAX_LINE_LEN];
    if(try_match("[")) {

        output = index;
        expr(1);
        match("]");

        // Restore to the output_backup.
        output = output_backup;
        is_array = true;
    }

    bool has_assignment =
         see("=") || see("+=") || see("-=") || see("*=") || see("/=") ||
         see("%=") || see("&=") || see("|=") || see("<<=") || see(">>=") ||
         see("^=");


    //if it is not an assignment...
    if(!local_exists || !has_assignment) {

        // it may be a label declaration!
        bool is_label = see(":");
        if(is_label) {
            output += sprintf(output, "%s:\n", name.c_str());
            match(":");

        // not a label, then it will be an expression.
        // restore the lexer.input and call expr() to handle this.
        } else {
            lexer = lexer_backup;
            strcpy(lexer.buffer, name.c_str());

            expr(1);
        }
        return;
    }

    bool direct_assignment = see("=");

    const char* instr =
        see("=") ? "" : see("+=") ? "ADD" : see("-=") ? "SUB" : see("*=") ? "MUL"
      : see("/=") ? "DIV" : see("%=") ? "MOD" : see("&=") ? "AND"
      : see("|=") ? "OR" : see("<<=") ? "SHL" : see(">>=") ? "SHR" : "XOR";

    next();
    expr(1);

    if(!direct_assignment)
        output += sprintf(output, "\tPUSHARG %d\n"
                                  "\t%s\n", locals[name], instr);

    if(is_array) {
        //Push index and set value.
        output += sprintf(output, "%s", index);
        output += sprintf(output, "\tSETNR %d\n", locals[name]);
    } else
        output += sprintf(output, "\tPOPN %d\n", locals[name]);
}

void line ();

void if_branch () {

    char* if_start = new_generated_label("IF");
    char* false_branch = new_generated_label("ELSE");
    char* join = new_generated_label("IF_END");

    output += sprintf(output, "\tJZ %s\n", false_branch);

    output += sprintf(output, "%s:\n", if_start);
    line();
    output += sprintf(output, "\tJMP %s\n", join);

    output += sprintf(output, "%s:\n", false_branch);
    if (try_match("else"))
        line();

    output += sprintf(output, "%s:\n", join);

    free(false_branch);
    free(join);
}

void match_if () {
    match("if");
    match("(");
    expr(1);
    match(")");
    if_branch();
}

void while_loop () {
    char* loop_to = new_generated_label("WHILE");
    char* break_to = new_generated_label("WHILE_END");
    loop_to_labels.push(loop_to);
    break_to_labels.push(break_to);

    output += sprintf(output, "%s:\n", loop_to);

    bool do_while = try_match("do");

    if (do_while)
        line();

    match("while");
    match("(");
    expr(1);
    match(")");

    output += sprintf(output, "\tJZ %s\n", break_to);

    if (do_while)
        match(";");

    else
        line();

    output += sprintf(output, "\tJMP %s\n", loop_to);
    output += sprintf(output, "%s:\n", break_to);

    loop_to_labels.pop();
    break_to_labels.pop();
    free(loop_to);
    free(break_to);
}

void for_loop () {
    match("for");
    match("(");

    //init
    line(); //will match(";")

    char* loop_start = new_generated_label("FOR");
    char* loop_to = new_generated_label("FOR_CONTINUE");
    char* break_to = new_generated_label("FOR_END");
    loop_to_labels.push(loop_to);
    break_to_labels.push(break_to);

    //emit label loop_start
    output += sprintf(output, "%s:\n", loop_start);

    //condition
    expr(1);
    output += sprintf(output, "\tJZ %s\n", break_to);
    match(";");

    //backup the output. Increment will be appended to the end of the loop
    char loop_increment[MAX_LINE_LEN];
    char* output_backup = output;
    output = loop_increment;
    normal_line();
    output = output_backup;

    match(")");

    //body
    line();

    //emit label loop_to
    output += sprintf(output, "%s:\n", loop_to);

    //Append increment
    output += sprintf(output, "%s", loop_increment);

    //loop
    output += sprintf(output, "\tJMP %s\n", loop_start);

    //emit label break_to
    output += sprintf(output, "%s:\n", break_to);

    loop_to_labels.pop();
    break_to_labels.pop();
    free(loop_to);
    free(break_to);
    free(loop_start);
}

void async_call () {

    string name (lexer.buffer);

    require(function_table.find(name) != function_table.end() &&
            string_table.find(name) != string_table.end(),
        "requires a function name, found '%s'");
    output += sprintf(output, "\tPUSHSTR %d\n", string_table[name]);
    next();

    match("(");

    // no more backward argument pushing
    int argument_count = 0;
    if (waiting_for(")")) do {
        expr(1);
        argument_count++;
    } while (try_match(","));

    match(")");

    require(argument_count <= 10, "too much argument for an async call");

    //CALLBS
    if (argument_count <= 4) {
        while (argument_count < 4) {
            output += sprintf(output, "\tPUSH 0\n");
            argument_count ++;
        }
        output += sprintf(output, "\tCALLBS\n");

    //SYSCALL 0x310 - 0x314
    } else {
        output += sprintf(output,
            "\tSYSCALL 0x31%d, (%d | (0 << 16)) ; CALLBS \n",
            argument_count - 5,
            argument_count + 1);
    }

    match(";");
}

void decl_local () {
    int local = new_local(lexer.buffer);
    next();

    if(try_match("[")) {
        require(lexer.token_type == TOKEN_INT,
                "'%s' is not an explicit integer. note it cannot be a macro definition");
        int value;
        sscanf(lexer.buffer, "%d", &value);
        local_no += value - 1;
        next();

        match("]");

    } else {
        if(try_match("=")) {
            expr(1);
            output += sprintf(output, "\tPOPN %d\n", local);
        }
    }

    match(";");
}

void match_asm () {
    match("__asm");
    match("{");
    output += sprintf(output, "\t");

    //Will not check grammar
    while(waiting_for("}")) {

        string token (lexer.buffer);

        // Strings: will be converted into string no. in string table
        //NOTE if there is a async_call, the user should write PUSHSTR "function_name",
        //     instead of PUSHSTR function_name.
        //     Function names will not be converted,
        //     because direct CALL uses them instead of numbers.
        if(lexer.token_type == TOKEN_STR) {
            // Get rid of delimiter
            string content = token.substr(1, token.size() - 2);

            if(string_table.find(content) == string_table.end())
                string_table[content] = string_table_len ++;

            output += sprintf(output, "%d", string_table[content]);

        // Locals: change to local no.
        } else if(locals.find(token) != locals.end()) {
            output += sprintf(output, "%d ", locals[token]);

        // Label decl (colon): delete extra space and add the colon
        } else if(token == ":") {
            output--;
            output += sprintf(output, ":");

        // Negative symbol: no extra space
        } else if(token == "-") {
            output += sprintf(output, "-");

        // otherwise: not changed
        } else {
            output += sprintf(output, "%s ", lexer.buffer);
        }

        //output new line if there is new line in __asm block
        int curln_backup = lexer.curln;
        next();

        if(lexer.curln != curln_backup)
            output += sprintf(output, "\n\t");
    }

    match("}");
}

void output_return (bool has_return_value) {
    if(has_return_value) {
        expr(1);
        output += sprintf(output, "\tPOPN %d\n",
            curfn_argument_count == 0 ? -2 : -1 - curfn_argument_count);
    }

    output += sprintf(output, "\tRETN %d\n",
            has_return_value ? curfn_argument_count - 1 : curfn_argument_count);
}

void line () {
    if (see("if"))
        match_if();

    else if (see("while") || see("do"))
        while_loop();

    else if (see("for"))
        for_loop();

    else if (try_match("var"))
        decl_local();

    else if (try_match("asynccall"))
        async_call();

    else if (try_match("break")) {
        output += sprintf(output, "\tJMP %s\n", break_to_labels.top());
        match(";");

    } else if (try_match("continue")) {
        output += sprintf(output, "\tJMP %s\n", loop_to_labels.top());
        match(";");

    } else if (try_match("goto")) {
        output += sprintf(output, "\tJMP %s\n", lexer.buffer);
        next();
        match(";");

    } else if (see("__asm")) {
        match_asm();

    } else if (try_match("{")) {
        while (waiting_for("}"))
            line();

        match("}");

    } else {
        bool ret = try_match("return");

        if (ret) {
            bool has_return_value = waiting_for(";");
            output_return(has_return_value);

        } else
            normal_line();

        match(";");
    }
}

void decl_function () {
    // Ready up global variables used for constructing a function
    bool has_return_value = false;
    curfn_argument_count = 0;
    local_no = 1;
    param_no = -2;

    while(!loop_to_labels.empty())
        loop_to_labels.pop();

    while(!break_to_labels.empty())
        break_to_labels.pop();

    locals.clear();


    if (try_match("int"))
        has_return_value = true;
    else
        match("void");

    match("function");
    string name (lexer.buffer);
    output += sprintf(output, "%s:\n", lexer.buffer);
    next();

    match("(");

    while (waiting_for(")")) {
        new_param(lexer.buffer);
        curfn_argument_count ++;
        next();

        if(!try_match(","))
            break;
    }
    match(")");

    // reverse the param numbers!
    for(auto it = locals.begin(); it != locals.end(); ++it)
        it->second = -(curfn_argument_count + it->second + 3);

    int callsign = 0;

    if (try_match("callsign")) {
        require(lexer.token_type == TOKEN_INT,
            "expect an explicit integer callsign, found '%s'");
        sscanf(lexer.buffer, "%d", &callsign);
        next();
    }

    new_fn(name.c_str(), has_return_value, callsign);
    function_table[name].argument_count = curfn_argument_count;

    //Leave some space for STACK declaration
    output += sprintf(output, "\tSTACK ");
    char* stack_inst_pos = output;
    output += sprintf(output, "    \n");

    match("{");

    while(waiting_for("}"))
        line();

    match("}");

    //Add STACK declaration to the front of the func
    char local_str[MAXLEN];
    sprintf(local_str, "%d", local_no - 1);
    strncpy(stack_inst_pos, local_str, strlen(local_str));

    //An extra RETN is required in the end of funtion, if there is no return value.
    if(!has_return_value)
        output_return(false);

    output += sprintf(output, "\n");
}



void output_header (FILE* fout) {
    fputs("include \"predefine.inc\"\n", fout);
    fputs("org 40h\n\n", fout);
}

void output_string_definitions (FILE* fout) {
    vector<pair<int, string> > strings;
    for(auto it = string_table.begin(); it != string_table.end(); ++it)
        strings.push_back(make_pair(it->second, it->first));

    sort(strings.begin(), strings.end(), [](pair<int, string> x, pair<int, string> y) {
        return x.first < y.first;
    });

    for(auto it = strings.begin(); it != strings.end(); ++it)
        fprintf(fout, "STRING_%d: DB \"%s\"\n", it->first, it->second.c_str());
}

void output_magic_table (FILE* fout) {
    fputs("\nmagic_table_addr:\n", fout);

    vector<Function> fns;
    for(auto it = function_table.begin(); it != function_table.end(); ++it)
        fns.push_back(it->second);

    sort(fns.begin(), fns.end(), [](Function x, Function y){
        return x.magic_id < y.magic_id;
    });

    for(auto it = fns.begin(); it != fns.end(); ++it)
        fprintf(fout, "MAGIC_%d : procedure %s %d STRING_%d %d\n",
            it->magic_id, it->name.c_str(), it->argument_count,
            it->string_id, it->callsign);

    fputs("\n"
          "magic_table_end:\n", fout);
}

void output_string_table (FILE* fout) {
    fputs("\nstring_table_addr:\n", fout);

    for(int i = 0; i < string_table_len; i++)
        fprintf(fout, "\tDB STRING_%d\n", i);

    fputs("\nstring_table_end:\n", fout);
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: ss <file>");
        return 1;
    }

    FILE* fout = fopen("ss.asm", "w");

    lex_init(argv[1]);
    init_symbols();
    error_count = 0;

    while(lexer.curch != '\0')
        decl_function();

    output_header(fout);

    fputs(output_base, fout);

    fputs("\n\n", fout);

    // No longer required. Switch to sg2io.cpp
    //fputs("include \"sg2lib.asm\"\n", fout);

    output_string_definitions(fout);
    output_magic_table(fout);
    output_string_table(fout);

    fputs("file_end:\n"
          "DB \"Creator: Henryshow\"\n"
          "DB \"Pseudo Compiler Author: Allen Zhou\"", fout);

    fclose(fout);

    return error_count != 0;
}
