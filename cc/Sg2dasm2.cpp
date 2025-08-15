#include <cstdio>
#include <iostream>
#include <fstream>

#include "Sg2dasm2.h"
// #include "Config.h"
#include "Localization.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::map;

void FixedIdentifierLoader::loadSyscallTable () {
    FILE* fin = fopen("config/SYSCALL_TABLE.txt", "r");
    Syscall syscall;
    char name[MAX_LINE_LEN];

    if(fin == NULL) {
        printf("Warning: Unable to load SYSCALL_TABLE.txt.\n");
        return;
    }

    while(fscanf(fin, "%x %d %d %s", &syscall.code, &syscall.paramCount,
                    &syscall.returnType, name) != EOF) {
        syscall.name = string(name);
        // will override if exists - use the last name in table
        syscalls[syscall.code] = syscall; 
    }
    fclose(fin);
}

void FixedIdentifierLoader::loadIntvTable () {
    FILE* fin = fopen("config/INTV_TABLE.txt", "r");
    Intv intv;
    char name[MAX_LINE_LEN];

    if(fin == NULL) {
        printf("Warning: Unable to load INTV_TABLE.txt.\n");
        return;
    }

    while(fscanf(fin, "%s %x", name, &intv.intvNo) != EOF) {
        intv.name = string(name);
        // will override if exists - use the last name in table
        intvs[intv.intvNo] = intv;
    }
    fclose(fin);
}

DAsmLabel* addLabel (int addr) {
    DAsmLabel* label = &labels[labelCount++];
    label->addr = addr;
    return label;
}

void readLine (char* input, int& addr, FixedIdentifierLoader& loader) {

    // Get or create the line
    // (Line may already exist due to previous jumps)
    Line& line = lines.find(addr) != lines.end() ? lines[addr] : (lines[addr] = Line());

    int instcode = *((int *)(input + addr));
    if (instcode >= 0x70 || instcode < 0) {
        // Not an inst. 
        // Fortunately we do not have "outside-func" variables
        // so we can safely assume it a string def.

        // Always assume it aligned to 0x4
        // Line& line = lines[addr / 4];

        line.instCode = -1;
        line.addr = addr;
        char strContentBuf[MAX_LINE_LEN];
        // will terminate at '\0'
        strcpy(strContentBuf, (const char *)(input + addr));

        // escape the content
        // it might recognize some bytes within a multibyte.
        // But this is only for showing the content, so ok-ish
        char strContent[MAX_LINE_LEN];
        for (char *p = strContentBuf, *q = strContent; ; p++, q++) {
            switch(*p) {
                case '\t':
                    *q = '\\'; q++; *q = 't'; break;
                case '\n':
                    *q = '\\'; q++; *q = 'n'; break;
                case '\r':
                    *q = '\\'; q++; *q = 'r'; break;
                case '\a':
                    *q = '\\'; q++; *q = 'a'; break;
                case '\b':
                    *q = '\\'; q++; *q = 'b'; break;
                case '\f':
                    *q = '\\'; q++; *q = 'f'; break;
                case '\v':
                    *q = '\\'; q++; *q = 'v'; break;
                case '\\':
                    *q = '\\'; q++; *q = '\\'; break;
                case '"':
                    *q = '\\'; q++; *q = '"'; break;
                default:
                    *q = *p;
            }
            if (*p == '\0') break;
        }

        // check length - use the non-escaped string len (!)
        // Here assume strongly that everything is aligned at 0x4
        int len = strlen(strContentBuf);
        // int dbRequired = (len + 1) / 4;  // trailing '\0'
        // if ((len + 1) % 4) dbRequired++;

        // Because of the way sg2asm is coded,
        // String defs are outputted a la Compiler 
        // (a sequence of DD's).
        // EDIT since it is not aligned to 0x4, we use a sequence of DB's instead.
        line.label = addLabel(addr);
        line.content = "";
        char buf[16];
        for(int i = 0; i <= len; i++) { // include the '\0' terminator
            sprintf(buf, "DB 0x%X\n\t", *((unsigned char *)(input + addr + i)));
            line.content += buf;
        }

        // move next
        addr += (len + 1);
        
        // might be aligned to 0x4 (Henryshow artifacts) - skip zeros
        while(*(input + addr) == 0) {
            addr++;
            line.content += "DB 0x0\n\t";
        }

        line.content += "; \"" + string(strContent) + "\"";

        // add to the string defs
        stringDefs.emplace(line.addr, strContent);

        return;
    }

    // Not a string def - normal inst
    Inst ins = instList[instcode];
    line.addr = addr;
    line.instCode = instcode;

    if (ins.type == InstType::OP_NO) {
        addr += 4;
        line.content = ins.symbol;

    } else if (ins.type == InstType::OP_L) {
        addr += 4;
        int operand = *((int *)(input + addr));
        addr += 4;
        
        char buf[MAX_LINE_LEN];
        Line& labelLine = lines.find(operand) != lines.end() ? 
                            lines[operand] : 
                            (lines[operand] = Line());
        switch(instcode) {
            case 0x4:   // CALL
            case 0x3B:  // JMP
            case 0x3C:  // JNZ
            case 0x3D:  // JZ
                // emit a new label at the target
                sprintf(buf, "LOC_%X", operand);
                labelLine.label = addLabel(operand);
                labelLine.label->addr = operand;
                labelLine.label->name = buf;

                // print out the jump inst
                sprintf(buf, "%s LOC_%X", ins.symbol, operand);
                break;

            case 0xA:   // PUSHINV
            case 0xD:   // SETINV
            case 0x1F:  // INCINV
            case 0x22:  // DECINV
            case 0x50:  // PUSHINVR
            case 0x53:  // SETINVR
            case 0x56:  // INCINVR
            case 0x59:  // DECINVR
                if (loader.intvs.find(operand) != loader.intvs.end()) {
                    sprintf(buf, "%s %d ; %s", ins.symbol, operand, loader.intvs[operand].name.c_str());
                } else {
                    sprintf(buf, "%s %d ; intv_%d", ins.symbol, operand, operand);
                    
                }
                break;

            default:
                sprintf(buf, "%s %d", ins.symbol, operand);
        }
        line.content = buf;

    } else {
        addr += 4;
        int operand = *((int *)(input + addr));
        addr += 4;
        int operand2 = *((int *)(input + addr));
        addr += 4;
        
        char buf[MAX_LINE_LEN];
        // must be defined outside switch
        // but only meaningful for JCOND, not SYSCALL
        Line& labelLine = lines.find(operand2) != lines.end() ? 
                            lines[operand2] : 
                            (lines[operand2] = Line());
        switch(instcode) {
            case 0x5:   // SYSCALL
                if (loader.syscalls.find(operand) != loader.syscalls.end()) {
                    sprintf(buf, "%s 0x%X, (%d | (%d << 16)) ; %s",
                        ins.symbol,
                        operand,
                        operand2 & 0xFFFF,
                        operand2 >> 16,
                        loader.syscalls[operand].name.c_str()
                    );
                } else {
                    sprintf(buf, "%s 0x%X, (%d | (%d << 16)) ; 0x%X",
                        ins.symbol,
                        operand,
                        operand2 & 0xFFFF,
                        operand2 >> 16,
                        operand
                    );
                }
                break;
            case 0x40:  // JCOND
                // emit a new label at the target
                sprintf(buf, "LOC_%X", operand2);
                labelLine.label = addLabel(operand2);
                labelLine.label->addr = operand2;
                labelLine.label->name = buf;

                // print out the jump inst
                sprintf(buf, "%s %d, LOC_%X",
                    ins.symbol, operand, operand2); 
                break;
            default:
                sprintf(buf, "%s %d, %d", ins.symbol, operand, operand2);
        }
        line.content = buf;
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("%s", mainMessage.c_str());
        return 0;
    }
    FixedIdentifierLoader loader = FixedIdentifierLoader();
    loader.loadIntvTable();
    loader.loadSyscallTable();

    printf("Sango Script Disassembler v0.9.8\n");

    // Config::loadConfig();
    
    char* infileName = new char[MAX_LOCAL_NUM];
    if (infileName[0] == '.' && (infileName[1] == '/' || infileName[1] == '\\'))
        infileName += 2;
    for (int i = 0; i <= strlen(argv[1]); i++) {
        if (argv[1][i] == '/') argv[1][i] = '\\';
        if (argv[1][i] == '.' || argv[1][i] == '\0') {
            strncpy(infileName, argv[1], i);
            infileName[i] = '\0';
            break;
        }
    }
    string str_infileName (infileName);
    string str_outfileName = str_infileName + ".asm";
    const char* outfileName = str_outfileName.c_str();

    // load the .so file to input
    FILE* fin = fopen(argv[1], "rb");
    
	fseek(fin, 0, SEEK_END);
	int finLength = ftell(fin);
	fseek(fin, 0, SEEK_SET);
    fread(input, 1, finLength, fin);

    fclose(fin);

    // read header
    int* in = (int *) (input + 0x10);
    int version = *in;
    int size = *(++in);
    int magicTableAddr = *(++in);
    int magicTableCount = *(++in);
    int stringTableAddr = *(++in);
    int stringTableCount = *(++in);
    int globalVarsCount = *(++in);
    int fileEndAddr = *(++in);

    // write header
    FILE* fout = fopen(outfileName, "w");
    fprintf(fout, R"(; === Assembly file generated by Sango Script Disassembler v2 ===
procedure macro addr, initstack, description, sequence
    DD addr
    DD initstack
    DD description
    DD sequence
endm

magic_table_count	equ	(magic_table_end - magic_table_addr) / 0x10
string_table_count	equ	(string_table_end - string_table_addr) / 0x04
global_vars_count   equ %d

org 00h
    DD "Smart Script"		; signature
org 10h
    DD 0x%x 			; version
    DD 0x%x			    ; size
    DD magic_table_addr 	;
    DD magic_table_count	;
    DD string_table_addr	;
    DD string_table_count	;
    DD global_vars_count    ;
    DD file_end				; file length

org %xh
)", 
        globalVarsCount, version, size, size);

    // run a pass through code section
    // also includes string definitions
    int addr = size;    // 0x40 by default
    while (addr < magicTableAddr) {
        int nextaddr = addr;
        readLine(input, nextaddr, loader);
        addr = nextaddr;
    }
    int codeSectionEnd = addr;

    // magic_table
    addr = magicTableAddr;
    for (int i = 0; i < magicTableCount; i++) {
        FunctionDef func;
        func.addr = *((int *) (input + addr));
        func.initStack = *((int *) (input + addr + 4));
        func.description = *((int *) (input + addr + 8));
        func.sequence = *((int *) (input + addr + 12));

        // add function name to label
        Line& labelLine = lines[func.addr];
        labelLine.label = addLabel(func.addr);

        // Find the function name as a DD'd string
        // and set the label at the function entry
        if (stringDefs.find(func.description) != stringDefs.end()) {
            labelLine.label->name = stringDefs[func.description];

        } else {
            printf("ERROR @ 0x%X: could not find function name string located at 0x%x\n",
                addr, func.description);
            return 1;
        }
        functions.push_back(func);
        addr += 0x10;
    }

    // string_table
    addr = stringTableAddr;
    for (int i = 0; i < stringTableCount; i++) {
        int stringAddr = *((int *) (input + addr));
        stringTable.push_back(stringAddr);

        // fill the stringdef label name according to its loc within the string_table
        // note that the string def is already defined (this is not the string table def)
        if (stringDefs.find(stringAddr) != stringDefs.end()) {
            char buf[20];
            sprintf(buf, "STRING_%d", i);
            lines[stringAddr].label->name = buf; // fill the label name

        } else if (*(input + stringAddr) == 0) {
            // only do special case for empty string
            lines[stringAddr] = Line();
            lines[stringAddr].addr = stringAddr;
            lines[stringAddr].label = addLabel(stringAddr);
            lines[stringAddr].content = "DB 0x0 \n\t; \"\"";
            stringDefs.emplace(stringAddr, "");

            char buf[20];
            sprintf(buf, "STRING_%d", i);
            lines[stringAddr].label->name = buf; // fill the label name

        } else {
            printf("ERROR @ 0x%X: could not find string located at 0x%x\n",
                addr, stringAddr);
            return 1;
        }
        addr += 4;
    }

    // for original .so file, magic names are not in string table
    // so will need to search for them.
    for (int i = 0; i < magicTableCount; i++) {
        int funcNameAddr = functions[i].description;
        Line& funcNameLine = lines[funcNameAddr];
        if (funcNameLine.label->name == "") {
            char buf[16];
            sprintf(buf, "MAGICNAME_%d", i);
            funcNameLine.label->name = buf;
        }
    }

    // EDIT: While SmartScript is case-sensitive, the assembler is not!
    // Check for label conflicts.
    std::map<std::string, int> labelConflicts;
    for (auto it = lines.begin(); it != lines.end(); it++) {
        Line& line = it->second;
        if (line.label != nullptr) {
            std::string labelName = line.label->name;
            for (int i = 0; i < labelName.length(); i++)
                labelName[i] = toupper(labelName[i]);

            if (labelConflicts.find(labelName) == labelConflicts.end()) {
                labelConflicts[labelName] = 0;
            } else {
                string oldName = line.label->name;
                line.label->name += "_" + std::to_string(labelConflicts[labelName] + 1);
                labelConflicts[labelName]++;

                printf("WARNING: Label conflict for %s at 0x%X. Renamed function entry label to %s\n",
                    oldName.c_str(), line.addr, line.label->name.c_str());
            }
        }
    }

    // Do a parse back in the code section, and output
    for (auto it = lines.begin(); it != lines.end(); it++) {
        Line& line = it->second;
        if (line.addr) {
            // emit label if exists
            if (line.label != nullptr) {
                // newline at the start of a function
                if (strncmp("LOC_", line.label->name.c_str(), 4) != 0)
                    fprintf(fout, "\n");

                fprintf(fout, "%s:\n", line.label->name.c_str());
            }

            // emit inst
            int operand, stringAddr;
            char instname[MAX_LINE_LEN], newcontent[MAX_LINE_LEN];
            switch(line.instCode) {
                case 0x7:   // PUSHSTR
                    sscanf(line.content.c_str(), "PUSHSTR %d", &operand);
                    stringAddr = stringTable[operand];
                    if (stringDefs.find(stringAddr) != stringDefs.end()) {
                        line.content += "; \"" + stringDefs[stringAddr] + "\"";
                    } else {
                        printf("ERROR @ 0x%X: could not find string located at 0x%x\n",
                            addr, stringAddr);
                        return 1;
                    }
                    break;

                case 0x4:   // CALL
                case 0x3B:  // JMP
                case 0x3C:  // JNZ
                case 0x3D:  // JZ
                    sscanf(line.content.c_str(), "%s LOC_%X", instname, &operand);
                    if (lines[operand].label != nullptr) {
                        sprintf(newcontent, "%s %s", instname, lines[operand].label->name.c_str());
                        line.content = newcontent;
                    }
                    break;
            } 

            // print the line
            fprintf(fout, "\t%s\n", line.content.c_str());
        }
    }

    // Output magic table
    fprintf(fout, "\nmagic_table_addr:\n");
    for (int i = 0; i < magicTableCount; i++){
        fprintf(fout, "MAGIC_%d: procedure %s %d %s %d\n",
            i,
            // stringDefs[functions[i].description].c_str(),
            lines[functions[i].addr].label->name.c_str(),
            functions[i].initStack,
            lines[functions[i].description].label->name.c_str(),
            functions[i].sequence
        );
    }
    fprintf(fout, "\nmagic_table_end:\n");

    // Output string table
    fprintf(fout, "\nstring_table_addr:\n");
    for (int i = 0; i < stringTableCount; i++) {
        fprintf(fout, "DD STRING_%d\n", i);
    }

    fprintf(fout, "string_table_end:\n\n");

    // over.
    fprintf(fout, "file_end:\n; dummy for assembler, not in original file\nDB \"Author: Henryshow, Allen Zhou\"");
    fclose(fout);

    printf("Decompile Successful !");
    
    return 0;
}