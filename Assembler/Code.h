#pragma once
#include <string>
#include <map>
class Code
{
public:
    std::string Dest(std::string Mnemonic);
    std::string Comp(std::string Mnemonic);
    std::string Jump(std::string Mnemonic);
private:
    static  std::map<std::string, std::string> _CompMap;
    static  std::map<std::string, std::string> _JumpMap;
    static  std::map<std::string, std::string> _DestMap;
};

