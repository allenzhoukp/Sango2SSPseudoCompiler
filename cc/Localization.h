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
    const string exefuncParamNotMatch1 = "function pointer ";
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
    const string invalidReturnType = "invalid return type: ";
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

	const string errHeader = "���� ";

    const string digitNotMatch = "�޷�ƥ������";
    const string stringNotMatch = "�޷�ƥ���ַ���";
    const string identNotMatch = "�޷�ƥ���ʶ��";
    const string lexerInvalidOp = "��Ч�����";
    const string asmLeftBrace = "��'__asm'��Ӧ��'{'";

    const string fileNotExist = "�޷��ҵ�include���ļ�";

    const string require1 = "Ӧ��'";
    const string require2 = "', ʵ�ʼ�⵽ '";


    const string parameterNotMatch1 = "����������������Ҫ ";
    const string parameterNotMatch2 = " ����������������������Ϊ ";
    const string getGlobalIntParam = "GetGlobal() ��Ҫ�������͵Ĳ���";
    const string setGlobalIntParam = "SetGlobal() ��Ҫ�������͵Ĳ���";
    const string getIntvIntParam = "GetIntv() ��Ҫ�������͵Ĳ���";
    const string setIntvIntParam = "SetIntv() ��Ҫ�������͵Ĳ���";
    const string delayIntParam = "Delay() ��Ҫ�������͵Ĳ���";
    const string instRequireStringParam = "() ��Ҫ�ַ������͵Ĳ���";
    const string paramTypeNotMatch = "�������Ͳ���";
    const string funcParamNotMatch1 = "���� ";
    const string exefuncParamNotMatch1 = "����ָ�� ";
    const string funcParamNotMatch2 = " ��Ҫ ";
    const string funcParamNotMatch3 = " ����������������������Ϊ ";
    const string invalidStructPtr = "�ṹ��ָ�����ͷǷ�";
    const string structMemberNotFound1 = "�ṹ���Ա '";
    const string structMemberNotFound2 = "' �������ڽṹ�� '";
    const string invalidArrayInStructtype = "�ṹ���������Ա���ͷǷ�";
    const string directAccess = "����ֱ��ȡ�ýṹ�屾����ṹ�������鱾���ֵ";
    const string notSupportedArray = " [] �����Ŀǰֻ�ܸ��ھֲ��������������ָ������֮��";
    const string invalidArrowOp = " -> �����ֻ�ܸ��ڽṹ��ָ��֮��";
    const string ppmmLvalue = " ++/-- ֻ�ܱ�Ӧ������ֵ �����ر�����intv�������Ա���ṹ���Ա�ȣ�";
    const string localArrayDirectAccess = "�ֲ������������ͨ�� [] ���������";
    const string unknownIdentifier = "δ֪��ʶ�� '";
    const string expectedExpr = "�˴���Ҫ���ʽ��ʵ��Ϊ '";
    const string negativeString = " - ������޷��������ַ���ǰ";
    const string opNotRequireInt = " ~ �������Ҫ����������";
    const string ppmmStringOperand = " ++/-- ��������ܱ�Ӧ�����ַ���";
    const string starOnPointers = "ȡֵ����� * ֻ�ܱ�Ӧ����ָ��";
    const string starOnInvalidPointer = "ȡֵ����� * �޷���Ӧ����������ָ��(void *)��ṹ��ָ��";
    const string invalidTypeCast = "�Ƿ�����ת�� (";
    const string bothStringRequired1 = "�޷�������� ";
    const string bothStringRequired2 = " �����ַ����ͷ��ַ���֮��";
    const string invalidStringOperand = "�Ƿ����������ͣ��ַ���";
    const string invalidModFloat = "�Ƿ����������ͣ������� ��ϵͳ��֧�ֶԸ�����ȡģ��";
    const string invalidFloatOperand = "�Ƿ����������ͣ�������";
    const string assignmentLvalue = "��ֵ�������Ҫ��ֵ�����ر�����intv�������Ա���ṹ���Ա�ȣ���Ϊ�������";
    const string expectedFloatExpr = "�˴�Ӧ��ֵΪ�������͵ı��ʽ";
    const string expectedStringExpr = "�˴�Ӧ��ֵΪ�ַ������͵ı��ʽ";
    const string expectedIntExpr = "�˴�Ӧ��ֵΪ�����ı��ʽ ������ int, short, byte, �޷������ͺ�ָ�룩";
    const char stackSizeIncrease[] = "���� %s, �� %d ��Ԥ��֮���ջ������ջ������ %d ��ֵ��\n";

    const string requireIntegerConstant = "�˴�Ӧ�г�����";
    const string invalidReturnType = "����ֵ���Ϸ���";
    const string labelRedefined = "ͬ����ǩ�ѱ�����";
    const char labelNotFound[] = "";
    // const char labelNotFound[] = "Warning %s, %d:%d: ��ǩ %s �޷������з� __asm �����ҵ����������\n";
    const string localRequireIdentifier = "��Ч��������������Ӧ��������ĸ�����ֺ��»��ߣ����Ҳ��������ֿ�ͷ";
    const string voidLocal = "��Ч�ֲ��������ͣ�void";
    const string localRedefined = "ͬ���ֲ������ѱ����壺 ";
    const string functionNotFound1 = "���� '";
    const string functionNotFound2 = "' δ�ҵ���";
    const string funcNameIdentifier = "��Ч��������������Ӧ��������ĸ�����ֺ��»��ߣ����Ҳ��������ֿ�ͷ";
    const string funcNameConflict = "����ͬ�������� ";
    const string invalidTypeName = "�Ƿ��������ƣ� ";
    const string nakedRequireAsm = "һ�� naked ����Ӧֻ��һ�� __asm{} ����Ϊ�����塣__asm{} �뺯������֮�䲻Ӧ�д����š��ɲο�magic.cpp�еĸ�ʽ��";
    const string noExplicitReturn = "����ĩβӦ����ʽ�ķ������";
};

const string mainMessage = "ʹ�÷���: ss �����ļ��� ����ļ���";

#endif

#endif
