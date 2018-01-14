#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "ParserObjects.h"
#include <vector>

using std::vector;

class Parser {
private:

    const static int MAX_LINE_LEN = 1024;
    const static int MAX_FUNC_LEN = 1024;

    Token* tokens;
    int tokenCount;

    map<string, int> typeNameToCode;
    map<string, int> numericNameToCode;

    Syscall syscalls[MAX_FUNC_LEN];
    int syscallCount = 0;
    map<string, Syscall*> syscallNameMapping;

    //intvs can be ANYWHERE, so it's no good to use array mapping.
    Intv intvs[MAX_FUNC_LEN];
    int intvCount = 0;

    //mapping: code - var.
    int globalTypes[MAX_FUNC_LEN];

    Function funcs[MAX_FUNC_LEN];
    int funcCount = 0;
    map<string, Function*> funcNameMapping;
    Function* currentFunc;

    //backward ++/-- (like i++)
    ExpressionNode* ppNode;
    ExpressionNode* mmNode;

    void loadSyscallTable ();
    void loadIntvTable ();

    bool isPtr (int type);
    void require (int tokenPos, bool condition, string errmsg);
    bool see (int tokenPos, string s);
    void match (int& tokenPos, string s);
    bool tryMatch (int& tokenPos, string s);
    bool validateType (int target, int given);
    int getType (int& tokenPos, bool match, string& typeName);
    int getType (int& tokenPos, bool match);

    void newLocal (int type, string name);
    void initParam (vector<Var> params);
    int getLocalNoByName (string name);
    Var getLocalByName (string name);
    int getIntvNoByName (string name);
    Intv getIntvByName (string name);


    class ExpressionNodePool {
    private:
        ExpressionNode pool[MAX_LINE_LEN * 10];
        int len = 0;
    public:
        ExpressionNodePool();
        ExpressionNode* newNode();
        void clear();

    } expNodePool;

public:
    Parser(Token* tokenList, int tokenCnt_);
    void initExpNodeParamArray (ExpressionNode* x, int len);
    bool tryMatchSyscall(ExpressionNode* &x, int& tokenPos);
    bool tryMatchInst(ExpressionNode* &x, int& tokenPos);
    bool tryMatchFunccall(ExpressionNode* &x, int& tokenPos);
    void back(ExpressionNode* &nodePos, int& tokenPos);
    void object(ExpressionNode* &x, int& tokenPos);
    void unary(ExpressionNode* &x, int& tokenPos);
    void expr(ExpressionNode* &x, int& tokenPos, int priority);
};

#endif
