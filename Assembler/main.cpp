#include "Assembler.h"
#include "SymbolTable.h"
#include <iostream>
#include <bitset>

std::pair<const char*,const char*> Files[] =
{ 
    {"RectL.asm", "RectL.hack"},
    {"MaxL.asm", "MaxL.hack"},
    {"PongL.asm", "PongL.hack"},

    {"Rect.asm", "Rect.hack"},
    {"Max.asm", "Max.hack"},
    {"Pong.asm", "Pong.hack"},
};

int main() {
    Assembler Assembler("NULL");
    for (std::pair FilePaths : Files)
    {
        Assembler.Assemble(FilePaths.first, FilePaths.second);
    }
    return 0;
}