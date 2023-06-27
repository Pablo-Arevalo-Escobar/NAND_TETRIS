#pragma once
#include "VMParser.h"
#include <map>

class VMCodeWriter
{
public:
    VMCodeWriter(const char* OutputFile);
    void WriteArithmetic(const char* Command);
    void WritePushPop(EVMCommandType Command, std::string Segment, int Index);
    void Close();
private:
    void WriteDynamic(EVMCommandType Command,std::string Segment, int Index);
private:
    std::ofstream _OutputFile;
    int _StaticCounter = 16;
    int _LoopCounter = 0;
};

