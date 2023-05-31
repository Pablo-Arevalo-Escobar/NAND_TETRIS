//Parser header file
#include <stdio.h>
#include <string>
#include <fstream>

enum ECommandType {A_COMMAND = 0, C_COMMAND = 1, L_COMMAND = 2, INVALID = -1};
class Parser {
    public:
        Parser(const char* InputFilePath);
        bool HasMoreCommands();
        void Advance();
        ECommandType CommandType();
        std::string Symbol();
        std::string Dest();
        std::string Comp();
        std::string Jump();

    private:
        std::string ReadLine();

    private:
        std::fstream _File;
        std::string _CurrentCommand = "";
};
