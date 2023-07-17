#pragma once
#include "VMParser.h"
/*
* TODO:
*  - Implement Write Label DONE
*  - Implement GoTo Commands and If-Go-To commands DONE
*  
*  - Both label and goto work but it appears that the push/pop operations require some 
*    serious optimization!!!
*/

class VMCodeWriter
{
public:
    VMCodeWriter(const char* OutputFile);
    VMCodeWriter(const char* OutputFile, bool BSysInit);
    void WriteArithmetic(const char* Command);
    void WritePushPop(EVMCommandType Command, std::string Segment, int Index);
    void SetFileName(std::string FileName);
    void WriteLabel(std::string Label);
    void WriteGoTo(std::string GoToLabel);
    void WriteIf(std::string IfGoToLabel);
    void WriteFunction(std::string FuncName, int NumOfParams);
    void WriteCall(std::string FuncName, int NumOfArgs);
    void WriteReturn();

    void Close();
private:
    void PushFrame(std::string Label, int NumOfArgs);
    void WriteDynamic(EVMCommandType Command,std::string Segment, int Index);
private:
    std::string _FileName;
    std::ofstream _OutputFile;
    int _StaticCounter = 16;
    int _LoopCounter = 0;
};

