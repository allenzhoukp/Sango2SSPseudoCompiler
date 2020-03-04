#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "ParserObjects.h"
#include <set>
#include <stack>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <fstream>

using std::vector;
using std::set;
using std::stack;
using std::unordered_map;

class Parser {
private:

    const static int MAX_LINE_LEN = 1024;
    const static int MAX_FUNC_LEN = 1024;

    //Config properties
    map<string, string> configs;

    //Token list
    Token* tokens;
    int tokenCount;

    //Information that won't change.
    map<string, int> typeNameToCode;
    map<string, int> numericNameToCode;

    Syscall syscalls[MAX_FUNC_LEN];
    int syscallCount = 0;
    map<string, Syscall*> syscallNameMapping;

    StructInfo structInfos[MAX_FUNC_LEN];
    int structInfoCount = 0;

    //intvs can be ANYWHERE, so it's no good to use array mapping.
    Intv intvs[MAX_FUNC_LEN];
    int intvCount = 0;

    //Changable: labels, functions, vars, strings, ...
    set<string> labels;
    int labelNo = 0;

    unordered_map<string, int> stringTable;
    int stringCount = 0;

    Function funcs[MAX_FUNC_LEN];
    int funcCount = 0;
    unordered_map<string, Function*> funcNameMapping;
    Function* currentFunc;

    //for multiple loop/switch: top stack indicates the label to jump to
    stack<string> breakToLabels;
    stack<string> loopToLabels;

    //for end-of-funtion return flag
    bool lineReturned = false;

    //backward ++/-- (like i++)
    stack<ExpressionNode*> ppNodes;
    stack<ExpressionNode*> mmNodes;

    //outputer
    // std::ofstream out;
    std::ostringstream out;

    //FixIdentifierLoader.cpp: load config files
    void loadConfig ();
    void loadSyscallTable ();
    void loadIntvTable ();
    void loadStructTable ();

    //Parser.cpp: Auxillary functions
    bool isPtr (int type);
    bool isInteger(int type) ;
    bool isStruct(int type) ;
    bool isStructPtr(int type);
    int getStructIdByType(int type);

    void require (int tokenPos, bool condition, string errmsg);
    bool see (int tokenPos, string s);
    void match (int& tokenPos, string s);
    bool tryMatch (int& tokenPos, string s);
    bool validateType (int target, int given);
    int getType (int& tokenPos, bool match, string& typeName);
    int getType (int& tokenPos, bool match);
    StructInfo* getStructInfoByid (int id) ;
    StructInfo* getStructInfoByName (string name) ;
    string toCamel (string name);

    void newLocal (int type, string name);
    void newLocalArray (int type, string name, int len);
    void initParam (vector<Var> params);
    int getLocalNoByName (string name);
    Var getLocalByName (string name);
    int getIntvNoByName (string name);
    Intv getIntvByName (string name);

    void newLabel (string name);
    string emitLabel (string name);
    bool findLabel (string name);

    int getStringNo (string str);
    string getString (int no);
    void newString (string str);

    //Expression node pool
    class ExpressionNodePool {
    private:
        ExpressionNode pool[MAX_LINE_LEN * 10];
        int len = 0;
    public:
        ExpressionNodePool();
        ExpressionNode* newNode();
        void clear();

    } expNodePool;

    //TreeConstruction.cpp: Construct expression tree in a single line, or expr(level = 0).
    void initExpNodeParamArray (ExpressionNode* x, int len);
    bool tryMatchSyscall(ExpressionNode* &x, int& tokenPos);
    bool tryMatchInst(ExpressionNode* &x, int& tokenPos);
    bool tryMatchFunccall(ExpressionNode* &x, int& tokenPos);
    StructMember matchMember (int& tokenPos, int structType);
    ExpressionNode* createMemberNode (StructMember member);
    int dotAndArrayInStruct (ExpressionNode* &nodePos, int& tokenPos, int structOrArrayType);
    void back(ExpressionNode* &nodePos, int& tokenPos);
    void object(ExpressionNode* &x, int& tokenPos);
    void unary(ExpressionNode* &x, int& tokenPos);
    void expr(ExpressionNode* &x, int& tokenPos, int priority);

    //TreeAnalysis.cpp: Destruct expression tree and generate code in a single line, or expr().
    void outputInst(ExpressionNode* x, int& stackDepth);
    void outputFuncCall(ExpressionNode* x, int& stackDepth);
    void outputGetValue(int type);
    void outputSetValue(int type);
    void outputUnaryOp(ExpressionNode* x, int& stackDepth, bool remainReturnStack);
    void outputBinaryOp(ExpressionNode* x, int& stackDepth, bool remainReturnStack);
    void treeDFS(ExpressionNode* x, int& stackDepth, bool remainReturnStack = true);
    void matchExpr(int& tokenPos, bool hasResultStack, int returnStackType = DataTypes::typeInt);

    //BranchAnalysis.cpp: Dealt with branches, like functions, if, loops, asm blocks, etc.
    void outputLabel(string name);
    bool tryMatchAsm(int& tokenPos);
    bool tryMatchIf(int& tokenPos);
    bool tryMatchWhile(int& tokenPos);
    bool tryMatchFor(int& tokenPos);
    bool tryMatchSwitch(int& tokenPos);
    bool tryMatchLabelDecl (int& tokenPos) ;
    bool tryMatchJumps (int& tokenPos);
    bool tryMatchLocalDecl (int& tokenPos) ;
    bool tryMatchAsynccall (int& tokenPos);
    void line(int& tokenPos);

    void matchFunc(int& tokenPos);

    //Parser.cpp: Output things out of a function.
    void outputStringDefs ();
    void outputMagicTable ();
    void outputStringTable ();

    //Parser.cpp: reduce labels.
    string reduceLabels(string output);

public:
    Parser();
    void init(Token* tokenList, int tokenCnt_);
    string str();
};

#endif
