#pragma once
#include "VMParser.h"

class VMCodeWriter
{
public:
    void WriteArithmetic(const char* Command);
    void WritePushPop(EVMCommandType Command, const char* Segment, int Index);
    void Close();
private:
    std::ofstream _OutputFile;
};

