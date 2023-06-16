#include "VMParser.h"
#include <iostream>

static std::string _ArithmeticCommands[] {
    "add","sub","neg",    // Arithmetic
    "eq", "gt", "lt",     // Comparison 
    "and" , "or", "not"   //Logical
};


VMParser::VMParser(const char* InputFilePath)
{
    try {
        _File.open(InputFilePath);
        if (_File.fail()) throw _File.failbit;
    }
    catch (...) {
        std::cerr << "ERROR::VMPARSER::FILE_NOT_FOUND_FAILBIT_" << _File.failbit << std::endl;
    }
}

bool VMParser::HasMoreLines()
{
    if (!_File.eof()) return true;
    return false;
}

void VMParser::Advance()
{
    if (!_File.is_open()) {
        std::cerr << "VMPARSER::CONSTRUCTOR::FILE_NOT_OPEN" << std::endl;
        return;
    }
    if (!HasMoreLines()) {
        std::cerr << "VMPARSER::ADVANCE::NO_MORE_COMMANDS" << std::endl;
        return;
    }
    do {
        _CurrentCommand = ReadLine();
    } while (_CurrentCommand.empty() && !_File.eof());

    #ifdef DEBUG
        std::cout << "CURRENT COMMAND :" << _CurrentCommand << "\n";
    #endif
}

EVMCommandType VMParser::CommandType()
{
    if (_CurrentCommand.empty()) {
        std::cerr << "PARSER::CommandType::NO_CURRENT_COMMAND" << std::endl;
        return EVMCommandType::INVALID;
    }

    //A faster alternative is to extract the first word of the line and use a hashmap to find the command type
    if (_CurrentCommand.find("push") != std::string::npos)
        return EVMCommandType::C_PUSH;
    else if (_CurrentCommand.find("pop") != std::string::npos)
        return EVMCommandType::C_POP;

    for (std::string ArithmeticCommand : _ArithmeticCommands) {
        if (_CurrentCommand.find(ArithmeticCommand) != std::string::npos)
            return EVMCommandType::C_ARITHMETIC;
    }
    


    return EVMCommandType::INVALID;
}

std::string VMParser::GetArg1()
{
    if (CommandType() == EVMCommandType::C_RETURN) {
        std::cerr << "VMPARSER::GETARG1::C_RETURN - GetArg1 Should Not Be Called With This Command Type" << std::endl;
        return std::string();
    }
    if (CommandType() == EVMCommandType::C_ARITHMETIC)
        return _CurrentCommand.substr(0, _CurrentCommand.find(' '));

    std::string ArgSubString = _CurrentCommand.substr(_CurrentCommand.find(' ') + 1);
    return ArgSubString.substr(0, ArgSubString.find(' '));
}

int VMParser::GetArg2()
{
    EVMCommandType CurrentCommandType = CommandType();
    if (CurrentCommandType != C_PUSH && CurrentCommandType != C_POP
        && CurrentCommandType != C_FUNCTION && CurrentCommandType != C_CALL) {
        std::cerr << "VMPARSER::GETARG2::INVALID_COMMAND_TYPE" << std::endl;
        return -1;
    }
    std::string ArgSubString = _CurrentCommand.substr(_CurrentCommand.find(' ') + 1);
    int Result = -1;
    try {
        Result = std::stoi(ArgSubString.substr(ArgSubString.find(' ')));
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "VMPARSER::GETARG2::ARG2 - Invalid argument: " << e.what() << std::endl;
        std::cerr << ArgSubString << " \n";
    }
    return Result;
}

void VMParser::Close()
{
    _File.close();
}

std::string VMParser::ReadLine()
{
    if (!_File.is_open()) {
        std::cerr << "ERROR::VMPARSER::FILE_NOT_OPEN" << std::endl;
        return "";
    }
    std::string NewLine = "";
    char CurrentChar = ' ';
    while (CurrentChar != '\n' && !_File.eof()) {
        CurrentChar = _File.get();
        if (CurrentChar == '\n') { break; }
        if (CurrentChar == ' ' && NewLine.size() == 0) continue;
        if (CurrentChar == '/') {
            while (CurrentChar != '\n' && !_File.eof()) {
                CurrentChar = _File.get();
            }
            CurrentChar = ' ';
            if (NewLine.empty()) continue;
            else break;
        }
        NewLine += CurrentChar;
    }
    return NewLine;
}
