#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "Localization.h"
#include "Sg2dasm2.h"

using std::int32_t;
using std::map;
using std::set;
using std::string;
using std::uint32_t;
using std::vector;

void FixedIdentifierLoader::loadSyscallTable() {
    // Load optional syscall names for output comments.
    std::ifstream input(SYSCALL_TABLE_PATH);
    Syscall syscall;
    uint32_t code;

    if (!input) {
        std::cout << "Warning: Unable to load " << SYSCALL_TABLE_PATH << ".\n";
        return;
    }

    while (input >> std::hex >> code >> std::dec >> syscall.paramCount
                 >> syscall.returnType >> syscall.name) {
        syscall.code = static_cast<int>(code);
        syscalls[code] = syscall;
    }
}

void FixedIdentifierLoader::loadIntvTable() {
    // Load optional INTV names for output comments.
    std::ifstream input(INTV_TABLE_PATH);
    Intv intv;
    uint32_t intvNo;

    if (!input) {
        std::cout << "Warning: Unable to load " << INTV_TABLE_PATH << ".\n";
        return;
    }

    while (input >> intv.name >> std::hex >> intvNo >> std::dec) {
        intv.intvNo = static_cast<int>(intvNo);
        intvs[static_cast<int32_t>(intvNo)] = intv;
    }
}

uint32_t readUint32(const vector<unsigned char>& input, uint32_t offset) {
    // Read one little-endian DWORD.
    return static_cast<uint32_t>(input[offset])
         | static_cast<uint32_t>(input[offset + 1]) << 8
         | static_cast<uint32_t>(input[offset + 2]) << 16
         | static_cast<uint32_t>(input[offset + 3]) << 24;
}

void writeRawData(std::ofstream& output, const vector<unsigned char>& input,
                  uint32_t begin, uint32_t end) {
    // Output raw bytes as DD directives followed by any remaining DB directives.
    while (begin + 4 <= end) {
        output << "\tDD 0x" << std::hex << std::uppercase << std::setw(8)
               << std::setfill('0') << readUint32(input, begin) << "\n";
        begin += 4;
    }
    while (begin < end) {
        output << "\tDB 0x" << std::hex << std::uppercase << std::setw(2)
               << std::setfill('0') << static_cast<unsigned>(input[begin]) << "\n";
        ++begin;
    }
    output << std::dec << std::nouppercase << std::setfill(' ');
}

string escapeCommentString(const string& value) {
    // Escape a byte string for a readable assembly comment.
    std::ostringstream escaped;
    for (char byte : value) {
        const unsigned char c = static_cast<unsigned char>(byte);
        switch (c) {
            case '\t': escaped << "\\t"; break;
            case '\n': escaped << "\\n"; break;
            case '\r': escaped << "\\r"; break;
            case '\\': escaped << "\\\\"; break;
            case '"': escaped << "\\\""; break;
            default:
                if (c >= 0x20 && c != 0x7F) {
                    escaped << static_cast<char>(c);
                } else {
                    escaped << "\\x" << std::hex << std::uppercase
                            << std::setw(2) << std::setfill('0')
                            << static_cast<unsigned>(c)
                            << std::dec << std::nouppercase << std::setfill(' ');
                }
        }
    }
    return escaped.str();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << dasmMainMessage << '\n';
        return 1;
    }

    std::ifstream source(argv[1], std::ios::binary);
    if (!source) {
        std::cerr << "Error: Unable to open " << argv[1] << ".\n";
        return 1;
    }

    // Read the complete input file.
    vector<unsigned char> input((std::istreambuf_iterator<char>(source)),
                                std::istreambuf_iterator<char>());
    if (input.size() < 0x40
            || !std::equal(input.begin(), input.begin() + 12, "Smart Script")) {
        std::cerr << "Error: Not a Smart Script object file.\n";
        return 1;
    }

    // Read and validate the header.
    const uint32_t version = readUint32(input, 0x10);
    const uint32_t codeStart = readUint32(input, 0x14);
    const uint32_t magicTableAddr = readUint32(input, 0x18);
    const uint32_t magicTableCount = readUint32(input, 0x1C);
    const uint32_t stringTableAddr = readUint32(input, 0x20);
    const uint32_t stringTableCount = readUint32(input, 0x24);
    const uint32_t globalVarsCount = readUint32(input, 0x28);
    const uint32_t fileEndAddr = readUint32(input, 0x2C);

    const uint64_t magicTableEnd = static_cast<uint64_t>(magicTableAddr)
                                 + static_cast<uint64_t>(magicTableCount) * 16;
    const uint64_t stringTableEnd = static_cast<uint64_t>(stringTableAddr)
                                  + static_cast<uint64_t>(stringTableCount) * 4;
    if (codeStart < 0x30 || codeStart > magicTableAddr
            || magicTableEnd > stringTableAddr
            || stringTableEnd > fileEndAddr
            || fileEndAddr > input.size()
            || codeStart % 4 != 0 || magicTableAddr % 4 != 0
            || stringTableAddr % 4 != 0 || fileEndAddr % 4 != 0) {
        std::cerr << "Error: The section addresses in the Smart Script header are invalid.\n";
        return 1;
    }

    // Read the function table.
    vector<FunctionDef> functions;
    functions.reserve(magicTableCount);
    for (uint32_t i = 0; i < magicTableCount; ++i) {
        const uint32_t addr = magicTableAddr + i * 16;
        functions.push_back({
            readUint32(input, addr),
            static_cast<int32_t>(readUint32(input, addr + 4)),
            readUint32(input, addr + 8),
            static_cast<int32_t>(readUint32(input, addr + 12))
        });
    }

    // Read the runtime string table.
    vector<uint32_t> stringTable;
    stringTable.reserve(stringTableCount);
    for (uint32_t i = 0; i < stringTableCount; ++i) {
        stringTable.push_back(readUint32(input, stringTableAddr + i * 4));
    }

    // Collect both runtime strings and function-name strings.
    set<uint32_t> stringAddresses(stringTable.begin(), stringTable.end());
    for (const FunctionDef& function : functions) {
        stringAddresses.insert(function.description);
    }

    map<uint32_t, string> stringDefs;
    map<uint32_t, uint32_t> stringEnds;

    // Read every referenced string through its NUL terminator.
    for (auto stringAt = stringAddresses.begin(); stringAt != stringAddresses.end(); ++stringAt) {
        const uint32_t addr = *stringAt;
        if (addr < codeStart || addr >= magicTableAddr) {
            std::cerr << "Error: Invalid string address 0x" << std::hex
                      << std::uppercase << addr << ".\n";
            return 1;
        }

        const auto nextString = std::next(stringAt);
        const uint32_t recordEnd = nextString == stringAddresses.end()
                                 ? magicTableAddr : *nextString;
        uint32_t terminator = addr;
        while (terminator < recordEnd && input[terminator] != 0) {
            ++terminator;
        }
        if (terminator == recordEnd) {
            std::cerr << "Error: String at 0x" << std::hex << std::uppercase << addr
                      << " is not NUL-terminated before the next string record.\n";
            return 1;
        }

        // Include conventional zero padding only as far as the next absolute
        // DWORD boundary. Everything else remains available for code or data.
        uint32_t stringEnd = terminator + 1;
        const uint64_t alignedEnd = (static_cast<uint64_t>(stringEnd) + 3) & ~uint64_t(3);
        if (alignedEnd <= recordEnd
                && std::all_of(input.begin() + stringEnd,
                               input.begin() + static_cast<uint32_t>(alignedEnd),
                               [](unsigned char byte) { return byte == 0; })) {
            stringEnd = static_cast<uint32_t>(alignedEnd);
        }

        stringDefs[addr] = string(input.begin() + addr, input.begin() + terminator);
        stringEnds[addr] = stringEnd;
    }

    // Discover code by following control flow from every function entry.
    map<uint32_t, Line> lines;
    set<uint32_t> branchTargets;
    set<uint32_t> attemptedAddresses;
    vector<uint32_t> pendingAddresses;
    pendingAddresses.reserve(functions.size());
    for (const FunctionDef& function : functions) {
        pendingAddresses.push_back(function.addr);
    }

    uint32_t unknownDwordCount = 0;
    while (!pendingAddresses.empty()) {
        const uint32_t addr = pendingAddresses.back();
        pendingAddresses.pop_back();
        if (!attemptedAddresses.insert(addr).second) {
            continue;
        }

        if (addr < codeStart || addr % 4 != 0 || addr > magicTableAddr - 4) {
            continue;
        }

        const auto stringAfter = stringEnds.upper_bound(addr);
        if (stringAfter != stringEnds.begin()) {
            const auto stringBefore = std::prev(stringAfter);
            if (addr < stringBefore->second) {
                continue;
            }
        }

        Line line;
        line.instruction = readUint32(input, addr);
        if (line.instruction >= INST_COUNT) {
            ++unknownDwordCount;
            continue;
        }

        line.operandCount = instList[line.instruction].operandCount;
        const uint32_t lineSize = 4 * static_cast<uint32_t>(line.operandCount + 1);
        const uint64_t lineEnd = static_cast<uint64_t>(addr) + lineSize;

        // Do not let an instruction consume a referenced string.
        const auto nextString = stringAddresses.lower_bound(addr);
        if (lineEnd > magicTableAddr
                || (nextString != stringAddresses.end() && lineEnd > *nextString)) {
            continue;
        }

        // Ignore a target that lands in another decoded instruction.
        const auto nextLine = lines.lower_bound(addr);
        if (nextLine != lines.end() && nextLine->first < lineEnd) {
            continue;
        }
        if (nextLine != lines.begin()) {
            const auto previousLine = std::prev(nextLine);
            const uint32_t previousEnd = previousLine->first
                + 4 * static_cast<uint32_t>(previousLine->second.operandCount + 1);
            if (addr < previousEnd) {
                continue;
            }
        }

        if (line.operandCount >= 1) {
            line.operand = static_cast<int32_t>(readUint32(input, addr + 4));
        }
        if (line.operandCount == 2) {
            line.operand2 = static_cast<int32_t>(readUint32(input, addr + 8));
        }

        lines[addr] = line;

        // Queue direct call and branch destinations.
        if (line.instruction == 0x04 || line.instruction == 0x3B
                || line.instruction == 0x3C || line.instruction == 0x3D) {
            const uint32_t target = static_cast<uint32_t>(line.operand);
            branchTargets.insert(target);
            pendingAddresses.push_back(target);
        } else if (line.instruction == 0x40) {
            const uint32_t target = static_cast<uint32_t>(line.operand2);
            branchTargets.insert(target);
            pendingAddresses.push_back(target);
        }

        // Returns and an unconditional jump do not fall through.
        if (line.instruction != 0x01 && line.instruction != 0x02
                && line.instruction != 0x3B) {
            pendingAddresses.push_back(static_cast<uint32_t>(lineEnd));
        }
    }

    // Create labels for decoded branch targets.
    for (uint32_t target : branchTargets) {
        auto line = lines.find(target);
        if (line != lines.end()) {
            std::ostringstream name;
            name << "LOC_" << std::hex << std::uppercase << target;
            line->second.label = name.str();
        }
    }

    // Name every function entry point.
    set<uint32_t> functionEntries;
    for (uint32_t i = 0; i < functions.size(); ++i) {
        const FunctionDef& function = functions[i];
        auto line = lines.find(function.addr);
        const auto name = stringDefs.find(function.description);
        if (line == lines.end() || name == stringDefs.end()) {
            std::cerr << "Error: Function record " << i
                      << " does not point to valid code and name data.\n";
            return 1;
        }
        line->second.label = name->second.empty()
                           ? "FUNCTION_" + std::to_string(i) : name->second;
        functionEntries.insert(function.addr);
    }

    // Create labels for referenced strings.
    map<uint32_t, string> dataLabels;
    for (uint32_t i = 0; i < stringTable.size(); ++i) {
        const uint32_t stringAddr = stringTable[i];
        if (dataLabels.find(stringAddr) == dataLabels.end()) {
            dataLabels[stringAddr] = "STRING_" + std::to_string(i);
        }
    }
    for (uint32_t i = 0; i < functions.size(); ++i) {
        if (dataLabels.find(functions[i].description) == dataLabels.end()) {
            dataLabels[functions[i].description] = "MAGICNAME_" + std::to_string(i);
        }
    }

    // Make every label unique for the case-insensitive assembler.
    map<string, uint32_t> usedLabels;
    auto makeLabelUnique = [&](string& label, uint32_t labelAddr) {
        string upper = label;
        std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        });
        if (usedLabels.emplace(upper, labelAddr).second) {
            return;
        }

        const string original = label;
        unsigned suffix = 1;
        do {
            label = original + "_" + std::to_string(suffix++);
            upper = label;
            std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c) {
                return static_cast<char>(std::toupper(c));
            });
        } while (!usedLabels.emplace(upper, labelAddr).second);

        std::cout << "Warning: Renamed duplicate label " << original << " at 0x"
                  << std::hex << std::uppercase << labelAddr << " to " << label
                  << std::dec << ".\n";
    };

    for (auto& entry : lines) {
        if (!entry.second.label.empty()) {
            makeLabelUnique(entry.second.label, entry.first);
        }
    }
    for (auto& entry : dataLabels) {
        makeLabelUnique(entry.second, entry.first);
    }

    vector<string> stringTableLabels;
    stringTableLabels.reserve(stringTable.size());
    for (uint32_t stringAddr : stringTable) {
        stringTableLabels.push_back(dataLabels[stringAddr]);
    }

    // Load optional names for instruction comments.
    FixedIdentifierLoader loader;
    loader.loadIntvTable();
    loader.loadSyscallTable();

    // Open the output assembly file.
    string outputPath = argv[1];
    const string::size_type slash = outputPath.find_last_of("/\\");
    const string::size_type dot = outputPath.find_last_of('.');
    if (dot == string::npos || (slash != string::npos && dot < slash)) {
        outputPath += ".asm";
    } else {
        outputPath.replace(dot, string::npos, ".asm");
    }

    std::ofstream output(outputPath);
    if (!output) {
        std::cerr << "Error: Unable to create " << outputPath << ".\n";
        return 1;
    }

    // Output the assembler helper macros.
    output << "; === Assembly generated by Sango Script Disassembler v2 ===\n"
R"(procedure macro addr, initstack, description, sequence
    DD addr
    DD initstack
    DD description
    DD sequence
endm

GETUSERVALUE macro index
    DD 5BH
    DD index
endm

SETUSERVALUE macro index
    DD 5AH
    DD index
endm

magic_table_count  equ (magic_table_end - magic_table_addr) / 0x10
string_table_count equ (string_table_end - string_table_addr) / 0x04
)";
    // Output the header.
    output << "global_vars_count   equ " << globalVarsCount << "\n\n"
           << "org 00h\n"
           << "\tDD \"Smart Script\"\t; 12-byte signature\n"
           << "org 0Ch\n";
    writeRawData(output, input, 0x0C, 0x10);

    output << "org 10h\n"
           << "\tDD 0x" << std::hex << std::uppercase << version << "\t; format version\n"
           << "\tDD 0x" << codeStart << "\t; code/data start\n"
           << "\tDD magic_table_addr\n"
           << "\tDD magic_table_count\n"
           << "\tDD string_table_addr\n"
           << "\tDD string_table_count\n"
           << "\tDD global_vars_count\n"
           << "\tDD file_end\t; loader-visible file length\n\n"
           << "org 30h\n";
    writeRawData(output, input, 0x30, codeStart);
    output << "\norg " << std::hex << std::uppercase << codeStart << "h\n";
    output << std::dec << std::nouppercase;

    // Output decoded instructions, referenced strings, and unclassified data.
    uint32_t outputAddr = codeStart;
    while (outputAddr < magicTableAddr) {
        const auto stringAt = stringDefs.find(outputAddr);
        if (stringAt != stringDefs.end()) {
            output << "\n" << dataLabels[outputAddr] << ":\n";
            writeRawData(output, input, outputAddr, stringEnds[outputAddr]);
            output << "\t; \"" << escapeCommentString(stringAt->second) << "\"\n";
            outputAddr = stringEnds[outputAddr];
            continue;
        }

        const auto lineAt = lines.find(outputAddr);
        if (lineAt == lines.end()) {
            uint32_t rawEnd = magicTableAddr;
            const auto nextLine = lines.lower_bound(outputAddr);
            const auto nextString = stringDefs.lower_bound(outputAddr);
            if (nextLine != lines.end()) {
                rawEnd = std::min(rawEnd, nextLine->first);
            }
            if (nextString != stringDefs.end()) {
                rawEnd = std::min(rawEnd, nextString->first);
            }
            writeRawData(output, input, outputAddr, rawEnd);
            outputAddr = rawEnd;
            continue;
        }

        const Line& line = lineAt->second;
        if (!line.label.empty()) {
            output << (functionEntries.count(outputAddr) ? "\n" : "")
                   << line.label << ":\n";
        }
        output << '\t';

        output << instList[line.instruction].symbol;
        if (line.operandCount >= 1) {
            output << ' ';
            if (line.instruction == 0x04 || line.instruction == 0x3B
                    || line.instruction == 0x3C || line.instruction == 0x3D) {
                const auto target = lines.find(static_cast<uint32_t>(line.operand));
                if (target != lines.end() && !target->second.label.empty()) {
                    output << target->second.label;
                } else {
                    output << "0x" << std::hex << std::uppercase
                           << static_cast<uint32_t>(line.operand) << std::dec;
                }
            } else if (line.instruction == 0x05) {
                output << "0x" << std::hex << std::uppercase
                       << static_cast<uint32_t>(line.operand) << std::dec;
            } else {
                output << line.operand;
            }
        }
        if (line.operandCount == 2) {
            output << ", ";
            if (line.instruction == 0x05) {
                // Split the packed SYSCALL metadata into its two 16-bit fields.
                const uint32_t packed = static_cast<uint32_t>(line.operand2);
                output << '(' << (packed & 0xFFFF) << " | ("
                       << (packed >> 16) << " << 16))";
            } else if (line.instruction == 0x40) {
                const auto target = lines.find(static_cast<uint32_t>(line.operand2));
                if (target != lines.end() && !target->second.label.empty()) {
                    output << target->second.label;
                } else {
                    output << "0x" << std::hex << std::uppercase
                           << static_cast<uint32_t>(line.operand2) << std::dec;
                }
            } else {
                output << line.operand2;
            }
        }

        // Add release-specific names without changing the numeric operands.
        const bool intvInstruction = line.instruction == 0x0A || line.instruction == 0x0D
                                  || line.instruction == 0x1F || line.instruction == 0x22
                                  || line.instruction == 0x50 || line.instruction == 0x53
                                  || line.instruction == 0x56 || line.instruction == 0x59;
        if (intvInstruction) {
            const auto intv = loader.intvs.find(line.operand);
            output << " ; " << (intv == loader.intvs.end()
                                  ? "intv_" + std::to_string(line.operand)
                                  : intv->second.name);
        } else if (line.instruction == 0x05) {
            const auto syscall = loader.syscalls.find(static_cast<uint32_t>(line.operand));
            if (syscall != loader.syscalls.end()) {
                output << " ; " << syscall->second.name;
            }
        } else if (line.instruction == 0x07 && line.operand >= 0
                && static_cast<uint32_t>(line.operand) < stringTable.size()) {
            output << " ; \""
                   << escapeCommentString(stringDefs[
                          stringTable[static_cast<uint32_t>(line.operand)]]) << '"';
        }
        output << "\n";

        outputAddr += 4 * static_cast<uint32_t>(line.operandCount + 1);
    }

    // Output the function and string tables.
    output << "\nmagic_table_addr:\n";
    for (uint32_t i = 0; i < functions.size(); ++i) {
        const FunctionDef& function = functions[i];
        output << "MAGIC_" << i << ": procedure " << lines[function.addr].label
               << ' ' << function.initStack << ' ' << dataLabels[function.description]
               << ' ' << function.sequence << "\n";
    }
    output << "magic_table_end:\n";
    writeRawData(output, input, static_cast<uint32_t>(magicTableEnd), stringTableAddr);
    output << "\nstring_table_addr:\n";
    for (const string& label : stringTableLabels) {
        output << "\tDD " << label << "\n";
    }
    output << "string_table_end:\n";

    // Output the logical end and any physical trailer.
    writeRawData(output, input, static_cast<uint32_t>(stringTableEnd), fileEndAddr);
    output << "file_end:\n";
    if (fileEndAddr < input.size()) {
        output << "; Bytes below are provenance/trailer data outside the loader-visible length.\n";
        writeRawData(output, input, fileEndAddr, static_cast<uint32_t>(input.size()));
    }

    output.close();
    if (unknownDwordCount != 0) {
        std::cout << "Warning: Preserved " << unknownDwordCount
                  << " unknown DWORD(s) as raw data.\n";
    }
    std::cout << "Sango Script Disassembler v2\n"
              << "Wrote " << outputPath << ".\n";
    return 0;
}
