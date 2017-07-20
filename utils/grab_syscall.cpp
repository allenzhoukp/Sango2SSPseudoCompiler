#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>

char buffer[1024];
struct Syscall{
    int b,c;
    bool exists;
    char name[1024];
} table[2048];

void doit(FILE* fin) {
    while(fgets(buffer, 1024, fin) != NULL) {
        char* target = buffer;
        while(*target == '\t' || *target == ' ')
            target++;
        if(strncmp("SYSCALL", target, 7) == 0) {
            int a,b,c;
            char name[1024];
            sscanf(target, "SYSCALL %x, (%d | (%d << 16)) ; %s", &a, &b, &c, name);
            table[a].b = b; table[a].c = c;
            table[a].exists = true;
            if(strncmp(name, "0x", 2) != 0)
                strcpy(table[a].name, name);
        }
    }
}

int main() {

    freopen("SYSCALL_TABLE.txt", "w", stdout);

    FILE* f1 = fopen("magic.asm", "r");
    FILE* f2 = fopen("system.asm", "r");

    for(int i = 0; i < 2048; i++) {
        char default_name[1024];
        sprintf(default_name, "SYSCALL_0x%04X", i);

        strcpy(table[i].name, default_name);
    }

    doit(f1);
    doit(f2);

    fclose(f1);
    fclose(f2);

    for(int i = 0; i < 2048; i++)
        if(table[i].exists)
            printf("0x%X %d %d %s\n", i, table[i].b, table[i].c, table[i].name);

    return 0;
}
