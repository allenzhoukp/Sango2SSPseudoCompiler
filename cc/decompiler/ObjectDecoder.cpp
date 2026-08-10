#include "Decompiler.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <limits>
#include <sstream>
#include <set>
#include <tuple>

namespace sango {
namespace decompiler {

namespace {

/** Reads one little-endian 32-bit word from a previously checked offset. */
std::uint32_t readWord(const std::vector<unsigned char>& bytes,
                       std::uint32_t offset) {
    return static_cast<std::uint32_t>(bytes[offset])
        | (static_cast<std::uint32_t>(bytes[offset + 1]) << 8)
        | (static_cast<std::uint32_t>(bytes[offset + 2]) << 16)
        | (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

/** Returns true when adding a counted table stays within the given limit. */
bool tableFits(std::uint32_t address, std::uint32_t count,
               std::uint32_t recordSize, std::size_t limit) {
    const std::uint64_t end = static_cast<std::uint64_t>(address)
        + static_cast<std::uint64_t>(count) * recordSize;
    return end <= limit;
}

/** Returns true when an address falls inside a decoded string record. */
bool isInsideString(const Module& module, std::uint32_t address) {
    std::map<std::uint32_t, ObjectString>::const_iterator after
        = module.strings.upper_bound(address);
    if (after == module.strings.begin()) {
        return false;
    }
    --after;
    return address < after->second.endAddress;
}

/** Adds a warning only once so large malformed inputs remain readable. */
void addUniqueWarning(Module& module, const std::string& warning) {
    if (std::find(module.warnings.begin(), module.warnings.end(), warning)
            == module.warnings.end()) {
        module.warnings.push_back(warning);
    }
}

}  // namespace

void ConfigurationNames::load(const std::string& configurationDirectory) {
    systemCalls.clear();
    internalValues.clear();
    warnings.clear();
    hasExtendedAsyncCalls = false;
    asyncSystemCallBase = 0;
    directAsyncParameterLimit = 4;

    const std::string configurationPath
        = configurationDirectory + "/CONFIG.txt";
    std::ifstream configurationInput(configurationPath.c_str());
    if (!configurationInput) {
        warnings.push_back("Unable to read " + configurationPath + ".");
    } else {
        bool hasAsyncBase = false;
        std::string line;
        while (std::getline(configurationInput, line)) {
            const std::string::size_type comment = line.find(';');
            if (comment != std::string::npos) {
                line.erase(comment);
            }
            const std::string::size_type equals = line.find('=');
            if (equals == std::string::npos) {
                continue;
            }
            std::string key = line.substr(0, equals);
            std::string value = line.substr(equals + 1);
            const std::string whitespace = " \t\r\n";
            const std::string::size_type keyStart = key.find_first_not_of(whitespace);
            const std::string::size_type keyEnd = key.find_last_not_of(whitespace);
            const std::string::size_type valueStart
                = value.find_first_not_of(whitespace);
            const std::string::size_type valueEnd
                = value.find_last_not_of(whitespace);
            if (keyStart == std::string::npos || valueStart == std::string::npos) {
                continue;
            }
            key = key.substr(keyStart, keyEnd - keyStart + 1);
            value = value.substr(valueStart, valueEnd - valueStart + 1);
            char* end = NULL;
            if (key == "asynccall_sys") {
                const unsigned long parsed = std::strtoul(value.c_str(), &end, 0);
                if (end != value.c_str() && *end == '\0') {
                    asyncSystemCallBase = static_cast<std::uint32_t>(parsed);
                    hasAsyncBase = true;
                }
            } else if (key == "callbs_max_params") {
                const long parsed = std::strtol(value.c_str(), &end, 10);
                if (end != value.c_str() && *end == '\0' && parsed >= 0) {
                    directAsyncParameterLimit = static_cast<int>(parsed);
                }
            }
        }
        hasExtendedAsyncCalls = hasAsyncBase;
    }

    const std::string systemCallPath
        = configurationDirectory + "/SYSCALL_TABLE.txt";
    std::ifstream systemCallInput(systemCallPath.c_str());
    if (!systemCallInput) {
        warnings.push_back("Unable to read " + systemCallPath + ".");
    } else {
        std::uint32_t number = 0;
        int parameterCount = 0;
        int returnCode = 0;
        std::string name;
        while (systemCallInput >> std::hex >> number >> std::dec
                >> parameterCount >> returnCode >> name) {
            SystemCall call;
            call.number = number;
            call.parameterCount = parameterCount;
            call.returnCode = returnCode;
            call.name = name;
            call.returnType = returnCode == 0 ? ValueType::unknown
                : returnCode == 2 ? ValueType::floatingPoint
                : returnCode == 3 ? ValueType::string
                : ValueType::integer;
            systemCalls[number].push_back(call);
        }
    }

    const std::string internalValuePath
        = configurationDirectory + "/INTV_TABLE.txt";
    std::ifstream internalValueInput(internalValuePath.c_str());
    if (!internalValueInput) {
        warnings.push_back("Unable to read " + internalValuePath + ".");
    } else {
        std::string name;
        std::uint32_t number = 0;
        while (internalValueInput >> name >> std::hex >> number >> std::dec) {
            internalValues[static_cast<std::int32_t>(number)] = name;
        }
    }
}

const SystemCall* ConfigurationNames::findSystemCall(
        std::uint32_t number, int parameterCount, int returnCode) const {
    std::unordered_map<std::uint32_t, std::vector<SystemCall> >::const_iterator
        found = systemCalls.find(number);
    if (found == systemCalls.end()) {
        return NULL;
    }
    for (std::size_t index = 0; index < found->second.size(); ++index) {
        const SystemCall& call = found->second[index];
        if (call.parameterCount == parameterCount
                && call.returnCode == returnCode) {
            return &call;
        }
    }
    return NULL;
}

bool ConfigurationNames::isExtendedAsyncCall(
        std::uint32_t number, int parameterCount, int returnCode) const {
    if (!hasExtendedAsyncCalls || returnCode != 0
            || number < asyncSystemCallBase) {
        return false;
    }
    const std::uint64_t expectedParameterCount
        = static_cast<std::uint64_t>(number - asyncSystemCallBase)
        + static_cast<std::uint64_t>(directAsyncParameterLimit) + 2U;
    return expectedParameterCount == static_cast<std::uint64_t>(parameterCount);
}

std::vector<SystemCall> ConfigurationNames::addMissingSystemCalls(
        const Module& module) {
    typedef std::tuple<std::uint32_t, int, int> Signature;
    std::set<Signature> missing;
    std::set<std::string> usedNames;
    for (std::unordered_map<std::uint32_t, std::vector<SystemCall> >::const_iterator
            group = systemCalls.begin(); group != systemCalls.end(); ++group) {
        for (std::size_t index = 0; index < group->second.size(); ++index) {
            usedNames.insert(group->second[index].name);
        }
    }
    for (std::map<std::uint32_t, Instruction>::const_iterator
            decoded = module.instructions.begin();
            decoded != module.instructions.end(); ++decoded) {
        const Instruction& instruction = decoded->second;
        if (instruction.opcode != 0x05) {
            continue;
        }
        const std::uint32_t number
            = static_cast<std::uint32_t>(instruction.operand);
        const std::uint32_t packed
            = static_cast<std::uint32_t>(instruction.secondOperand);
        const int parameterCount = static_cast<int>(packed & 0xffffU);
        const int returnCode = static_cast<int>(packed >> 16);
        if (!isExtendedAsyncCall(number, parameterCount, returnCode)
                && findSystemCall(number, parameterCount, returnCode) == NULL) {
            missing.insert(Signature(number, parameterCount, returnCode));
        }
    }

    std::vector<SystemCall> generated;
    for (std::set<Signature>::const_iterator signature = missing.begin();
            signature != missing.end(); ++signature) {
        SystemCall call;
        call.number = std::get<0>(*signature);
        call.parameterCount = std::get<1>(*signature);
        call.returnCode = std::get<2>(*signature);
        call.returnType = call.returnCode == 0 ? ValueType::unknown
            : call.returnCode == 2 ? ValueType::floatingPoint
            : call.returnCode == 3 ? ValueType::string
            : ValueType::integer;
        std::ostringstream name;
        name << "DECOMP_SYSCALL_0x" << std::hex << std::uppercase
             << call.number << std::dec << "_ARGS" << call.parameterCount
             << "_RET" << call.returnCode;
        call.name = name.str();
        int suffix = 2;
        while (usedNames.find(call.name) != usedNames.end()) {
            std::ostringstream uniqueName;
            uniqueName << name.str() << '_' << suffix++;
            call.name = uniqueName.str();
        }
        usedNames.insert(call.name);
        systemCalls[call.number].push_back(call);
        generated.push_back(call);
    }
    return generated;
}

bool ObjectDecoder::decode(const std::string& path, Module& module,
                           std::string& error) const {
    module = Module();
    module.inputPath = path;
    error.clear();

    std::ifstream input(path.c_str(), std::ios::binary);
    if (!input) {
        error = "Unable to open " + path + ".";
        return false;
    }
    module.bytes.assign(std::istreambuf_iterator<char>(input),
                        std::istreambuf_iterator<char>());
    if (module.bytes.size() < 0x40
            || !std::equal(module.bytes.begin(), module.bytes.begin() + 12,
                           "Smart Script")) {
        error = "The input is not a Smart Script object file.";
        return false;
    }

    module.version = readWord(module.bytes, 0x10);
    module.codeStart = readWord(module.bytes, 0x14);
    module.functionTableAddress = readWord(module.bytes, 0x18);
    const std::uint32_t functionCount = readWord(module.bytes, 0x1c);
    module.stringTableAddress = readWord(module.bytes, 0x20);
    const std::uint32_t stringCount = readWord(module.bytes, 0x24);
    module.globalCount = readWord(module.bytes, 0x28);
    module.logicalEnd = readWord(module.bytes, 0x2c);

    if (module.codeStart < 0x30
            || module.codeStart > module.functionTableAddress
            || module.functionTableAddress > module.stringTableAddress
            || module.stringTableAddress > module.logicalEnd
            || module.logicalEnd > module.bytes.size()
            || (module.codeStart & 3U) != 0
            || (module.functionTableAddress & 3U) != 0
            || (module.stringTableAddress & 3U) != 0
            || !tableFits(module.functionTableAddress, functionCount, 16,
                          module.stringTableAddress)
            || !tableFits(module.stringTableAddress, stringCount, 4,
                          module.logicalEnd)) {
        error = "The object header contains inconsistent section addresses.";
        return false;
    }

    module.functions.reserve(functionCount);
    for (std::uint32_t index = 0; index < functionCount; ++index) {
        const std::uint32_t record = module.functionTableAddress + index * 16;
        FunctionRecord function;
        function.entryAddress = readWord(module.bytes, record);
        function.parameterCount
            = static_cast<std::int32_t>(readWord(module.bytes, record + 4));
        function.nameAddress = readWord(module.bytes, record + 8);
        function.callSign
            = static_cast<std::int32_t>(readWord(module.bytes, record + 12));
        module.functions.push_back(function);
    }

    module.runtimeStringAddresses.reserve(stringCount);
    std::set<std::uint32_t> stringAddresses;
    for (std::uint32_t index = 0; index < stringCount; ++index) {
        const std::uint32_t address
            = readWord(module.bytes, module.stringTableAddress + index * 4);
        module.runtimeStringAddresses.push_back(address);
        stringAddresses.insert(address);
    }
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        stringAddresses.insert(module.functions[index].nameAddress);
    }

    for (std::set<std::uint32_t>::const_iterator at = stringAddresses.begin();
            at != stringAddresses.end(); ++at) {
        const std::uint32_t address = *at;
        if (address < module.codeStart || address >= module.functionTableAddress) {
            error = "A string pointer falls outside the code and data section.";
            return false;
        }
        std::set<std::uint32_t>::const_iterator next = at;
        ++next;
        const std::uint32_t recordEnd = next == stringAddresses.end()
            ? module.functionTableAddress : *next;
        std::uint32_t terminator = address;
        while (terminator < recordEnd && module.bytes[terminator] != 0) {
            ++terminator;
        }
        if (terminator == recordEnd) {
            error = "A referenced string is not terminated before the next record.";
            return false;
        }

        ObjectString objectString;
        objectString.address = address;
        objectString.bytes.assign(module.bytes.begin() + address,
                                  module.bytes.begin() + terminator);
        objectString.endAddress = terminator + 1;
        const std::uint64_t aligned
            = (static_cast<std::uint64_t>(objectString.endAddress) + 3U) & ~3ULL;
        if (aligned <= recordEnd
                && std::all_of(module.bytes.begin() + objectString.endAddress,
                               module.bytes.begin() + static_cast<std::uint32_t>(aligned),
                               // Accept only conventional zero alignment bytes.
                               [](unsigned char byte) { return byte == 0; })) {
            objectString.endAddress = static_cast<std::uint32_t>(aligned);
        }
        module.strings[address] = objectString;
    }

    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        std::map<std::uint32_t, ObjectString>::const_iterator name
            = module.strings.find(module.functions[index].nameAddress);
        if (name == module.strings.end()) {
            error = "A function name does not point to a decoded string.";
            return false;
        }
        module.functions[index].name = name->second.bytes;
    }

    std::vector<std::uint32_t> pending;
    std::set<std::uint32_t> attempted;
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        pending.push_back(module.functions[index].entryAddress);
    }

    while (!pending.empty()) {
        const std::uint32_t address = pending.back();
        pending.pop_back();
        if (!attempted.insert(address).second) {
            continue;
        }
        if (address < module.codeStart || (address & 3U) != 0
                || address + 4 > module.functionTableAddress
                || isInsideString(module, address)) {
            addUniqueWarning(module,
                "A discovered code address is outside decodable instruction data.");
            continue;
        }

        const std::uint32_t opcode = readWord(module.bytes, address);
        const InstructionDescription* description
            = instructionDescription(opcode);
        if (description == NULL) {
            module.unknownWords.insert(address);
            continue;
        }
        const std::uint32_t size
            = 4U * static_cast<std::uint32_t>(description->operandCount + 1);
        if (address + size > module.functionTableAddress) {
            addUniqueWarning(module,
                "An instruction would extend beyond the code and data section.");
            continue;
        }
        bool overlapsString = false;
        std::map<std::uint32_t, ObjectString>::const_iterator stringAt
            = module.strings.lower_bound(address);
        if (stringAt != module.strings.end()
                && stringAt->first < address + size) {
            overlapsString = true;
        }
        if (overlapsString) {
            addUniqueWarning(module,
                "An instruction would overlap a referenced string.");
            continue;
        }

        std::map<std::uint32_t, Instruction>::const_iterator nextInstruction
            = module.instructions.lower_bound(address);
        if (nextInstruction != module.instructions.end()
                && nextInstruction->first < address + size) {
            addUniqueWarning(module,
                "Two discovered instructions overlap.");
            continue;
        }
        if (nextInstruction != module.instructions.begin()) {
            std::map<std::uint32_t, Instruction>::const_iterator previous
                = nextInstruction;
            --previous;
            if (address < previous->second.nextAddress()) {
                addUniqueWarning(module,
                    "A control-flow target lands inside another instruction.");
                continue;
            }
        }

        Instruction instruction;
        instruction.address = address;
        instruction.opcode = opcode;
        instruction.operandCount = description->operandCount;
        if (description->operandCount >= 1) {
            instruction.operand
                = static_cast<std::int32_t>(readWord(module.bytes, address + 4));
        }
        if (description->operandCount == 2) {
            instruction.secondOperand
                = static_cast<std::int32_t>(readWord(module.bytes, address + 8));
        }
        module.instructions[address] = instruction;

        if (opcode == 0x04 || opcode == 0x3b || opcode == 0x3c
                || opcode == 0x3d) {
            pending.push_back(static_cast<std::uint32_t>(instruction.operand));
        } else if (opcode == 0x40) {
            pending.push_back(static_cast<std::uint32_t>(instruction.secondOperand));
        }
        if (opcode != 0x01 && opcode != 0x02 && opcode != 0x3b) {
            pending.push_back(instruction.nextAddress());
        }
    }

    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        if (module.instructions.find(module.functions[index].entryAddress)
                == module.instructions.end()) {
            error = "A function entry does not point to a decoded instruction.";
            return false;
        }
    }
    return true;
}

}  // namespace decompiler
}  // namespace sango
