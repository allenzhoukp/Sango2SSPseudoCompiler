#include "Parser.h"
#include "Panic.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <regex>

using std::endl;

//=================Expression Tree Analysis==================

void Parser::outputInst(ExpressionNode* x, int& stackDepth) {
    //GetGlobal(), SetGlobal(), GetIntv(), SetIntv(), Delay(), Wait(), IsRunning()
    //INST_4F,     INST_52,     PUSHINVR,  INST_53,   DELAY,   INST_45,INST_46
    if(x->name == "GetGlobal" || x->name == "GetIntv") {
        treeDFS(x->params[0], stackDepth);
        out << "\t" << (x->name == "GetGlobal" ? "INST_4F 0" : "PUSHINVR 0") << endl;

    } else if(x->name == "SetGlobal" || x->name == "SetIntv") {
        //value is pushed first.
        treeDFS(x->params[1], stackDepth);
        treeDFS(x->params[0], stackDepth);
        out << "\t" << (x->name == "GetGlobal" ? "INST_52 0" : "INST_53 0")  << endl;
        stackDepth -= 2;

    } else if(x->name == "Delay" || x->name == "Wait") {
        treeDFS(x->params[0], stackDepth);
        out << "\t" << (x->name == "Delay" ? "DELAY" : "INST_45") << endl;
        stackDepth--;

    } else if(x->name == "IsRunning") {
        treeDFS(x->params[0], stackDepth);
        out << "\t" << "INST_46" << endl;

    } else {
        printf("!FATAL! invalid inst name");
    }
}

void Parser::outputFuncCall(ExpressionNode* x, int& stackDepth) {
    //caution: w/return value func requires at least one param!
    //if has return value but no param, a dummy param is required.
    bool hasReturnValue = x->func->returnType != DataTypes::typeVoid;
    bool needDummy = hasReturnValue && x->func->paramCount == 0;
    if(needDummy) {
        out << "\t" << "PUSH 0 ; dummy" << endl;
        stackDepth++;
    }

    for(int i = 0; i < x->func->paramCount; i++)
        treeDFS(x->params[i], stackDepth);
    out << "\t" << "CALL " << x->func->name;

    stackDepth -= x->func->paramCount;

    //non-void function, add a stack depth for return value.
    //special: for has-dummy funccall, stack depth remains (origin + 1). No need for adding up.
    if(hasReturnValue && !needDummy)
        stackDepth++;
}

void Parser::outputGetValue(int type) {
    switch(type) {
    case DataTypes::typeUIntPtr:
        out << "\t" << "CALL GetInt_Unsigned" << endl;
        break;
    case DataTypes::typeShortPtr:
        out << "\t" << "CALL GetShort" << endl;
        break;
    case DataTypes::typeUShortPtr:
        out << "\t" << "CALL GetShort_Unsigned" << endl;
        break;
    case DataTypes::typeBytePtr:
        out << "\t" << "CALL GetByte" << endl;
        break;
    case DataTypes::typeUBytePtr:
        out << "\t" << "CALL GetByte_Unsigned" << endl;
        break;
    case DataTypes::typeStringPtr:
        out << "\t" << "SYSCALL 0x200, (1 | (3 << 16)) ; GetString" << endl;
        break;
    default:
        out << "\t" << "CALL GetInt" << endl;
    }
}

void Parser::outputSetValue(int type) {
    switch(type) {
    case DataTypes::typeUIntPtr:
        out << "\t" << "CALL SetInt_Unsigned" << endl;
        break;
    case DataTypes::typeShortPtr:
        out << "\t" << "CALL SetShort" << endl;
        break;
    case DataTypes::typeUShortPtr:
        out << "\t" << "CALL SetShort_Unsigned" << endl;
        break;
    case DataTypes::typeBytePtr:
        out << "\t" << "CALL SetByte" << endl;
        break;
    case DataTypes::typeUBytePtr:
        out << "\t" << "CALL SetByte_Unsigned" << endl;
        break;
    case DataTypes::typeStringPtr:
        out << "\t" << "CALL SetString" << endl;
        break;
    default:
        out << "\t" << "CALL SetInt" << endl;
    }
}

//!, -, ~, ++/--(front), *, (type cast), b(backward ++/--)
void Parser::outputUnaryOp(ExpressionNode* x, int& stackDepth, bool remainReturnStack) {

    if(x->op == "!") {
        treeDFS(x->left, stackDepth);

        switch(x->left->resultType) {
        case DataTypes::typeString:
            out << "\t" << "INST_4C" << endl; //pop, and push 0. (Wait, it is string to int?)
            break;

        case DataTypes::typeFloat:
            out << "\t" << "FTOL" << endl;
        default:
            out << "\t" << "ZERO" << endl;
        }

    } else if(x->op == "-") {
        treeDFS(x->left, stackDepth);
        out << "\t" << (x->left->resultType == DataTypes::typeFloat ? "NEGF" : "NEG") << endl;

    } else if(x->op == "~") {
        treeDFS(x->left, stackDepth);
        out << "\t" << "NOT" << endl;

    //HOLY SHIT...
    } else if(x->op == "++" || x->op == "--") {

        //change this node into an += or -= binaryOp.
        auto changeToAssignment = [&](string op, ExpressionNode* x, int& stackDepth, bool remainReturnStack) {
            x->op = (op == "++" ? "+=" : "-=");
            x->type = ExpNodeType::binaryOp;

            x->right = expNodePool.newNode();
            x->right->type = ExpNodeType::intConst;
            x->right->intValue = 1;
            x->right->resultType = DataTypes::typeInt;
            x->right->isLvalue = false;

            treeDFS(x, stackDepth, remainReturnStack);
        };

        //an assignment. Not quite easy.
        //lvalue possibly be: locals, intvs, local w/array (operator[]), ->, *.

        //local: if it is float, then it's no different from += 1. Otherwise, use INCN.
        if(x->left->type == ExpNodeType::local) {
            if(x->left->localVar.type == DataTypes::typeFloat) {
                changeToAssignment(x->op, x, stackDepth, remainReturnStack);

            } else {
                out << "\t" << (x->op == "++" ? "INCN " : "DECN ") << x->left->localVar.no
                    << " ; " << x->left->localVar.name << endl;

                if(remainReturnStack) {
                    out << "\t" << "PUSHARG " << x->left->localVar.no
                        << " ; " << x->left->localVar.name << endl;
                    stackDepth++;
                }
            }

        //intv: must be int. Use INCINV.
        } else if (x->left->type == ExpNodeType::intv) {
            out << "\t" << (x->op == "++" ? "INCINV " : "DECINV ") << x->left->intvVar.intvNo
                << " ; " << x->left->intvVar.name;

            if(remainReturnStack) {
                out << "\t" << "PUSHINV " << x->left->intvVar.intvNo
                    << " ; " << x->left->intvVar.name << endl;
                stackDepth++;
            }

        //operator[]. Only locals currently. Use INST_54/57 (INCNR/DECNR).
        } else if (x->left->type == ExpNodeType::binaryOp && x->left->op == "[]") {
            //for float, still change to += 1.
            if(x->left->left->localVar.type == DataTypes::typeFloat) {
                changeToAssignment(x->op, x, stackDepth, remainReturnStack);

            } else {
                treeDFS(x->left->right, stackDepth);

                //use local 1 to prevent re-calculation of the index.
                if(remainReturnStack) {
                    out << "\t" << "POPN 1" << endl;
                    out << "\t" << "PUSHARG 1" << endl;
                }

                out << "\t" << (x->op == "++" ? "INST_54 " : "INST_57 ")
                    << x->left->left->localVar.no << " ; " << x->left->left->localVar.name << endl;
                stackDepth--;

                if(remainReturnStack) {
                    out << "\t" << "PUSHARG 1" << endl;
                    out << "\t" << "PUSHNR " << x->left->localVar.no
                        << " ; " << x->left->localVar.name << endl;
                    stackDepth++;

                }
            }

        //operator * or ->. Change to +=, no optimization.
        } else {
            changeToAssignment(x->op, x, stackDepth, remainReturnStack);
        }

    //Backward ++/--: direct push (dealt in expr()).
    } else if (x->op == "b") {
        treeDFS(x->left, stackDepth);

    //Get value. Use functions in sg2lang.h, or SYSCALL 0x200 (for string ptrs).
    //Although it can be an assignment, such case will be dealt with at the parent node.
    //So won't deal with it here.
    } else if (x->op == "*") {
        treeDFS(x->left, stackDepth);
        outputGetValue(x->left->resultType);

    //Type cast.
    //int <-> float, int->string, float->string
    } else if (x->op[0] == '(') {
        int typeTo;
        sscanf(x->op.c_str(), "%d", &typeTo);
        if(typeTo == DataTypes::typeString) {
            if(x->left->resultType == DataTypes::typeFloat)
                out << "\t" << "INST_4B" << endl;
            else
                out << "\t" << "INST_49" << endl;

        } else if (typeTo == DataTypes::typeFloat)
            out << "\t" << "ITOF" << endl;

        else
            out << "\t" << "FTOL" << endl;
    }
}



// 0: =, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
// 1: &&, ||
// 2: &, |, ^
// 3: ==, !=, <, <=, >, >=
// 4: <<, >>
// 5: +, -
// 6: *, /, %
// 7: unary()
void Parser::outputBinaryOp(ExpressionNode* x, int& stackDepth, bool remainReturnStack) {

    //Get rid of '=' in the end
    string calcPart = x->op[x->op.length() - 1] == '=' ? x->op.substr(0, x->op.length() - 1) : x->op;
    string instruction = calcPart == "+" ? "ADD" :
                         calcPart == "-" ? "SUB" :
                         calcPart == "*" ? "MUL" :
                         calcPart == "/" ? "DIV" :
                         calcPart == "%" ? "MOD" :
                         calcPart == "<<" ? "SHL" :
                         calcPart == ">>" ? "SHR" :
                         calcPart == "==" ? "CMP" :
                         calcPart == "!=" ? "CMPZ" :
                         calcPart == "<" ? "CMPL" :
                         calcPart == ">" ? "CMPG" :
                         calcPart == "<=" ? "CMPLE" :
                         calcPart == ">=" ? "CMPGE" :
                         calcPart == "&" ? "AND" :
                         calcPart == "|" ? "OR" :
                         calcPart == "^" ? "XOR" :
                         calcPart == "&&" ? "ORNZ" :
                         calcPart == "||" ? "ORZ" : ""; //"" for operator =

    //HOLY SHIT...
    if(x->op == "=" || x->op == "+=" || x->op == "-=" || x->op == "*=" || x->op == "/=" ||
       x->op == "%=" || x->op == "&=" || x->op == "|=" || x->op == "^=" || x->op == "<<=" ||
       x->op == ">>=") {

        //an assignment. Complicated.
        //lvalue possibly be: locals, intvs, local w/array (operator[]), ->, *.

        auto isImmediateValue = [](ExpressionNode* x) -> bool {
            return x->type == ExpNodeType::intConst ||
                   x->type == ExpNodeType::strConst ||
                   x->type == ExpNodeType::local ||
                   x->type == ExpNodeType::intv;
        };

        auto getNewValue = [&](){
            if(x->right->resultType != x->resultType) //only two possibilites: float to int, int to float.
                out << "\t" << (x->right->resultType == DataTypes::typeFloat ? "FTOL" : "LTOF") << endl;

            //do calculation
            if(x->op != "=") {
                out << "\t" << instruction <<
                        (x->resultType == DataTypes::typeString ? "S" :
                         x->resultType == DataTypes::typeFloat  ? "F" : "")
                    << endl;
                stackDepth--;
            }
        };

        //local and intv
        if(x->left->type == ExpNodeType::local || x->left->type == ExpNodeType::intv) {
            if(x->op != "=") {
                out << "\t" << (x->left->type == ExpNodeType::local ? "PUSHARG " : "PUSHINV ")
                    << x->left->localVar.no << " ; " << x->left->localVar.name << endl;
                stackDepth++;
            }

            treeDFS(x->right, stackDepth);
            getNewValue();

            //set value
            out << "\t" << (x->left->type == ExpNodeType::local ? "POPN " : "SETINV ")
                << x->left->localVar.no << " ; " << x->left->localVar.name << endl;
            stackDepth--;

            //remainStack: repush into stack hier
            if(remainReturnStack) {
                out << "\t" << (x->left->type == ExpNodeType::local ? "PUSHARG " : "PUSHINV ")
                    << x->left->localVar.no << " ; " << x->left->localVar.name << endl;
                stackDepth++;
            }

        //operator[]. Only locals currently.
        } else if(x->left->type == ExpNodeType::binaryOp && x->left->op == "[]") {
            Var var = x->left->left->localVar;

            if(x->op != "=") {
                //get origin value
                treeDFS(x->left->right, stackDepth);
                if(!isImmediateValue(x->left->right)) {
                    out << "\t" << "POPN 1" << endl;
                    out << "\t" << "PUSHARG 1" << endl;
                }
                out << "\t" << "PUSHNR " << var.no << " ; " << var.name << endl;

                //get right operand and calc
                treeDFS(x->right, stackDepth);
                getNewValue();

                if(remainReturnStack){
                    out << "\t" << "POPN 2" << endl;
                    out << "\t" << "PUSHARG 2" << endl;
                }

                //set
                if(!isImmediateValue(x->left->right)) {
                    out << "\t" << "PUSHARG 1" << endl;
                    stackDepth++;
                } else
                    treeDFS(x->left->right, stackDepth);

                out << "\t" << "SETNR " << var.no << " ; " << var.name << endl;
                stackDepth -= 2;

                //result remains in stack
                if(remainReturnStack) {
                    out << "\t" << "PUSHARG 2" << endl;
                    stackDepth--;
                }

            } else {
                treeDFS(x->right, stackDepth);
                getNewValue();
                if(remainReturnStack){
                    out << "\t" << "POPN 2" << endl;
                    out << "\t" << "PUSHARG 2" << endl;
                }
                treeDFS(x->left->right, stackDepth);
                out << "\t" << "SETNR " << var.no << " ; " << var.name << endl;
                stackDepth -= 2;
                if(remainReturnStack) {
                    out << "\t" << "PUSHARG 2" << endl;
                    stackDepth++;
                }
            }

        //operator -> and *.
        } else if((x->left->type == ExpNodeType::binaryOp && x->left->op == "->") ||
                  (x->left->type == ExpNodeType::unaryOp  && x->left->op == "*")) {
            auto getAddr = [&]() {
                treeDFS(x->left->left, stackDepth);
                if(x->left->op == "->") {
                    //-> 's right operand should be an integer value.
                    treeDFS(x->left->right, stackDepth);
                    out << "\t" << "ADD" << endl;
                    stackDepth--;
                }
            };
            if(x->op != "=") {
                getAddr();
                outputGetValue(x->left->type);

                treeDFS(x->right, stackDepth);
                getNewValue();
                if(remainReturnStack) {
                    out << "\t" << "POPN 2" << endl;
                    out << "\t" << "PUSHARG 2" << endl;
                }
                outputSetValue(x->left->type);
                stackDepth -= 2;
                if(remainReturnStack) {
                    out << "\t" << "PUSHARG 2" << endl;
                    stackDepth++;
                }
            } else {
                getAddr();
                treeDFS(x->right, stackDepth);
                if(remainReturnStack) {
                    out << "\t" << "POPN 2" << endl;
                    out << "\t" << "PUSHARG 2" << endl;
                }
                outputSetValue(x->left->type);
                stackDepth -= 2;
                if(remainReturnStack) {
                    out << "\t" << "POPN 2" << endl;
                    out << "\t" << "PUSHARG 2" << endl;
                }
            }
        }


    } else {
        //comparisons and + for string:
        if(x->left->resultType == DataTypes::typeString) {
            treeDFS(x->left, stackDepth);
            treeDFS(x->right, stackDepth);
            //Hell knows why Henryshow uses CMPLEFS and CMPGEFS, rather than CMPLES and CMPGES...
            if(calcPart == ">=" || calcPart == "<=")
                instruction += "F";
            out << "\t" << instruction << "S" << endl;
            stackDepth--;

        //raise to float if one of the operands is float
        } else if(x->left->resultType == DataTypes::typeFloat || x->right->resultType == DataTypes::typeFloat) {
            treeDFS(x->left, stackDepth);
            if(x->left->resultType != DataTypes::typeFloat)
                out << "\t" << "LTOF" << endl;

            treeDFS(x->right, stackDepth);
            if(x->right->resultType != DataTypes::typeFloat)
                out << "\t" << "LTOF" << endl;

            out << "\t" << instruction << "F" << endl;
            stackDepth--;

        //no extra type cast
        } else {
            treeDFS(x->left, stackDepth);
            treeDFS(x->right, stackDepth);
            out << "\t" << instruction << endl;
            stackDepth--;
        }
    }
}

//search down the tree.
//to be exact, this will be a loooooong method. I will try to split it up a little bit.
//NOTE remainReturnStack = true in default.
void Parser::treeDFS(ExpressionNode* x, int& stackDepth, bool remainReturnStack) {

    if(x == NULL) {
        printf("!FATAL!: NULL ptr x in treeDFS\n");
        exit(1);
    }

    switch(x->type) {

    case ExpNodeType::intConst:
        out << "\t" << "PUSH " << x->intValue << endl;
        stackDepth++;
        break;

    case ExpNodeType::strConst:
        //Look up in the string table. String constants need to be pushed by 'PUSHSTR stringNo'
        if(getStringNo(x->strValue) == -1)
            newString(x->strValue);
        out << "\t" << "PUSHSTR " << getStringNo(x->strValue) << " ; " << x->strValue << endl;
        stackDepth++;
        break;

    case ExpNodeType::local:
        out << "\t" << "PUSHARG " << x->localVar.no << " ; " << x->localVar.name << endl;
        stackDepth++;
        break;

    case ExpNodeType::intv:
        out << "\t" << "PUSHINV " << x->intvVar.intvNo << " ; " << x->intvVar.name << endl;
        stackDepth++;
        break;

    case ExpNodeType::inst:
        outputInst(x, stackDepth);
        break;

    case ExpNodeType::funcCall:
        outputFuncCall(x, stackDepth);
        break;

    case ExpNodeType::syscall:
        for(int i = 0; i < x->sys->paramCount; i++)
            treeDFS(x->params[i], stackDepth);
        out << "SYSCALL 0x" << std::hex << x->sys->code << std::dec
                << ", (" << x->sys->paramCount << " | (" << x->sys->returnType << " << 16))"
                << " ; " << x->sys->name << endl;

        stackDepth -= x->sys->paramCount;
        if(x->sys->returnType != DataTypes::typeVoid)
            stackDepth++;

        break;

    case ExpNodeType::unaryOp:
        outputUnaryOp(x, stackDepth, remainReturnStack);
        break;

    case ExpNodeType::binaryOp:
        outputBinaryOp(x, stackDepth, remainReturnStack);
        break;

    }
}

void Parser::expr(int& tokenPos, bool remainReturnStack) {
    //TODO expr tree analysis
}
