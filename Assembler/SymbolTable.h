#pragma once
#include <string>
#include <map>
class SymbolTable
{
public:
    SymbolTable();
    void AddEntry(std::string Symbol, int Address);
    bool Contains(std::string Symbol);
    int GetAddress(std::string Symbol);
    void Reset();

private:
    std::map<std::string, int> _SymbolTable;
    const static std::map<std::string, int> _PredefinedSymbols;
};

