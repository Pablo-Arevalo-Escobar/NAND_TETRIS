#pragma once
#include<string>
#include<fstream>

namespace Jack {

enum ETokenType { KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST, NILL };
enum EKeyWord {
    CLASS, METHOD, FUNCTION, CONSTRUCTOR, INT, BOOLEAN, CHAR, VOID, VAR, CAR, STATIC,
    FIELD, LET, DO, IF, ELSE, WHILE, RETURN, TRUE, FALSE, KNILL, THIS
};

class JackTokenizer
{
public:
    JackTokenizer(const char* InputFilePath);
    bool HasMoreTokens();
    void Advance();
    char Symbol();
    ETokenType TokenType();
    EKeyWord KeyWord();
    std::string Identifier();
    int IntVal();
    std::string StringVal();
private:
    std::string ReadToken();
    std::string ProcessStringLiteral();
    void ProcessBlockComment();
    void ProcessComment();
private:
    std::fstream _File;
    std::string _CurrentToken;
    ETokenType _CurrentTokenType;
    EKeyWord _CurrentKeyWord;
};


}

