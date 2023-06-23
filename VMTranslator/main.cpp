#include <iostream>
#include "VMParser.h"
#include "VMCodeWriter.h"
#include <fstream>

std::pair<const char*, const char*> Files[] =
{
   /* {"SimpleAdd.vm", "SimpleAdd.asm"},
    {"StackTest.vm", "StackTest.asm"},
    {"BasicTest.vm", "BasicTest.asm"},*/
    {"PointerTest.vm", "PointerTest.asm"},
    {"PointerTestCopy.vm", "PointerTestCopy.asm"},

    //{"StaticTest.vm", "StaticTest.asm"},
};
void ProcessFile(const char* InputFile, const char* OutputFile);

int main(int ArgCount, char* ArgValues[]) {
    
    //std::string _CurrentCommand = " push constant 7 ";
    //std::string ArgSubString = _CurrentCommand.substr(_CurrentCommand.find(' ') + 1);
    //std::cout <<  ArgSubString.substr(ArgSubString.find(' '));
    for (std::pair<const char*, const char*> FilePaths : Files) {
        ProcessFile(FilePaths.first, FilePaths.second);
    }
    return 1;
}


void ProcessFile(const char* InputFile, const char* OutputFile) {
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
        default:
            std::cerr << "ERROR::COMMAND_TYPE_PROCESS_NOT_IMPLEMENTED_YET\n";
        }
    }
    CodeWriter.Close();
    Parser.Close();
    std::cout << "File translated!\n";
}