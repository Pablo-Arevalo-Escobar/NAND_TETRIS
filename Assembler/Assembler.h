#pragma once
#include "parser.h"
#include "Code.h"
#include "SymbolTable.h"

class Assembler
{
public:
    Assembler(const char* InputFilePath);
    void Assemble(const char* InputFilePath, const char* OutputFilePath);
private:
    void FirstPass();
    void SecondPass();
    
private:
    std::ofstream _OutputFile;
    Parser _Parser;
    Code _CodeGenerator;
    SymbolTable _SymbolTable;
};

