#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    _SymbolTable = _PredefinedSymbols;
}

void SymbolTable::AddEntry(std::string Symbol, int Address)
{
    if (_SymbolTable.contains(Symbol))
        return;
    _SymbolTable[Symbol] = Address;
}

bool SymbolTable::Contains(std::string Symbol)
{
    return _SymbolTable.contains(Symbol);
}

int SymbolTable::GetAddress(std::string Symbol)
{
    if (!_SymbolTable.contains(Symbol)) {
        return -1;
    }
    return _SymbolTable[Symbol];
}

void SymbolTable::Reset()
{
    _SymbolTable.clear();
    _SymbolTable = _PredefinedSymbols;
}



//HashMap for predefined symbols
const std::map<std::string, int> SymbolTable::_PredefinedSymbols = {
    //When a == 0
    {"SP",0},
    {"LCL",1},
    {"ARG",2},
    {"THIS",3},
    {"THAT",4},

    {"R0",0},
    {"R1",1},
    {"R2",2},
    {"R3",3},
    {"R4",4},
    {"R5",5},
    {"R6",6},
    {"R7",7},
    {"R8",8},
    {"R9",9},
    {"R10",10},
    {"R11",11},
    {"R12",12},
    {"R13",13},
    {"R14",14},
    {"R15",15},

    {"SCREEN",16384},
    {"KBD",24576},
};