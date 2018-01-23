all:
	ss
	Testcc

ss: cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp cc/Panic.cpp
	g++ -std=c++11 cc/Main.cpp cc/Lexer.cpp cc/Parser.cpp cc/FixIdentifierLoader.cpp cc/TreeConstruction.cpp cc/TreeAnalysis.cpp cc/BranchAnalysis.cpp cc/Panic.cpp -o ss.exe -g

Testcc: Testcc.cpp
	ss Testcc.cpp Testcc.asm
