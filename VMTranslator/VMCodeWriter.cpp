#include "VMCodeWriter.h"
#include <map>
#include <unordered_set>
#include <iostream>

static std::map<std::string, std::string> _ArithmeticMap = {
    {"add","+"},{"sub","-"},{"neg","-"},
    {"and","&"},{"or","|"},{"not","!"}
};

static std::unordered_set<std::string> _UnarySet = {
    "neg", "not"
};


static std::map<std::string, std::string> _SegmentMap = {
    {"temp","5"}, {"local","LCL"}, {"argument","ARG"},
    {"this","THIS"}, {"that","THAT"}
};


VMCodeWriter::VMCodeWriter(const char* OutputFilePath)
{
    _OutputFile = std::ofstream(OutputFilePath);
    if (!_OutputFile) {
        std::cerr << "Error creating file!" << std::endl;
        return;
    }
    _OutputFile <<
        "@256\n" <<
        "D=A\n" <<
        "@SP\n" <<
        "M = D\n";
}

void VMCodeWriter::WriteArithmetic(const char* Command)
{
    //Implement assembly code for arithmetic commands
    // add, sub , neg
    // eq gt lt
    // and or not

    //In terms of logic we don't care what arithmetic command is being used as they all have the same formula
    // pop two and then perform operation on them

    //The procedure is as follows:
    //1. Pop the two topmost values from the stack and store them in D and M
    //2. Perform the operation
    
    //Print a debug message to the file 

    if (_ArithmeticMap.count(Command) == 0) {
        std::cerr << "VMCodeWrite::WriteArithmetic::INVALID COMMAND\n";
        return;
    }

    #ifdef DEBUG
        std::cout << "ARITHMETIC COMMAND DEBUG :: " << Command << "\n";
        _OutputFile << "// ARITHMETIC COMMAND: " << Command << "\n";
    #endif


    if (_UnarySet.count(Command) == 0) { // Binary Operators
            //Pop two elements from the stack
            WritePushPop(EVMCommandType::C_POP, "temp", 0);
            WritePushPop(EVMCommandType::C_POP, "temp", 1);

            //Perform the operation
            //...
            _OutputFile <<
                "@6\n" <<
                "D=M\n" <<
                "@5\n" <<
                "M = M" << _ArithmeticMap[Command] << "D\n";

            //Push the result back to the stack
            WritePushPop(EVMCommandType::C_PUSH, "temp", 0);
    }
    else { // Unary Operators

    }

    #ifdef DEBUG
        _OutputFile << "// END OF ARITHMETIC COMMAND: " << Command << "\n";
    #endif
}

void VMCodeWriter::WritePushPop(EVMCommandType Command, const char* Segment, int Index)
{
    #ifdef _DEBUG
        std::cout << "Segment " << Segment << " Index " << Index << "\n";
    #endif

    if (_SegmentMap.count(Segment) == 1) {
        WriteDynamic(Command, Segment, Index);
    }
    else {
        //Handle Pointer, Temp, Constant, and Static values
        switch (Command) {
        case C_POP:
            break;
        case C_PUSH:
            if (strcmp(Segment ,"constant") == 0) {
                _OutputFile <<
                    "@" << Index << "\n" <<
                    "D=A\n" <<
                    "@SP\n" <<
                    "M=M+1\n" <<
                    "A=M\n" <<
                    "M=D\n";
            }
            break;
        }
        
    }


    #ifdef DEBUG
        _OutputFile << "// END OF PUSH/POP COMMAND\n";
    #endif
    
}


void VMCodeWriter::WriteDynamic(EVMCommandType Command, const char* Segment, int Index) {
    
    if (_SegmentMap.count(Segment) == 0) {
        std::cerr << "VMCodeWrite::WriteDynamic::INVALID_SEGMENT_COMMAND " << Segment << "\n";
        return;
    }
    switch (Command) {
    case C_POP:
    #ifdef DEBUG
        std::cout << "DYNAMIC POP\n";
        _OutputFile << "// DYNAMIC POP COMMAND " << Segment << " " << Index << "\n";
    #endif
    _OutputFile <<
        "@" << _SegmentMap[Segment] << "\n" <<
        "D = A\n" <<
        "@" << Index << "\n" <<
        "A = D + A\n" <<
        "D = A\n" << // Store address [segment+index]
        "@R13\n" << // Register - this may cause issues in the future 
        "M=D\n" << // Store dest address in r13
        "@SP\n" <<
        "A=M\n" <<
        "D=M\n" << // Retrieve value from the top of the stack
        "@R13\n" <<
        "A=M\n" << // Go to destination address
        "M=D\n" << // Store top of stack value into dest address
        "@SP\n" <<
        "M=M-1\n";
    break;

    case C_PUSH:
    #ifdef DEBUG
        _OutputFile << "// DYNAMIC PUSH COMMAND " << Segment << " " << Index << "\n";
    #endif
    _OutputFile <<
        "@SP\n" << // Access stack pointer
        "M = M+1\n" << // Increment pointer Value
        "@" << Segment << "\n" << // Access segment
        "D = A\n" << // Store segment address
        "@" << Index << "\n" << // Access index
        "A = D+A\n" << // Increment index value By segment value and go to that address
        "D = M\n" << // Store the value in that  address to the data register
        "@SP\n" << // Access stack pointer
        "A = M\n" << // Go to top of stack
        "M = D\n";                // Store data register value into top of stack
    break;

    }
}








void VMCodeWriter::Close()
{
    _OutputFile.close();
}
