#include "Parser.h"

#include <cstdio>
#include <cstring>

void Parser::loadSyscallTable () {
    FILE* fin = fopen("SYSCALL_TABLE.txt", "r");
    Syscall syscall;
    char name[MAX_LINE_LEN];

    if(fin == NULL) {
        printf("Unable to load SYSCALL_TABLE.txt!");
        exit(1);
    }

    while(fscanf(fin, "%x %d %d %s", &syscall.code, &syscall.paramCount,
                      &syscall.returnType, name) != EOF) {
        syscall.name = string(name);
        syscalls[syscallCount] = syscall;
        syscallNameMapping[name] = &syscalls[syscallCount];
        syscallCount++;
    }
    fclose(fin);
}

void Parser::loadIntvTable () {
    FILE* fin = fopen("INTV_TABLE.txt", "r");
    Intv intv;
    char name[MAX_LINE_LEN];

    if(fin == NULL)
        return;

    while(fscanf(fin, "%s %x", name, &intv.intvNo) != EOF) {
        intv.name = string(name);
        intvs[intvCount++] = intv;
    }
    fclose(fin);
}

void Parser::loadStructTable () {
    FILE* fin = fopen("STRUCT_TABLE.txt", "r");

    char buf[MAX_LINE_LEN];
    char line[MAX_LINE_LEN];

    while(!feof(fin)) {
        StructInfo& info = structInfos[structInfoCount];
        info.id = structInfoCount++;
        fscanf(fin, "%s", buf);
        info.name = string(buf);
        fscanf(fin, "0x%x", &info.size);

        while(true) {
            fgets(line, MAX_LINE_LEN, fin);
            if(strncmp(line, "end", 3) == 0)
                break;
            StructMember& member = info.members[info.memberCount];
            char* typePos = line + sscanf(line, "%x%s", &member.offset, buf);
            member.name = string(buf);
            member.typeName = "";
            //copy all non-comments into typeName.
            while(*typePos != ';')
                member.typeName += typePos++;
        }
    }
    for(int i = 0; i < structInfoCount; i++) {
        for(int j = 0; j < structInfos[i].memberCount; j++) {
            StructMember &member = structInfos[i].members[j];
            //Borrow tokens[] array to use... fxck man why i write a getType() which is soooooo hard-wired to Lexer?!
            //And yes, Lexer is soooooo hard-wired to file i-o! (fxck error output!)
            tokens = new Token[10];
            tokenCount = 0;

            strcpy(line, member.typeName.c_str());
            char* p = line;
            int read;
            while((read = sscanf(p, "%s", buf)) != 0) {
                tokens[tokenCount++].content = string(buf);
                p += read;
            }

            int pos = 0;
            member.type = getType(pos, false);
            if(member.type == -1) {
                if(member.typeName == "unsigned" || member.typeName == "unsigned int")
                    member.type = StructMember::typeUInt;
                else if (member.typeName == "short")
                    member.type = StructMember::typeShort;
                else if (member.typeName == "byte")
                    member.type = StructMember::typeByte;
                else if (member.typeName == "unsigned short")
                    member.type = StructMember::typeUShort;
                else if (member.typeName == "unsigned byte")
                    member.type = StructMember::typeUByte;
                else {
                    StructInfo* info = getStructInfoByName(member.typeName);
                    member.type = info->id;
                    if(info->id == -1)
                        printf("Invalid type name! Struct name = %s, member name = %s\n", structInfos[i].name.c_str(), member.name.c_str());
                }
            }

            delete[] tokens;
            tokenCount = 0;
        }
    }
}
