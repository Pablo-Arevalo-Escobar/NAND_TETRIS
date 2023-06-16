#pragma once
#include <string>
#include <fstream>

enum EVMCommandType {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
    INVALID,
};


//V1.0 - Only supports arithmetic and push/pop commands
class VMParser
{
public:
    VMParser(const char* InputFilePath);
    bool HasMoreLines();
    void Advance();
    EVMCommandType CommandType();
    std::string GetArg1();
    int GetArg2();
    void Close();
private:
    std::string ReadLine();
private:
    std::fstream _File;
    std::string _CurrentCommand = "";
};

