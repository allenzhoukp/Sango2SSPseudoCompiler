#include "Decompiler.h"

#include <cctype>
#include <iomanip>
#include <sstream>

namespace sango {
namespace decompiler {

namespace {

const InstructionDescription descriptions[] = {
    {"NOP", 0}, {"RETVAL", 1}, {"RETN", 1}, {"STACK", 1},
    {"CALL", 1}, {"SYSCALL", 2}, {"PUSH", 1}, {"PUSHSTR", 1},
    {"PUSHARG", 1}, {"PUSHGLB", 1}, {"PUSHINV", 1}, {"POPN", 1},
    {"SETGLB", 1}, {"SETINV", 1}, {"POP", 0}, {"ADD", 0},
    {"ADDF", 0}, {"ADDS", 0}, {"SUB", 0}, {"SUBF", 0},
    {"MUL", 0}, {"MULF", 0}, {"DIV", 0}, {"DIVF", 0},
    {"MOD", 0}, {"NOP19", 0}, {"NEG", 0}, {"NEGF", 0},
    {"NOT", 0}, {"INCN", 1}, {"INCGLB", 1}, {"INCINV", 1},
    {"DECN", 1}, {"DECGLB", 1}, {"DECINV", 1}, {"AND", 0},
    {"OR", 0}, {"XOR", 0}, {"CMP", 0}, {"CMPF", 0},
    {"CMPS", 0}, {"CMPZ", 0}, {"CMPZF", 0}, {"CMPZS", 0},
    {"CMPG", 0}, {"CMPGF", 0}, {"CMPGS", 0}, {"CMPGE", 0},
    {"CMPGEF", 0}, {"CMPGEFS", 0}, {"CMPL", 0}, {"CMPLF", 0},
    {"CMPLS", 0}, {"CMPLE", 0}, {"CMPLEF", 0}, {"CMPLEFS", 0},
    {"LNOT", 0}, {"LAND", 0}, {"LOR", 0}, {"JMP", 1},
    {"JNZ", 1}, {"JZ", 1}, {"SHL", 0}, {"SAR", 0},
    {"JCOND", 2}, {"NOP41", 0}, {"DELAY", 0}, {"NOP43", 0},
    {"NOP44", 0}, {"WAIT", 0}, {"ISRUNNING", 0}, {"CALLBS", 0},
    {"LTOF", 0}, {"LTOA", 0}, {"FTOL", 0}, {"GCVT", 0},
    {"REPLZERO", 0}, {"INST_4D", 0}, {"PUSHNR", 1}, {"PUSHGLBR", 1},
    {"PUSHINVR", 1}, {"SETNR", 1}, {"SETGLBR", 1}, {"SETINVR", 1},
    {"INCNR", 1}, {"INCGLBR", 1}, {"INCINVR", 1}, {"DECNR", 1},
    {"DECGLBR", 1}, {"DECINVR", 1}, {"SETUSERVALUE", 1}, {"GETUSERVALUE", 1}
};

/** Returns true when a byte may continue a source identifier. */
bool isIdentifierByte(unsigned char byte) {
    return std::isalnum(byte) != 0 || byte == '_';
}

}  // namespace

std::uint32_t Instruction::nextAddress() const {
    return address + 4U * static_cast<std::uint32_t>(operandCount + 1);
}

bool FunctionGraph::isComplete() const {
    return warnings.empty() && !blocks.empty();
}

bool Expression::sameAs(const Expression& other) const {
    return text == other.text && type == other.type
        && hasSideEffect == other.hasSideEffect;
}

const InstructionDescription* instructionDescription(std::uint32_t opcode) {
    const std::size_t count = sizeof(descriptions) / sizeof(descriptions[0]);
    return opcode < count ? &descriptions[opcode] : NULL;
}

const char* valueTypeName(ValueType type) {
    switch (type) {
        case ValueType::integer: return "integer";
        case ValueType::floatingPoint: return "floating point";
        case ValueType::string: return "string";
        case ValueType::conflict: return "conflict";
        default: return "unknown";
    }
}

const char* sourceTypeName(ValueType type) {
    if (type == ValueType::floatingPoint) {
        return "float";
    }
    if (type == ValueType::string) {
        return "string";
    }
    return "int";
}

std::string quoteSourceString(const std::string& bytes) {
    std::ostringstream output;
    output << '"';
    for (std::size_t index = 0; index < bytes.size(); ++index) {
        const unsigned char byte = static_cast<unsigned char>(bytes[index]);
        switch (byte) {
            case '\\': output << "\\\\"; break;
            case '"': output << "\\\""; break;
            case '\n': output << "\\n"; break;
            case '\r': output << "\\r"; break;
            case '\t': output << "\\t"; break;
            default:
                if (byte >= 0x20 && byte <= 0x7e) {
                    output << static_cast<char>(byte);
                } else {
                    output << "\\x" << std::hex << std::uppercase
                           << std::setw(2) << std::setfill('0')
                           << static_cast<unsigned int>(byte)
                           << std::dec << std::nouppercase << std::setfill(' ');
                }
        }
    }
    output << '"';
    return output.str();
}

std::string sourceIdentifier(const std::string& name, int fallbackIndex) {
    std::string result;
    for (std::size_t index = 0; index < name.size(); ++index) {
        const unsigned char byte = static_cast<unsigned char>(name[index]);
        if ((index == 0 && (std::isalpha(byte) != 0 || byte == '_'))
                || (index != 0 && isIdentifierByte(byte))) {
            result += static_cast<char>(byte);
        } else if (index != 0) {
            result += '_';
        }
    }
    if (result.empty()) {
        result = "function_" + std::to_string(fallbackIndex);
    }
    return result;
}

}  // namespace decompiler
}  // namespace sango
