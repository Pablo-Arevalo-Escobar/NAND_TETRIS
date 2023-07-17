#include "VMCodeWriter.h"
#include <map>
#include <unordered_set>
#include <iostream>
#include <sstream>

static std::map<std::string, std::string> _ArithmeticMap = {
    {"add","+"},{"sub","-"},{"neg","-"},
    {"and","&"},{"or","|"},{"not","!"}
};

static std::map<std::string, std::string> _JumpMap = {
    {"eq","JEQ"}, {"gt","JGT"}, {"lt","JLT"}
};

static std::map<std::string,std::string> _UnaryMap = {
    {"neg","-"}, {"not","!"}
};


static std::map<std::string, std::string> _SegmentMap = {
    {"temp","5"}, {"local","LCL"}, {"argument","ARG"},
    {"this","THIS"}, {"that","THAT"}, {"reg","R14"}, 
    {"static","INDF"}, {"pointer", "INDF"}// Undefined
};

static std::map<std::string, int> _LabelMap = {

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

VMCodeWriter::VMCodeWriter(const char* OutputFilePath, bool BSysInit)
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

    PushFrame("StartPoint", 0);
    WriteGoTo("Sys.init");
    WriteLabel("StartPoint");

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

    if (_ArithmeticMap.count(Command) == 0 && _JumpMap.count(Command) == 0) {
        std::cerr << "VMCodeWrite::WriteArithmetic::INVALID COMMAND\n";
        return;
    }

    #ifdef DEBUG
        std::cout << "ARITHMETIC COMMAND DEBUG :: " << Command << "\n";
        _OutputFile << "// ARITHMETIC COMMAND: " << Command << "\n";
    #endif


    if (_UnaryMap.count(Command) == 0) { // Binary Operators
            //Pop two elements from the stack
            //temp register: 5 + i
            WritePushPop(EVMCommandType::C_POP, "reg", 1); 
            WritePushPop(EVMCommandType::C_POP, "reg", 0);

            //Perform the operation
            if (_ArithmeticMap.count(Command) != 0) {
                _OutputFile <<
                    "@R15\n" <<
                    "D=M\n" <<
                    "@R14\n" <<
                    "M = M" << _ArithmeticMap[Command] << "D\n";
            }

            //Jump Command
            else {
                _OutputFile <<
                    "@R15\n" <<
                    "D=M\n" <<
                    "@R14\n" <<
                    "D = M-D\n" <<
                    "(LOOP)\n" <<
                    "M=1\n" <<
                    "@END" << _LoopCounter << "\n" <<
                    "D;" << _JumpMap[Command] << "\n" <<
                    "@R14\n" <<
                    "M=0\n" <<
                    "(END" << _LoopCounter << ")\n";
                ++_LoopCounter;
            }
    }
    else { // Unary Operators
        WritePushPop(EVMCommandType::C_POP, "reg", 0);
        _OutputFile <<
            "@R14\n" <<
            "D=" << _UnaryMap[Command] << "M\n" <<
            "M=D\n";
    }


    //Push the result back to the stack
    WritePushPop(EVMCommandType::C_PUSH, "reg", 0);

    #ifdef DEBUG
        _OutputFile << "// END OF ARITHMETIC COMMAND: " << Command << "\n";
    #endif
}

void VMCodeWriter::WritePushPop(EVMCommandType Command, std::string Segment, int Index)
{
    #ifdef DEBUG
        std::cout << "Segment " << Segment << " Index " << Index << "\n";
        _OutputFile << "// SEGMENT " << Segment << " INDEX " << Index << "\n";
    #endif

 
    if (_SegmentMap.count(Segment) == 1)
    {
        WriteDynamic(Command, Segment.c_str(), Index);
    }
    else if (strcmp(Segment.c_str(), "constant") == 0) {
        //Handle Constant Push values
        _OutputFile <<
            "@" << Index << "\n" <<
            "D=A\n" <<
            "@SP\n" <<
            // "M=M+1\n" <<
            "A=M\n" <<
            "M=D\n" <<
            "@SP\n" <<
            "M=M+1\n";
        
    }


    #ifdef DEBUG
        _OutputFile << "// END OF PUSH/POP COMMAND\n";
    #endif
    
}

void VMCodeWriter::SetFileName(std::string FileName)
{
    _FileName = FileName;
}

void VMCodeWriter::WriteLabel(std::string Label)
{
    #ifdef DEBUG
        std::cout << "WriteLabel: " << Label << '\n';
        _OutputFile << "// Write Label " << Label << '\n';
    #endif

    //TODO: Implement error checking for label format
    _OutputFile << '(' << Label << ")\n";

    #ifdef DEBUG
        _OutputFile << "// END OF WriteLabel COMMAND\n";
    #endif
}

void VMCodeWriter::WriteGoTo(std::string GoToLabel)
{
    #ifdef DEBUG
        std::cout << "WriteGoToLabel: " << GoToLabel << '\n';
        _OutputFile << "// Write GoToLabel " << GoToLabel << '\n';
    #endif

    _OutputFile << '@' << GoToLabel << '\n';
    _OutputFile << "0;JMP\n";

    #ifdef DEBUG
            _OutputFile << "// END OF WriteGoToLabel COMMAND\n";
    #endif
}

void VMCodeWriter::WriteIf(std::string IfGoToLabel)
{

    #ifdef DEBUG
        std::cout << "WriteGoToLabel: " << IfGoToLabel << '\n';
        _OutputFile << "// Write IfGoToLabel " << IfGoToLabel << '\n';
    #endif

    //Pop top stack value into @R14
    WritePushPop(EVMCommandType::C_POP, "reg", 0);
    _OutputFile << "@R14\n";
    _OutputFile << "D=M\n";
    _OutputFile << '@' << IfGoToLabel << '\n';
    _OutputFile << "D;JGT\n";

    #ifdef DEBUG
        _OutputFile << "// END OF WriteIfGoToLabel COMMAND\n";
    #endif

}

void VMCodeWriter::WriteFunction(std::string FuncName, int NumOfParams)
{
#ifdef DEBUG
    std::cout << "WriteFunction: " << FuncName  << " " << NumOfParams  << '\n';
    _OutputFile << "// WriteFunction " << FuncName << " " << NumOfParams << '\n';
#endif
    WriteLabel(FuncName);
    for (int i = 0; i < NumOfParams; ++i) {
        WritePushPop(C_PUSH, "constant", 0);
    }

#ifdef DEBUG
    _OutputFile << "// END OF WriteFunction COMMAND\n";
#endif
    
}

void VMCodeWriter::PushFrame(std::string Label, int NumOfArgs) {
    //Push the Label(return address) onto the stack
    //TODO: Not good to have this stack behaviour in this function
    // Should be translated to something around WritePushPop(C_PUSH, Label:NAME,0);
    _OutputFile <<
        "@" << Label << "\n" <<
        "D=A\n" <<
        "@SP\n" <<
        "A=M\n" <<
        "M=D\n" <<
        "@SP\n" <<
        "M=M+1\n";

    //WritePushPop(EVMCommandType::C_PUSH, "local", 0);
    _OutputFile <<
        "@LCL\n" <<
        "D=M\n" <<
        "@SP\n" <<
        // "M=M+1\n" <<
        "A=M\n" <<
        "M=D\n" <<
        "@SP\n" <<
        "M=M+1\n";

    //WritePushPop(EVMCommandType::C_PUSH, "argument", 0);
    _OutputFile <<
        "@ARG\n" <<
        "D=M\n" <<
        "@SP\n" <<
        // "M=M+1\n" <<
        "A=M\n" <<
        "M=D\n" <<
        "@SP\n" <<
        "M=M+1\n";


    //WritePushPop(EVMCommandType::C_PUSH, "this", 0);
    _OutputFile <<
        "@THIS\n" <<
        "D=M\n" <<
        "@SP\n" <<
        // "M=M+1\n" <<
        "A=M\n" <<
        "M=D\n" <<
        "@SP\n" <<
        "M=M+1\n";
    //WritePushPop(EVMCommandType::C_PUSH, "that", 0);
    _OutputFile <<
        "@THAT\n" <<
        "D=M\n" <<
        "@SP\n" <<
        // "M=M+1\n" <<
        "A=M\n" <<
        "M=D\n" <<
        "@SP\n" <<
        "M=M+1\n";


    _OutputFile <<
        "@SP\n" <<
        "D=M\n" <<
        "@5\n" <<
        "D=D-A\n" <<
        "@" << NumOfArgs << "\n" <<
        "D=D-A\n" <<
        "@ARG\n" <<
        "M=D\n" <<
        "@SP\n" <<
        "D=M\n" <<
        "@LCL\n" <<
        "M = D\n";
}

void VMCodeWriter::WriteCall(std::string FuncName, int NumOfArgs)
{

#ifdef DEBUG
    std::cout << "WriteCall: " << FuncName << " " << NumOfArgs << '\n';
    _OutputFile << "// WriteCall  " << FuncName << " " << NumOfArgs << '\n';
#endif

    //Unique Return Label
    int InstanceNum = 0;
    std::string Label = _FileName + '.' + FuncName + "$ret." + std::to_string(InstanceNum);
    if (_LabelMap.count(Label) != 0) {
        InstanceNum = _LabelMap[Label];
        Label = _FileName + '.'  + FuncName + "$ret." + std::to_string(InstanceNum);
        ++_LabelMap[Label];
    }
    else {
        _LabelMap[Label] = 1;
    }

    PushFrame(Label, NumOfArgs);
    WriteGoTo(FuncName);
    WriteLabel(Label);

#ifdef DEBUG
    _OutputFile << "// END OF WriteCall COMMAND\n";
#endif
}

//TODO:: The core issue with the function stack is in this function.
// Fix asap
void VMCodeWriter::WriteReturn()
{
#ifdef DEBUG
    std::cout << "WriteReturn: "  << '\n';
    _OutputFile << "// WriteReturn \n";
#endif
    // Get the frame info

    //Store return address 

    _OutputFile
        << "@5\n"
        << "D=A\n"
        << "@LCL\n"
        << "A=M-D\n" // get return address, address
        << "D=M\n" // go to return address, address
        << "@R14\n"
        << "M = D\n"; // store return address value in a temp register

    //Reposition return value
    // Pops a value of the stack and sets the value of *ARG to it
    _OutputFile <<
        "@SP\n" <<
        "M=M-1\n" <<
        "A=M\n" <<
        "D=M\n" <<
        "@ARG\n" <<
        "A=M\n" <<
        "M=D\n";



    //Reposition stack pointer
    _OutputFile << "@ARG\n";
    _OutputFile << "D = M+1\n";
    _OutputFile << "@SP\n";
    _OutputFile << "M = D\n";

    //Restore THAT value for caller
    _OutputFile << "@LCL\n";
    _OutputFile << "A=M-1\n";
    _OutputFile << "D=M\n";
    _OutputFile << "@THAT\n";
    _OutputFile << "M = D\n";

    //Restore THIS value for caller

    _OutputFile 
    << "@2\n"
    << "D=A\n"
    << "@LCL\n"
    << "A=M-D\n"
    << "D=M\n"
    << "@THIS\n"
    << "M = D\n";

    //Restore ARG value for caller
    _OutputFile 
    << "@3\n"
    << "D=A\n"
    << "@LCL\n"
    << "A=M-D\n"
    << "D=M\n"
    << "@ARG\n"
    << "M = D\n";

    //Restore LCL value for caller
    _OutputFile 
    << "@4\n"
    << "D=A\n"
    << "@LCL\n"
    << "A=M-D\n"
    << "D=M\n"
    << "@LCL\n"
    << "M = D\n";

    //Go To Return Address
    _OutputFile << "@R14\n"
    << "A=M\n"
    << "0;JMP\n";

#ifdef DEBUG
    _OutputFile << "// END OF WriteReturn COMMAND\n";
#endif
}


void VMCodeWriter::WriteDynamic(EVMCommandType Command, std::string Segment, int Index) {
    
    if (_SegmentMap.count(Segment) == 0) {
        std::cerr << "VMCodeWrite::WriteDynamic::INVALID_SEGMENT_COMMAND " << Segment << "\n";
        return;
    }

    //This output works for registers which points to a location
    std::string SegmentDest = (std::strcmp(Segment.c_str(), "this") == 0 || std::strcmp(Segment.c_str(), "that") == 0
                                || std::strcmp(Segment.c_str(), "argument") == 0 || std::strcmp(Segment.c_str(), "local") == 0) ? "M" : "A";

    if (std::strcmp(Segment.c_str(), "pointer") == 0) {
        switch (Index) {
        case 0:
            Segment = "this";
            break;
        case 1:
            Segment = "that";
            Index = 0;
            break;
        default:
            std::cerr << "VMCODEWRITER::WritePushPop::UNDEFINED INDEX FOR POINTER\n";
        }
    }

    else if (std::strcmp(Segment.c_str(), "static") == 0) {
        std::stringstream StringStream;
        StringStream << _FileName << '.' << Segment.c_str() << '.' << Index;
        std::string StaticID = StringStream.str();

        if (_SegmentMap.count(StaticID) == 0) {
            if (_StaticCounter > 240) {
                std::cerr << "VMCODEWRITER::WriteDynamic::_StaticCounter Out Of Range -- Too Many Static Variables!!";
            }
            _SegmentMap[StaticID] = _FileName + '.' +  std::to_string(_StaticCounter++);
        }
        Segment = StaticID;
        Index = 0;

    }

    
    switch (Command) {
    case C_POP:
    #ifdef DEBUG
        std::cout << "DYNAMIC POP\n";
        _OutputFile << "// DYNAMIC POP COMMAND " << Segment << " " << Index << "\n";
    #endif

        
        //This output only works for temp segments
        _OutputFile <<
            "@SP\n"
            "M=M-1\n"
            "@" << _SegmentMap[Segment] << "\n" <<
            "D =" << SegmentDest << "\n" <<
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
            "M=D\n"; // Store top of stack value into dest address
    break;

    case C_PUSH:
    #ifdef DEBUG
        _OutputFile << "// DYNAMIC PUSH COMMAND " << Segment << " " << Index << "\n";
    #endif
        //This output works for registers which points to a location

        //This output only works for temp segments
        _OutputFile <<
            "@" << _SegmentMap[Segment] << "\n" << // Access segment
            "D =" << SegmentDest << "\n" << // Store segment address
            "@" << Index << "\n" << // Access index
            "A = D+A\n" << // Increment index value By segment value and go to that address
            "D = M\n" << // Store the value in that  address to the data register
            "@SP\n" << // Access stack pointer
            "A = M\n" << // Go to top of stack
            "M = D\n" <<                // Store data register value into top of stack
            "@SP\n" <<
            "M = M+1\n";
    break;

    }
}










void VMCodeWriter::Close()
{
    _OutputFile.close();
    _LoopCounter = 0;
    _StaticCounter = 16;
}

