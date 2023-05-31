#include "parser.h"
#include "Code.h"
#include <iostream>
#include <bitset>



int main() {
    std::ofstream OutputFile("Add.hack");
    if (!OutputFile) {
        std::cerr << "Error creating file!" << std::endl;
        return 1;
    }

    Parser Parser("Add.asm");
    Code CodeGenerator;
    
    while (Parser.HasMoreCommands()) {
        Parser.Advance();
        if (Parser.CommandType() == ECommandType::INVALID)
            continue;

        std::string Dest = "Null";
        std::string Comp = "Null";
        std::string Jump = "Null";
        std::string TranslatedLine = "";
        if(Parser.CommandType() == ECommandType::C_COMMAND)
            TranslatedLine += '1';
        else if(Parser.CommandType() == ECommandType::A_COMMAND)
            TranslatedLine += '0';
        bool HasError = false;


        if (Parser.CommandType() == ECommandType::C_COMMAND) {
            Dest = Parser.Dest();
            Comp = Parser.Comp();
            Jump = Parser.Jump();

            TranslatedLine += "11";
            TranslatedLine += CodeGenerator.Comp(Comp);
            TranslatedLine += CodeGenerator.Dest(Dest);
            TranslatedLine += CodeGenerator.Jump(Jump);
        }
        else if (Parser.CommandType() == ECommandType::A_COMMAND) {
            std::string Value = Parser.Symbol();
            //Add check for if value is a number or a symbol
            try {
                int ValueInt = std::stoi(Value);
                std::string BinaryValue = std::bitset<15>(ValueInt).to_string();
                TranslatedLine += BinaryValue;
            }
            catch (...) {
                std::cout << "ERROR::INVALID_SYMBOL" << std::endl;
                HasError = true;
            }
        }
        if(!TranslatedLine.empty())
            std::cout << TranslatedLine << "\n";

        OutputFile << TranslatedLine << "\n";
    }
    OutputFile.close();
    return 0;
}