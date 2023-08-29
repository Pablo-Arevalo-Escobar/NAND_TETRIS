#include <iostream>
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include <fstream>

//WIN API FILE SEARCHING
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

#include <filesystem>

const char* InFilePath = "Main.jack";
const char* OutFilePath = "MainTM.xml";

std::string FolderPaths[] =
{
    "C:/Users/pablo/workspace/NAND_TETRIS/JackAnalyzer/ArrayTest",
};
void ProcessFolder(std::string FolderPath);

int main() {
    for (std::string FolderPath : FolderPaths) {
        ProcessFolder(FolderPath);
    }
    return 1; 
}


void ProcessFolder(std::string FolderPath) {
    int NameStartIndex = FolderPath.find_last_of('/');
    std::string FileName = FolderPath.substr(NameStartIndex + 1);
    std::string OutputFile = FileName + ".xml";
    bool BFolder = true;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(FolderPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".jack") {
                std::string FName = entry.path().filename().string();
                std::string InputPath = FolderPath + '/' + FName;
                std::string OutputFile = FName + ".xml";
                Jack::CompilationEngine CompEngine(InputPath.c_str(), OutputFile.c_str());
                std::cout << "FILE NAME  " << FName << "\n";
                CompEngine.CompileClass();
            }
        }
        std::cout << FileName;
    }
    catch (...) {
        std::cout << "ERROR::ProcessFolder::Invalid_Folder_Path\n";
        exit(0);
    }

    std::cout << "Success! File Translated : )\n";
}