all:
	cc
	test

cc: cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp cc/Panic.cpp cc/Config.cpp
	g++ -std=c++11 cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp cc/Panic.cpp cc/Config.cpp -o ss.exe -g -static

test: Test.cpp
	ss Test.cpp -a

dasm: cc/Sg2dasm2.cpp 
	g++ -std=c++11 cc/Sg2dasm2.cpp -o sg2dasm2.exe -g -static
