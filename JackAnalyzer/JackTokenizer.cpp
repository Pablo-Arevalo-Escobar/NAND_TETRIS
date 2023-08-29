#include "JackTokenizer.h"
#include <iostream>
#include <map>
#include<unordered_set>
#include <stack>

using namespace Jack;

std::map<std::string, EKeyWord> _KeywordMap{

    {"class",CLASS}, {"constructor",CONSTRUCTOR} , {"function",FUNCTION} , {"method",METHOD},
    {"field",FIELD}, {"static",STATIC}, {"var",VAR}, {"int",INT}, {"char",CHAR}, {"boolean",BOOLEAN},
    {"void",VOID}, {"true",TRUE}, {"false",FALSE}, {"null",EKeyWord::KNILL}, {"this",THIS}, {"let",LET}, 
    {"do",EKeyWord::DO},{"if",EKeyWord::IF}, {"else",EKeyWord::ELSE}, {"while",EKeyWord::WHILE}, 
    {"return", EKeyWord::RETURN}
};
std::unordered_set<char> _SymbolSet{
    '{', '}','(',')','[',']','.',',',';','+',
    '-','*','/','&','|','<','>','=','~'
};

JackTokenizer::JackTokenizer(const char* InputFilePath)
{
    try {
        _File.open(InputFilePath);
        if (_File.fail()) throw _File.failbit;
    }
    catch (...) {
        std::cerr << "ERROR::VMPARSER::FILE_NOT_FOUND_FAILBIT_" << _File.failbit << std::endl;
    }
    _CurrentKeyWord = EKeyWord::KNILL;
    _CurrentTokenType = ETokenType::NILL;
    _CurrentToken = "";
}

bool JackTokenizer::HasMoreTokens()
{
    if (!_File.eof()) return true;
    return false;
}

void JackTokenizer::Advance()
{
    if (!_File.is_open()) {
        std::cerr << "VMPARSER::CONSTRUCTOR::FILE_NOT_OPEN" << std::endl;
        return;
    }
    if (!HasMoreTokens()) {
        std::cerr << "VMPARSER::ADVANCE::NO_MORE_COMMANDS" << std::endl;
        return;
    }
    do {
        _CurrentToken = ReadToken();
    } while (_CurrentToken.empty() && !_File.eof());
    TokenType();

#ifdef DEBUG
    std::cout << "CURRENT COMMAND :" << _CurrentCommand << "\n";
#endif
}

char JackTokenizer::Symbol()
{
    if (_CurrentTokenType != ETokenType::SYMBOL) {
        std::cerr << "ERROR::TOKENIZER::SYMBOL::INVALID_TOKEN_TYPE\n";
        return -1;
    }
    return _CurrentToken[0];
}

ETokenType JackTokenizer::TokenType()
{
    if (_CurrentToken.empty()) {
        std::cerr << "ERROR::TOKENIZER::TOKENTYPE::TOKEN_EMPTY\n";
        return ETokenType::NILL;
    }
    if (_KeywordMap.count(_CurrentToken) > 0) {
        _CurrentTokenType = KEYWORD;
        return ETokenType::KEYWORD;
    }
    else if (_SymbolSet.count(_CurrentToken[0]) > 0) {
        _CurrentTokenType = SYMBOL;
        return ETokenType::SYMBOL;
    }
    else if (_CurrentToken.find('\"') != std::string::npos) {
        _CurrentTokenType = STRING_CONST;
        return ETokenType::STRING_CONST;
    }
    try {
        int value = stoi(_CurrentToken);
        _CurrentTokenType = INT_CONST;
        return ETokenType::INT_CONST;
    }
    catch (...) {}
    
    if (!isdigit(_CurrentToken[0])) {
        _CurrentTokenType = IDENTIFIER;
        return ETokenType::IDENTIFIER;
    }
    _CurrentTokenType = ETokenType::NILL;
    return ETokenType::NILL;
}

EKeyWord JackTokenizer::KeyWord()
{
    if (_CurrentTokenType != ETokenType::KEYWORD) {
        std::cerr << "ERROR::TOKENIZER::KEYWORD::INVALID_TOKEN_TYPE\n";
        return EKeyWord::KNILL;
    }
    return _KeywordMap[_CurrentToken];
}

std::string JackTokenizer::Identifier()
{
    if (_CurrentTokenType != ETokenType::IDENTIFIER) {
        std::cerr << "ERROR::TOKENIZER::IDENTIFIER::INVALID_TOKEN_TYPE\n";
        return "";
    }
    return _CurrentToken;
}

int JackTokenizer::IntVal()
{
    if (_CurrentTokenType != ETokenType::INT_CONST) {
        std::cerr << "ERROR::TOKENIZER::INTVAL::INVALID_TOKEN_TYPE\n";
        return -1;
    }
    try {
        return stoi(_CurrentToken);
    }
    catch (...) {
        std::cerr << "ERROR::TOKENIZER::INTVAL::INVALID_TOKEN_STRING\n";
        return -1;
    }
}

std::string JackTokenizer::StringVal()
{
    //if (_CurrentTokenType != ETokenType::STRING_CONST) {
    //    std::cerr << "ERROR::TOKENIZER::STRINGVAL::INVALID_TOKEN_TYPE\n";
    //    return "";
    //}
    return _CurrentToken;
}

std::string JackTokenizer::ReadToken()
{
    if (!_File.is_open()) {
        std::cerr << "ERROR::VMPARSER::FILE_NOT_OPEN" << std::endl;
        return "";
    }
    std::string NewToken = "";
    char CurrentChar = ' ';
    while (CurrentChar != '\n' && !_File.eof()) {
         CurrentChar = _File.get();
         if (CurrentChar == '\"') {
             NewToken = ProcessStringLiteral();
             break;
         }
        else if (CurrentChar == '/' && (_File.peek() == '/' || _File.peek() == '*')) {
            (_File.peek() == '*') ? ProcessBlockComment() : ProcessComment();
            CurrentChar = ' ';
            continue;
        }
        else if ( (CurrentChar == ' ' || CurrentChar == '\t') && NewToken.size() == 0) continue;
        else if (CurrentChar == '\n' || CurrentChar == ' ') { break; }
        NewToken += CurrentChar; 
        if (_SymbolSet.count(_File.peek()) != 0 || _SymbolSet.count(CurrentChar)!=0) break;
    }
    if (_File.eof()) return "";
    return NewToken;
}

void JackTokenizer::ProcessBlockComment() {
    while (!_File.eof()) {
        char CurrentChar = _File.get();
        if (CurrentChar == '*' && _File.peek() == '/') {
            _File.get();
            return;
        }
    }
}

void JackTokenizer::ProcessComment() {
    char CurrentChar = '/';
    while (CurrentChar != '\n' && !_File.eof()) {
        CurrentChar = _File.get();
    }
}

std::string JackTokenizer::ProcessStringLiteral()
{
    char CurrentChar = '\"';
    std::string Literal = "\"";
    while (_File.peek() != '\"') {
        Literal += _File.get();
    }
    Literal += _File.get();
    return Literal;
}


