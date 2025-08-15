#include "Parser.h"

#include <cstdio>
#include <cstring>
#include <regex>

void Parser::loadSyscallTable () {
    FILE* fin = fopen("config/SYSCALL_TABLE.txt", "r");
    Syscall syscall;
    char name[MAX_LINE_LEN];

    if(fin == NULL) {
        printf("Error: Unable to load SYSCALL_TABLE.txt!\n");
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
    FILE* fin = fopen("config/INTV_TABLE.txt", "r");
    Intv intv;
    char name[MAX_LINE_LEN];

    if(fin == NULL) {
        printf("Warning: Unable to load INTV_TABLE.txt. You can't access intvs by name in this case\n");
        return;
    }

    while(fscanf(fin, "%s %x", name, &intv.intvNo) != EOF) {
        intv.name = string(name);
        intvs[intvCount++] = intv;
    }
    fclose(fin);
}

void Parser::loadStructTable () {
    FILE* fin = fopen("config/STRUCT_TABLE.txt", "r");

    char buf[MAX_LINE_LEN];
    char line[MAX_LINE_LEN];

    if(fin == NULL) {
        printf("Warning: Unable to load STRUCT_TABLE.txt. Structure types are inaccessable in this case\n");
        return;
    }

    while(!feof(fin)) {
        StructInfo& info = structInfos[structInfoCount];
        info.id = structInfoCount++;
        int read;
        do {
            fgets(line, MAX_LINE_LEN, fin);
            read = sscanf(line, "%s%i", buf, &info.size);
        } while (read == 0 || read == -1);
        info.name = toCamel(string(buf));

        while(true) {
            fgets(line, MAX_LINE_LEN, fin);

            if(strncmp(line, "end", 3) == 0)
                break;

            StructMember& member = info.members[info.memberCount++];

            sscanf(line, "%X%s%n", &member.offset, buf, &read);
            member.name = string(buf);

            char* p = line + read;
            while(*p != '\0' && isspace(*p)) p++;
            for(char* q = p; *q != '\0'; q++){
                if(*q == ';') {
                    *q = '\0';
                    break;
                }
            }

            member.typeName = string(p);
        }
    }

    //Backup, and restore later.
    Token* tokensBak = tokens;
    int tokenCountBak = tokenCount;

    for(int i = 0; i < structInfoCount; i++) {
        for(int j = 0; j < structInfos[i].memberCount; j++) {
            StructMember &member = structInfos[i].members[j];
            //Borrow tokens[] array to use... fxck man why i write a getType() which is soooooo hard-wired to Lexer?!
            //And yes, Lexer is soooooo hard-wired to file i-o! (fxck error output!)
            tokens = new Token[10];
            tokenCount = 0;

            strcpy(line, member.typeName.c_str());
            char* p = line;
            while(*p != '\0') {
                tokens[tokenCount].content = "";

                while(!isspace(*p) && *p != '\0')
                    tokens[tokenCount].content += *p++;

                tokenCount++;
                while(isspace(*p) && *p != '\0') p++;
            }

            int pos = 0;
            member.type = getType(pos, true);
            if(member.type == -1)
                printf("Invalid type name! Struct name = %s, member name = %s\n", structInfos[i].name.c_str(), member.name.c_str());
            
            // arrays - never implemented before v0.9.5
            if(tokens[pos + 1].content == "dup") {
                member.isArray = true;
                // ignore the length - do not care about index out of bounds
            }

            delete[] tokens;
            tokenCount = 0;
        }
    }

    tokens = tokensBak;
    tokenCount = tokenCountBak;
}
