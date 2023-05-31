#include "Code.h"

//HashMap for C-Instruction Mnemonics
std::map<std::string, std::string> Code::_CompMap = {
    //When a == 0
    {"0","0101010"},
    {"1","0111111"},
    {"-1","0111010"},
    {"D","0001100"},
    {"A","0110000"},
    {"!D","0001101"},
    {"!A","0110001"},
    {"-D","0001111"},
    {"-A","0110011"},
    {"D+1","0011111"},
    {"A+1","0110111"},
    {"D-1","0001110"},
    {"A-1","0110010"},
    {"D+A","0000010"},
    {"D-A","0010011"},
    {"A-D","0000111"},
    {"D&A","0000000"},
    {"D|A","0010101"},

    // when a == 1
    {"M","1110000"},
    {"!M","1110001"},
    {"-M","1110011"},
    {"M+1","1110111"},
    {"M-1","1110010"},
    {"D+M","1000010"},
    {"D-M","1010011"},
    {"M-D","1000111"},
    {"D&M","1000000"},
    {"D|M","1010101"}
};

std::map<std::string, std::string> Code::_DestMap = {
    {"Null","000"},
    {"M","001"},
    {"D","010"},
    {"MD","011"},
    {"A","100"},
    {"AM","101"},
    {"AD","110"},
    {"AMD","111"}
};

std::map<std::string, std::string> Code::_JumpMap = {
    {"Null","000"},
    {"JGT","001"},
    {"JEQ","010"},
    {"JGE","011"},
    {"JLT","100"},
    {"JNE","101"},
    {"JLE","110"},
    {"JMP","111"}
};

std::string Code::Dest(std::string Mnemonic)
{
    if (_DestMap.contains(Mnemonic))
        return _DestMap[Mnemonic];
    return "000";
}

std::string Code::Comp(std::string Mnemonic)
{
    if (_CompMap.contains(Mnemonic))
        return _CompMap[Mnemonic];
    return _CompMap["0"];
}

std::string Code::Jump(std::string Mnemonic)
{
    if (_JumpMap.contains(Mnemonic))
        return _JumpMap[Mnemonic];
    return "000";
}
