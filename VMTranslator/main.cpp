#include <iostream>

int main(int ArgCount, char* ArgValues[]) {

    if (ArgCount > 1)
        std::cout << "Hello File! " << ArgValues[1] << "\n";
    else
       std::cout << "Hello World!\n";
    
    std::string TestString = "push constant 17";
    auto pos = TestString.find(' ');
    std::string sub2 = TestString.substr(pos+1);
    std::string sub21 = sub2.substr(0, sub2.find(' '));
    std::string sub22 = sub2.substr(sub2.find(' '));

    std::cout << "String : " << TestString << "\n";
    std::cout << "sub2 " << sub2 << "\n";
    std::cout << "arg1 : " << sub21 << "\n";
    std::cout << "arg2 : " << sub22 << "\n";

    return 1;
}