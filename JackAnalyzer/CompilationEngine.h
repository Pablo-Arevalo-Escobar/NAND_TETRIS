#pragma once
#include <fstream>
#include "JackTokenizer.h"

namespace Jack {
class CompilationEngine
{
public:
    CompilationEngine(const char* InputFilePath, const char* OutputFilePath);
    ~CompilationEngine();
    void CompileClass();
    void CompileClassVarDec();
    void CompileSubroutine();
    void CompileParameterList();
    void CompileSubroutineBody();
    void CompileVarDec();
    void CompileStatements();
    void CompileLet();
    void CompileIf();
    void CompileWhile();
    void CompileDo();
    void CompileReturn();
    void CompileExpression();
    void CompileTerm();
    int CompileExpressionList();

    void SubroutineCall();
private:
    std::string ExtractSymbol(char Token);
    bool ValidateKeyword(EKeyWord ExpectedWord);
    bool ValidateSymbol(char Symbol);
    bool ValidateIdentifier();
    bool ValidateType();
    bool LookAheadKeyword(EKeyWord Expected);
    bool LookAheadTerm();
    bool ValidateOperation();
    void Write(std::string s, bool bStart);
    void WriteToken(ETokenType ExpectedType);
private:
    JackTokenizer _Tokenizer;
    std::ofstream _OutFile;
    int _IndentLevel = 0;
    bool _Error = false;
};

}


