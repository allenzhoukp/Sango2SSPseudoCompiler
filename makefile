CXX ?= g++

CC_SOURCES = cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp cc/Panic.cpp cc/Config.cpp

.PHONY: all cc cc-raw dasm decomp test

all: cc dasm decomp test

cc-raw: $(CC_SOURCES)
	$(CXX) -std=c++11 -O0 -g $(CC_SOURCES) -o ss-debug.exe -static

cc: $(CC_SOURCES)
	$(CXX) -std=c++11 -O2 -DNDEBUG $(CC_SOURCES) -o ss.exe -static

dasm: cc/Sg2dasm2.cpp 
	$(CXX) -std=c++11 -O2 -DNDEBUG cc/Sg2dasm2.cpp -o sg2dasm2.exe -static

DECOMPILER_SOURCES = cc/Sg2decomp.cpp cc/decompiler/InstructionSet.cpp cc/decompiler/ObjectDecoder.cpp cc/decompiler/ControlFlowGraph.cpp cc/decompiler/TypeAnalyzer.cpp cc/decompiler/StackLifter.cpp cc/decompiler/PatternStructurer.cpp cc/decompiler/SangoEmitter.cpp

decomp: $(DECOMPILER_SOURCES)
	$(CXX) -std=c++11 -O2 -DNDEBUG $(DECOMPILER_SOURCES) -o sg2decomp.exe -static

test: cc
	./ss.exe SS-Tutorial.cpp -a