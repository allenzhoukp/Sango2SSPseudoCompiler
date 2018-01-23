#include "Parser.h"

#include <cstdio>
#include <cstring>

void Parser::loadSyscallTable () {
    FILE* fin = fopen("config/SYSCALL_TABLE.txt", "r");
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
    FILE* fin = fopen("config/INTV_TABLE.txt", "r");
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
    FILE* fin = fopen("config/STRUCT_TABLE.txt", "r");

    char buf[MAX_LINE_LEN];
    char line[MAX_LINE_LEN];

    /*
    while(!feof(fin)) {
        StructInfo& info = structInfos[structInfoCount];
        info.id = structInfoCount++;
        fscanf(fin, "%s", buf);
        info.name = toCamel(string(buf));
        printf("%s\n", info.name.c_str());
        fscanf(fin, "%i", &info.size);

        while(true) {
            fscanf(fin, "%s", buf);
            if(strncmp(buf, "end", 3) == 0)
                break;
            StructMember& member = info.members[info.memberCount++];
            sscanf(buf, "%x", &member.offset);
            fscanf(fin, "%s", buf);
            member.name = string(buf);
            member.typeName = "";
            //copy all non-comments into typeName.
            char c;
            while(isspace(c = fgetc(fin)));
            member.typeName += c;
            while(true) {
                c = fgetc(fin);
                if(c == ';' || c == '\n')
                    break;
                member.typeName += c;
            }
            while((c = fgetc(fin)) != '\n');
        }
    }
    */

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
            member.type = getType(pos, false);
            if(member.type == -1)
                printf("Invalid type name! Struct name = %s, member name = %s\n", structInfos[i].name.c_str(), member.name.c_str());

            delete[] tokens;
            tokenCount = 0;
        }
    }

    tokens = tokensBak;
    tokenCount = tokenCountBak;
}
