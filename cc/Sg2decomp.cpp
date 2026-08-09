#include "decompiler/Decompiler.h"

#include <iostream>
#include <string>
#include <vector>

using sango::decompiler::ConfigurationNames;
using sango::decompiler::ControlFlowGraphBuilder;
using sango::decompiler::DecompilationReport;
using sango::decompiler::FunctionGraph;
using sango::decompiler::LiftedFunction;
using sango::decompiler::Module;
using sango::decompiler::ObjectDecoder;
using sango::decompiler::PatternStructurer;
using sango::decompiler::SangoEmitter;
using sango::decompiler::StackLifter;
using sango::decompiler::TypeAnalyzer;

namespace {

/** Stores validated command-line choices for one decompiler run. */
struct Options {
    std::string inputPath;
    std::string outputPath;
    std::string reportPath;
    std::string mode = "structured";
};

/** Replaces the last filename extension or appends a suffix when none exists. */
std::string replaceExtension(const std::string& path,
                             const std::string& extension) {
    const std::string::size_type slash = path.find_last_of("/\\");
    const std::string::size_type dot = path.find_last_of('.');
    if (dot == std::string::npos
            || (slash != std::string::npos && dot < slash)) {
        return path + extension;
    }
    return path.substr(0, dot) + extension;
}

/** Prints the concise command-line contract. */
void printUsage() {
    std::cout
        << "Usage: sg2decomp.exe input.so [-o output.cpp] [--report output.json]\n"
        << "                         [--mode structured|goto|lossless]\n";
}

/** Parses command-line arguments without silently accepting misspellings. */
bool parseOptions(int argumentCount, char** arguments,
                  Options& options, std::string& error) {
    if (argumentCount < 2) {
        error = "An input object is required.";
        return false;
    }
    options.inputPath = arguments[1];
    for (int index = 2; index < argumentCount; ++index) {
        const std::string argument = arguments[index];
        if ((argument == "-o" || argument == "--report" || argument == "--mode")
                && index + 1 >= argumentCount) {
            error = "Option " + argument + " requires a value.";
            return false;
        }
        if (argument == "-o") {
            options.outputPath = arguments[++index];
        } else if (argument == "--report") {
            options.reportPath = arguments[++index];
        } else if (argument == "--mode") {
            options.mode = arguments[++index];
        } else if (argument.compare(0, 7, "--mode=") == 0) {
            options.mode = argument.substr(7);
        } else {
            error = "Unknown option " + argument + ".";
            return false;
        }
    }
    if (options.mode != "structured" && options.mode != "goto"
            && options.mode != "lossless") {
        error = "Mode must be structured, goto, or lossless.";
        return false;
    }
    if (options.outputPath.empty()) {
        options.outputPath = replaceExtension(options.inputPath, ".decompiled.cpp");
    }
    if (options.reportPath.empty()) {
        options.reportPath = replaceExtension(options.outputPath, ".report.json");
    }
    return true;
}

}  // namespace

/** Runs the object decoder, lifter, structurer, source emitter, and report writer. */
int main(int argumentCount, char** arguments) {
    if (argumentCount == 2
            && (std::string(arguments[1]) == "--help"
                || std::string(arguments[1]) == "-h")) {
        printUsage();
        return 0;
    }
    Options options;
    std::string error;
    if (!parseOptions(argumentCount, arguments, options, error)) {
        std::cerr << "Error: " << error << '\n';
        printUsage();
        return 1;
    }

    Module module;
    ObjectDecoder decoder;
    if (!decoder.decode(options.inputPath, module, error)) {
        std::cerr << "Error: " << error << '\n';
        return 1;
    }

    ConfigurationNames names;
    names.load("config");
    module.warnings.insert(module.warnings.end(),
                           names.warnings.begin(), names.warnings.end());

    ControlFlowGraphBuilder graphBuilder;
    std::vector<FunctionGraph> graphs;
    graphs.reserve(module.functions.size());
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        graphs.push_back(graphBuilder.build(module, static_cast<int>(index)));
    }

    TypeAnalyzer types;
    types.analyze(module, graphs, names);
    StackLifter lifter;
    PatternStructurer structurer;
    std::vector<LiftedFunction> functions;
    functions.reserve(module.functions.size());
    for (std::size_t index = 0; index < module.functions.size(); ++index) {
        LiftedFunction function = lifter.lift(module, graphs[index], names, types);
        if (options.mode == "structured" && function.succeeded) {
            structurer.structure(graphs[index], function);
        }
        functions.push_back(function);
    }

    DecompilationReport report;
    SangoEmitter emitter;
    if (!emitter.emit(module, graphs, functions, names, types, options.mode,
                      options.outputPath, report, error)) {
        std::cerr << "Error: " << error << '\n';
        return 1;
    }
    if (!report.write(options.reportPath, error)) {
        std::cerr << "Error: " << error << '\n';
        return 1;
    }

    std::cout << "Sango Smart Script Decompiler\n"
              << "Wrote " << options.outputPath << " and "
              << options.reportPath << ".\n"
              << "Structured " << report.structuredFunctionCount << " of "
              << module.functions.size() << " functions; "
              << report.residualGotoCount << " residual goto(s).\n";
    return 0;
}
