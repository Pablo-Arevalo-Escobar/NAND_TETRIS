#include <iostream>
#include "VMParser.h"
#include "VMCodeWriter.h"
#include <fstream>

//WIN API FILE SEARCHING
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

#include <filesystem>

std::pair<const char*, const char*> Files[] =
{
    //{"SimpleAdd.vm", "SimpleAdd.asm"},
    //{"StackTest.vm", "StackTest.asm"},
    //{"BasicTest.vm", "BasicTest.asm"},
    //{"PointerTest.vm", "PointerTest.asm"},
    //{"PointerTestCopy.vm", "PointerTestCopy.asm"},
    {"StaticTest.vm", "StaticTest.asm"}, 

    ////Branching and Functions
    //{"BasicLoop.vm", "BasicLoop.asm"},
    //{"FibonacciSeries.vm", "FibonacciSeries.asm"},
    {"SimpleFunction.vm", "SimpleFunction.asm"},
};

std::string FolderPaths[] =
{
    "C:/Users/pablo/workspace/NAND_TETRIS/VMTranslator/FibonacciElement",
    "C:/Users/pablo/workspace/NAND_TETRIS/VMTranslator/SimpleReturn",
    "C:/Users/pablo/workspace/NAND_TETRIS/VMTranslator/StaticsTest",
    "C:/Users/pablo/workspace/NAND_TETRIS/VMTranslator/NestedCall",

};

void ProcessFile(const char* InputFile, const char* OutputFile);
void ProcessFolder(std::string FolderPath);
void ProcessFile(const char* InputFile, VMCodeWriter& CodeWriter);

int main(int ArgCount, char* ArgValues[]) {
    
    //std::string _CurrentCommand = " push constant 7 ";
    //std::string ArgSubString = _CurrentCommand.substr(_CurrentCommand.find(' ') + 1);
    //std::cout <<  ArgSubString.substr(ArgSubString.find(' '));
    for (std::string FolderPath : FolderPaths) {
        ProcessFolder(FolderPath);
    }
    for (std::pair<const char*, const char*> FilePaths : Files) {
        ProcessFile(FilePaths.first, FilePaths.second);
    }
    return 1;
}


void ProcessFolder(std::string FolderPath) {
    int NameStartIndex = FolderPath.find_last_of('/');
    std::string FileName = FolderPath.substr(NameStartIndex + 1);
    std::string OutputFile = FileName + ".asm";
    bool BFolder = true;

    VMCodeWriter CodeWriter(OutputFile.c_str(), BFolder);
    try {
        for (const auto& entry : std::filesystem::directory_iterator(FolderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".vm") {
                std::string FName = entry.path().filename().string();
                FName = FName.substr(0, FName.length() - 3);
                std::cout << "FILE NAME  " << FName << "\n";

                CodeWriter.SetFileName(FName);
                ProcessFile(entry.path().string().c_str(), CodeWriter);
            }
        }
        CodeWriter.Close();
        
        std::cout << FileName;
    }
    catch(...) {
        std::cout << "ERROR::ProcessFolder::Invalid_Folder_Path\n";
        exit(0);
    }

    std::cout << "Success! File Translated : )\n";
}


void ProcessFile(const char* InputFile, const char* OutputFile) {
    VMParser Parser(InputFile);
    VMCodeWriter CodeWriter(OutputFile);
    std::string FName = InputFile;
    CodeWriter.SetFileName(FName.substr(0, FName.size() - 3));

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
        case C_LABEL:
            CodeWriter.WriteLabel(Parser.GetArg1().c_str());
            break;
        case C_GOTO:
            CodeWriter.WriteGoTo(Parser.GetArg1().c_str());
            break;
        case C_IF:
            CodeWriter.WriteIf(Parser.GetArg1().c_str());
            break;
        case C_FUNCTION:
            CodeWriter.WriteFunction(Parser.GetArg1().c_str(), Parser.GetArg2());
            break;
        case C_CALL:
            CodeWriter.WriteCall(Parser.GetArg1().c_str(), Parser.GetArg2());
            break;
        case C_RETURN:
            CodeWriter.WriteReturn();
            break;
        case INVALID:
        default:
            std::cerr << "ProcessFile::ERROR::COMMAND_TYPE_PROCESS_NOT_IMPLEMENTED_YET\n";
        }
    }
    CodeWriter.Close();
    Parser.Close();
    std::cout << "File translated!\n";
}

void ProcessFile(const char* InputFile, VMCodeWriter& CodeWriter) {
    VMParser Parser(InputFile);

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
        case C_LABEL:
            CodeWriter.WriteLabel(Parser.GetArg1().c_str());
            break;
        case C_GOTO:
            CodeWriter.WriteGoTo(Parser.GetArg1().c_str());
            break;
        case C_IF:
            CodeWriter.WriteIf(Parser.GetArg1().c_str());
            break;
        case C_FUNCTION:
            CodeWriter.WriteFunction(Parser.GetArg1().c_str(), Parser.GetArg2());
            break;
        case C_CALL:
            CodeWriter.WriteCall(Parser.GetArg1().c_str(), Parser.GetArg2());
            break;
        case C_RETURN:
            CodeWriter.WriteReturn();
            break;
        case INVALID:
        default:
            std::cerr << "ProcessFile::ERROR::COMMAND_TYPE_PROCESS_NOT_IMPLEMENTED_YET\n";
        }
    }
    Parser.Close();
    std::cout << "File translated!\n";
}