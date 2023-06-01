#include "Assembler.h"
#include <iostream>
#include <bitset>


Assembler::Assembler(const char* InputFilePath) : _Parser(InputFilePath), _SymbolTable(){}

void Assembler::Assemble(const char* InputFilePath, const char* OutputFilePath)
{
    _OutputFile = std::ofstream(OutputFilePath);
    if (!_OutputFile) {
        std::cerr << "Error creating file!" << std::endl;
        return;
    }
    _Parser = Parser(InputFilePath);
    _SymbolTable.Reset();

    FirstPass();
    SecondPass();

    _OutputFile.close();
}

void Assembler::FirstPass()
{
    int ROMAddress = 0;
    while (_Parser.HasMoreCommands()) {
        _Parser.Advance();
        if (_Parser.CommandType() == ECommandType::A_COMMAND || _Parser.CommandType() == ECommandType::C_COMMAND)
            ++ROMAddress;
        if (_Parser.CommandType() == ECommandType::L_COMMAND) 
            _SymbolTable.AddEntry(_Parser.Symbol(), ROMAddress);
    }
    _Parser.Reset();
}

void Assembler::SecondPass()
{
    int RAMAddress = 16;
    while (_Parser.HasMoreCommands()) {
        _Parser.Advance();
        if (_Parser.CommandType() == ECommandType::INVALID || _Parser.CommandType() == ECommandType::L_COMMAND)
            continue;
        std::string TranslatedLine = "";

        if (_Parser.CommandType() == ECommandType::C_COMMAND) {
            TranslatedLine += '1';
            std::string Dest, Comp, Jump = "Null";
            Dest = _Parser.Dest();
            Comp = _Parser.Comp();
            Jump = _Parser.Jump();

            TranslatedLine += "11";
            TranslatedLine += _CodeGenerator.Comp(Comp);
            TranslatedLine += _CodeGenerator.Dest(Dest);
            TranslatedLine += _CodeGenerator.Jump(Jump);
        }

        else if (_Parser.CommandType() == ECommandType::A_COMMAND) {
            TranslatedLine += '0';
            std::string Symbol = _Parser.Symbol();
            try {
                //Symbol is a constant
                int ValueInt = std::stoi(Symbol);
                std::string BinaryValue = std::bitset<15>(ValueInt).to_string();
                TranslatedLine += BinaryValue;
            }
            catch (...) {
                //Symbol is a variable
                if (!_SymbolTable.Contains(Symbol)) {
                    _SymbolTable.AddEntry(Symbol, RAMAddress);
                    ++RAMAddress;
                }
                int Address = _SymbolTable.GetAddress(Symbol);
                std::string BinaryValue = std::bitset<15>(Address).to_string();
                TranslatedLine += BinaryValue;
            }
        }
        _OutputFile << TranslatedLine << "\n";
    }
}

