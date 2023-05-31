//Parser cpp file
#include "Parser.h"
#include <iostream>




Parser::Parser(const char* InputFilePath) {
    try {
        _File.open(InputFilePath);
        if(_File.fail()) throw _File.failbit;
    }
    catch(...) {
        std::cout << "ERROR::PARSER::FILE_NOT_FOUND_FAILBIT_"<< _File.failbit << std::endl;
    }
}

bool Parser::HasMoreCommands() {
    if(!_File.eof()) return true;
    return false;
}

void Parser::Advance() {
    if(!_File.is_open()) {
        std::cout << "PARSER::CONSTRUCTOR::FILE_NOT_OPEN" << std::endl;
        return;
    }
    if (!HasMoreCommands()) {
        std::cout << "PARSER::ADVANCE::NO_MORE_COMMANDS" << std::endl;
        return;
    }
    do {
        _CurrentCommand = ReadLine();
    } while (_CurrentCommand.empty() && !_File.eof());
}

ECommandType Parser::CommandType()
{
    if (_CurrentCommand.empty()) {
        std::cout << "PARSER::CommandType::NO_CURRENT_COMMAND" << std::endl;
        return ECommandType::INVALID;
    }
    
    //A_COMMAND FORMAT @xxx
    if(_CurrentCommand[0] == '@')
        return ECommandType::A_COMMAND;

    //C_COMMAND FORMAT dest=comp;jump
    if(_CurrentCommand.find('=') != std::string::npos || _CurrentCommand.find(';') != std::string::npos)
        return ECommandType::C_COMMAND;

    //L_COMMAND FORMAT Xxx
    return ECommandType::L_COMMAND;
}

std::string Parser::Symbol() {
    ECommandType CType = CommandType();
    if (CType != ECommandType::A_COMMAND && CType != ECommandType::L_COMMAND) {
       std::cout << "PARSER::SYMBOL::WRONG_COMMAND_TYPE" << std::endl;
       return "Null";
    }
    if(_CurrentCommand.find("@") != std::string::npos)
        return _CurrentCommand.substr(1);
    return _CurrentCommand;
}

std::string Parser::Dest() {
    if(CommandType() != ECommandType::C_COMMAND) {
        std::cout << "PARSER::DEST::WRONG_COMMAND_TYPE" << std::endl;
        return "Null";
    }
    std::string Dest = "Null";
    if (_CurrentCommand.find("=") != std::string::npos) 
        Dest = _CurrentCommand.substr(0, _CurrentCommand.find("="));
    return Dest;
}

std::string Parser::Comp()
{
    if (CommandType() != ECommandType::C_COMMAND) {
        std::cout << "PARSER::DEST::WRONG_COMMAND_TYPE" << std::endl;
        return "";
    }

    //C-Instruciton Format dest=comp;jump
    //Either dest and = is empty or jump and ; is empty
    std::string CompString = "Null";
    size_t EqualPos = _CurrentCommand.find('=');
    size_t SemiColonPos = _CurrentCommand.find(';');
    if (EqualPos != std::string::npos && SemiColonPos != std::string::npos) {
        CompString = _CurrentCommand.substr(EqualPos + 1, SemiColonPos);
    }
    else if (EqualPos != std::string::npos) {
        CompString = _CurrentCommand.substr(EqualPos + 1);
    }
    else if (SemiColonPos != std::string::npos) {
        CompString = _CurrentCommand.substr(0, SemiColonPos);
    }

    return CompString;
}

std::string Parser::Jump()
{
    if (CommandType() != ECommandType::C_COMMAND) {
        std::cout << "PARSER::DEST::WRONG_COMMAND_TYPE" << std::endl;
        return "";
    }
    size_t SemiColonPos = _CurrentCommand.find(";");
    std::string Jump = "Null";
    if (SemiColonPos != std::string::npos) {
        Jump = _CurrentCommand.substr(SemiColonPos);
    }
    return Jump;
}

std::string Parser::ReadLine()
{
    if (!_File.is_open()) {
        std::cout << "ERROR::PARSER::FILE_NOT_OPEN" << std::endl;
        return "";
    }
    std::string NewLine = "";
    char CurrentChar = ' ';
    while(CurrentChar != '\n' && !_File.eof()) {
        CurrentChar = _File.get();
        if(CurrentChar == '\n') break;
        if(CurrentChar == ' ') continue;
        if(CurrentChar == '/') {
            while(CurrentChar != '\n' && !_File.eof()) {
                CurrentChar = _File.get();
            }
            CurrentChar = ' ';
            continue;
        }
        NewLine += CurrentChar;
    }
    return NewLine;
}
