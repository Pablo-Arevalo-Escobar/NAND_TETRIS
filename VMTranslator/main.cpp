#include <iostream>
#include "VMParser.h"
#include "VMCodeWriter.h"
#include <fstream>


static const char* InputFile = "SimpleAdd.vm";
static const char* OutputFile = "SimpleAdd.asm";

int main(int ArgCount, char* ArgValues[]) {
    VMParser Parser(InputFile);
    VMCodeWriter CodeWriter(OutputFile);

    while (Parser.HasMoreLines()) {
        Parser.Advance();
        EVMCommandType CommandType = Parser.CommandType();
        
        switch (CommandType) {
        case C_ARITHMETIC:
            CodeWriter.WriteArithmetic(Parser.GetArg1().c_str());
            break;
        case C_PUSH:
            CodeWriter.WritePushPop(C_PUSH, Parser.GetArg1().c_str(), Parser.GetArg2());
            break;
        case C_POP:
            CodeWriter.WritePushPop(C_POP, Parser.GetArg1().c_str(), Parser.GetArg2());
            break;
        case INVALID:
            break;
        default:
            std::cerr << "ERROR::COMMAND_TYPE_PROCESS_NOT_IMPLEMENTED_YET\n";
        }
    }
    CodeWriter.Close();
    Parser.Close();
    std::cout << "File translated!\n";
    //std::string _CurrentCommand = " push constant 7 ";
    //std::string ArgSubString = _CurrentCommand.substr(_CurrentCommand.find(' ') + 1);
    //std::cout <<  ArgSubString.substr(ArgSubString.find(' '));


    return 1;
}