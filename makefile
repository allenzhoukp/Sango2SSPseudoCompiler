CXX ?= g++

CC_SOURCES = cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp cc/Panic.cpp cc/Config.cpp

.PHONY: all cc cc-release test dasm decomp test-decomp

all: cc test

cc: $(CC_SOURCES)
	$(CXX) -std=c++11 -O0 -g $(CC_SOURCES) -o ss-debug.exe -static

cc-release: $(CC_SOURCES)
	$(CXX) -std=c++11 -O2 -DNDEBUG $(CC_SOURCES) -o ss.exe -static

test: Test.cpp
	ss Test.cpp -a

dasm: cc/Sg2dasm2.cpp 
	$(CXX) -std=c++11 cc/Sg2dasm2.cpp -o sg2dasm2.exe -g -static

DECOMPILER_SOURCES = cc/Sg2decomp.cpp cc/decompiler/InstructionSet.cpp cc/decompiler/ObjectDecoder.cpp cc/decompiler/ControlFlowGraph.cpp cc/decompiler/TypeAnalyzer.cpp cc/decompiler/StackLifter.cpp cc/decompiler/PatternStructurer.cpp cc/decompiler/SangoEmitter.cpp

decomp: $(DECOMPILER_SOURCES)
	$(CXX) -std=c++11 -O2 -DNDEBUG $(DECOMPILER_SOURCES) -o sg2decomp.exe -static

# Runs the full decompiler test suite: interface round trips, the structural
# regression tests (float typing, switch fallthrough, mid-function CALL), and
# the memory-operation audit. Requires sg2decomp.exe, ss.exe and sg2asm.exe.
test-decomp: decomp
	./tests/decompiler/roundtrip-windows-interop.sh
	python3 tests/decompiler/float-constant-typing.py
	python3 tests/decompiler/switch-fallthrough.py
	python3 tests/decompiler/mid-function-call.py
	python3 tests/decompiler/audit-memory-operations.py --require-exact-counts
