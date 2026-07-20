CXX ?= g++

CC_SOURCES = cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp cc/Panic.cpp cc/Config.cpp

.PHONY: all cc cc-release test dasm

all: cc test

cc: $(CC_SOURCES)
	$(CXX) -std=c++11 -O0 -g $(CC_SOURCES) -o ss.exe -static

cc-release: $(CC_SOURCES)
	$(CXX) -std=c++11 -O2 -DNDEBUG $(CC_SOURCES) -o ss-release.exe -static

test: Test.cpp
	ss Test.cpp -a

dasm: cc/Sg2dasm2.cpp 
	$(CXX) -std=c++11 cc/Sg2dasm2.cpp -o sg2dasm2.exe -g -static
