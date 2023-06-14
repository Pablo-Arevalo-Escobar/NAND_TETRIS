#include "VMCodeWriter.h"

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
}

void VMCodeWriter::WritePushPop(EVMCommandType Command, const char* Segment, int Index)
{
}

void VMCodeWriter::Close()
{
}
