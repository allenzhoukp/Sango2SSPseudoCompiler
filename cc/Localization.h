#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
using std::string;

// Careful when saving. It may contain words in other charsets.

#define LANG_CN_GB

#ifdef LANG_EN

namespace ErrMsg {

	const string errHeader = "Error ";

    const string digitNotMatch = "digit not match";
    const string stringNotMatch = "string not match";
    const string identNotMatch = "ident not match";
    const string lexerInvalidOp = "Invalid operator";
    const string asmLeftBrace = "Expect '{' after '__asm'";

    const string fileNotExist = "Could not find";

    const string require1 = "Error: expected ";
    const string require2 = ", found '";


    const string parameterNotMatch1 = "the number of parameters does not match. Required ";
    const string parameterNotMatch2 = " params, seen ";
    const string getGlobalIntParam = "GetGlobal() requires an integer parameter";
    const string setGlobalIntParam = "SetGlobal() requires integer parameters";
    const string getIntvIntParam = "GetIntv() requires an integer parameter";
    const string setIntvIntParam = "SetIntv() requires integer parameters";
    const string delayIntParam = "Delay() requires an integer parameter";
    const string instRequireStringParam = "() requires a string parameter";
    const string paramTypeNotMatch = "parameter type not match";
    const string funcParamNotMatch1 = "function ";
    const string funcParamNotMatch2 = " requires ";
    const string funcParamNotMatch3 = " parameter(s), found ";
    const string invalidStructPtr = "Invalid struct * type";
    const string structMemberNotFound1 = "struct member '";
    const string structMemberNotFound2 = "' not found in struct '";
    const string invalidArrayInStructtype = "invalid type for a struct-member array";
    const string directAccess = "invalid member access: structures or arrays in structures cannot be directly accessed";
    const string notSupportedArray = "Operator [] can only be applied to local variables or non-void pointers currently";
    const string invalidArrowOp = "Operator -> can only be applied to struct *";
    const string ppmmLvalue = "Operator ++/-- requires an lvalue operand (local variables, global variables, INTVs, etc.)";
    const string localArrayDirectAccess = "local arrays can only be accessed by operator []";
    const string unknownIdentifier = "Unknown identifier '";
    const string expectedExpr = "Expected an expression, found '";
    const string negativeString = "A negative sign cannot be placed in front of a string value.";
    const string opNotRequireInt = "Operator ~ requires an integer operand.";
    const string ppmmStringOperand = "Operator ++/-- cannot be used on a string";
    const string starOnPointers = "Operator * can only be applied on pointers";
    const string starOnInvalidPointer = "Operator * cannot be applied on a struct or void pointer";
    const string invalidTypeCast = "invalid type cast to (";
    const string bothStringRequired1 = "Can't apply operator ";
    const string bothStringRequired2 = " between a string and a non-string";
    const string invalidStringOperand = "Invalid operand type: string";
    const string invalidModFloat = "Invalid operand type: float (Sango2 does not support % instructions for float)";
    const string invalidFloatOperand = "Invalid operand type: float";
    const string assignmentLvalue = "assignment operator requires an lvalue as left operand";
    const string expectedFloatExpr = "expected an expression with float value";
    const string expectedStringExpr = "expected an expression with string value";
    const string expectedIntExpr = "expected an expression with integer value (i.e. int, short, byte, unsigned integers and pointers)";
    const char stackSizeIncrease[] = "Warning %s, line %d: unexpected stack size increase for %d. \n";

    const string requireIntegerConstant = "expected an integer constant here";
    const string labelRedefined = "label redefined";
    const char labelNotFound[] = "Warning %s, %d:%d: label %s not found in all non-asm blocks. Proceed at your own risk! \n";
    const string localRequireIdentifier = "invalid local variable name: it should only contains letters, numbers and '_', and should not start with numbers";
    const string voidLocal = "invalid local variable type: void";
    const string localRedefined = "there has already been a local variable named ";
    const string functionNotFound1 = "function '";
    const string functionNotFound2 = "' not found!";
    const string funcNameIdentifier = "invalid function name: it should only contains letters, numbers and '_', and should not start with numbers";
    const string funcNameConflict = "there has already been a function named ";
    const string invalidTypeName = "invalid type name: ";
    const string nakedRequireAsm = "a naked method can only have one __asm{} block as the function body";
    const string noExplicitReturn = "no explicit return at the end of the function body";
};

const string mainMessage = "Usage: ss <input> <output>";

#endif

#ifdef LANG_CN_GB

namespace ErrMsg {

	const string errHeader = "错误 ";

    const string digitNotMatch = "无法匹配数字";
    const string stringNotMatch = "无法匹配字符串";
    const string identNotMatch = "无法匹配标识符";
    const string lexerInvalidOp = "无效运算符";
    const string asmLeftBrace = "在'__asm'后应有'{'";

    const string fileNotExist = "无法找到include的文件";

    const string require1 = "应有'";
    const string require2 = "', 实际检测到 '";


    const string parameterNotMatch1 = "参数个数不符。需要 ";
    const string parameterNotMatch2 = " 个参数，但所给参数个数为 ";
    const string getGlobalIntParam = "GetGlobal() 需要整数类型的参数";
    const string setGlobalIntParam = "SetGlobal() 需要整数类型的参数";
    const string getIntvIntParam = "GetIntv() 需要整数类型的参数";
    const string setIntvIntParam = "SetIntv() 需要整数类型的参数";
    const string delayIntParam = "Delay() 需要整数类型的参数";
    const string instRequireStringParam = "() 需要字符串类型的参数";
    const string paramTypeNotMatch = "参数类型不符";
    const string funcParamNotMatch1 = "函数 ";
    const string funcParamNotMatch2 = " 需要 ";
    const string funcParamNotMatch3 = " 个参数，但所给参数个数为 ";
    const string invalidStructPtr = "结构体指针类型非法";
    const string structMemberNotFound1 = "结构体成员 '";
    const string structMemberNotFound2 = "' 并不属于结构体 '";
    const string invalidArrayInStructtype = "结构体内数组成员类型非法";
    const string directAccess = "不能直接取得结构体本身，或结构体内数组本身的值";
    const string notSupportedArray = " [] 运算符目前只能跟在局部变量或非无类型指针类型之后";
    const string invalidArrowOp = " -> 运算符只能跟在结构体指针之后";
    const string ppmmLvalue = " ++/-- 只能被应用于左值 （本地变量、intv、数组成员、结构体成员等）";
    const string localArrayDirectAccess = "局部变量数组必须通过 [] 运算符访问";
    const string unknownIdentifier = "未知标识符 '";
    const string expectedExpr = "此处需要表达式，实际为 '";
    const string negativeString = " - 运算符无法被放在字符串前";
    const string opNotRequireInt = " ~ 运算符需要整数操作数";
    const string ppmmStringOperand = " ++/-- 运算符不能被应用于字符串";
    const string starOnPointers = "取值运算符 * 只能被应用于指针";
    const string starOnInvalidPointer = "取值运算符 * 无法被应用于无类型指针(void *)或结构体指针";
    const string invalidTypeCast = "非法类型转换 (";
    const string bothStringRequired1 = "无法将运算符 ";
    const string bothStringRequired2 = " 用于字符串和非字符串之间";
    const string invalidStringOperand = "非法操作数类型：字符串";
    const string invalidModFloat = "非法操作数类型：浮点数 （系统不支持对浮点数取模）";
    const string invalidFloatOperand = "非法操作数类型：浮点数";
    const string assignmentLvalue = "赋值运算符需要左值（本地变量、intv、数组成员、结构体成员等）作为左操作数";
    const string expectedFloatExpr = "此处应有值为浮点类型的表达式";
    const string expectedStringExpr = "此处应有值为字符串类型的表达式";
    const string expectedIntExpr = "此处应有值为整数的表达式 （包括 int, short, byte, 无符号类型和指针）";
    const char stackSizeIncrease[] = "警告 %s, 行 %d ：预料之外的栈增长，栈顶多余 %d 个值。\n";

    const string requireIntegerConstant = "此处应有常整数";
    const string labelRedefined = "同名标签已被定义";
    const char labelNotFound[] = "";
    // const char labelNotFound[] = "Warning %s, %d:%d: 标签 %s 无法在所有非 __asm 块中找到。请谨慎。\n";
    const string localRequireIdentifier = "无效变量名：变量名应仅包含字母、数字和下划线，并且不能以数字开头";
    const string voidLocal = "无效局部变量类型：void";
    const string localRedefined = "同名局部变量已被定义： ";
    const string functionNotFound1 = "函数 '";
    const string functionNotFound2 = "' 未找到！";
    const string funcNameIdentifier = "无效函数名：函数名应仅包含字母、数字和下划线，并且不能以数字开头";
    const string funcNameConflict = "已有同名函数： ";
    const string invalidTypeName = "非法类型名称： ";
    const string nakedRequireAsm = "一个 naked 函数应只有一个 __asm{} 块作为函数体。__asm{} 与函数定义之间不应有大括号。可参考magic.cpp中的格式。";
    const string noExplicitReturn = "函数末尾应有显式的返回语句";
};

const string mainMessage = "使用方法: ss 输入文件名 输出文件名";

#endif

#endif
