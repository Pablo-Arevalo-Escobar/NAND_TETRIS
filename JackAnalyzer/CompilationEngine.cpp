#include "CompilationEngine.h"
#include <iostream>
#include <unordered_set>


//TO DO  :: WORK ON TERM COMPILATION
using namespace Jack;
std::unordered_set<char> _OperationSet{
    '+','-','*','/','&','|','<','>','='
};
std::unordered_set<std::string> _KeywordConstant{
    "true", "false", "null", "this"
};

CompilationEngine::CompilationEngine(const char* InputFilePath, const char* OutputFilePath) : _Tokenizer(InputFilePath) {
    _OutFile = std::ofstream(OutputFilePath);
    if (!_OutFile) {
        std::cerr << "Error creating file!" << std::endl;
        return;
    }
}

CompilationEngine::~CompilationEngine()
{
    _OutFile.close();
}

void CompilationEngine::CompileClass()
{
    _Tokenizer.Advance();
    _OutFile << "<class>\n";
    _IndentLevel += 1;
    //DEAL WITH CLASS LOGIC
    ValidateKeyword(CLASS);
    ValidateIdentifier();
    ValidateSymbol('{');
    while(LookAheadKeyword(STATIC) || LookAheadKeyword(FIELD)) 
        CompileClassVarDec();
    while(LookAheadKeyword(CONSTRUCTOR) || LookAheadKeyword(FUNCTION) || LookAheadKeyword(METHOD))
        CompileSubroutine();  
    ValidateSymbol('}');
    _IndentLevel -= 1;
    _OutFile << "</class>\n";
    if (_Error) 
        std::cerr << "INVALID COMPILATION\n";
    _OutFile.close();
}

void CompilationEngine::CompileClassVarDec()
{
    Write("classVarDec", true);
    _IndentLevel += 1;

    if (ValidateKeyword(STATIC) || ValidateKeyword(FIELD)) {
        ValidateType();
        ValidateIdentifier(); // varName
        while (ValidateSymbol(',')) {
            ValidateIdentifier(); // var Name
        }
        ValidateSymbol(';');
    }

    _IndentLevel -= 1;
    Write("classVarDec", false);
}

void CompilationEngine::CompileSubroutine()
{
    Write("subroutineDec", true);
    _IndentLevel += 1;
    if (ValidateKeyword(CONSTRUCTOR) || ValidateKeyword(FUNCTION) || ValidateKeyword(METHOD)) {
        if (!(ValidateType() || ValidateKeyword(VOID)))
            _Error = true;
        ValidateIdentifier(); // Subroutine Name
        ValidateSymbol('(');
        CompileParameterList();
        ValidateSymbol(')');
        CompileSubroutineBody();
    }
    _IndentLevel -= 1;
    Write("subroutineDec", false);
}

void CompilationEngine::CompileParameterList()
{
    Write("parameterList", true);
    ++_IndentLevel;
    if (ValidateType()) {
        ValidateIdentifier();
        while (ValidateSymbol(',') ) {
            ValidateType();
            ValidateIdentifier();
        }
    }
    --_IndentLevel;
    Write("parameterList", false);
}

void CompilationEngine::CompileSubroutineBody()
{
    Write("subroutineBody", true);
    _IndentLevel += 1;
    ValidateSymbol('{');
    while (LookAheadKeyword(VAR)) CompileVarDec();
    CompileStatements();
    ValidateSymbol('}');
    _IndentLevel -= 1;
    Write("subroutineBody", false);

}

void CompilationEngine::CompileVarDec()
{
    Write("varDec", true);
    _IndentLevel += 1;
    ValidateKeyword(VAR);
    ValidateType();
    ValidateIdentifier();
    while (ValidateSymbol(','))
        ValidateIdentifier();
    ValidateSymbol(';');
    _IndentLevel -= 1;
    Write("varDec", false);
}

void CompilationEngine::CompileStatements()
{
    Write("statements", true);
    ++_IndentLevel;
    bool DoLoop = true;
    while (DoLoop) {
        switch (_Tokenizer.KeyWord())
        {
        case LET:
            CompileLet();
            break;
        case IF:
            CompileIf();
            break;
        case WHILE:
            CompileWhile();
            break;
        case DO:
            CompileDo();
            break;
        case RETURN:
            CompileReturn();
            break;
        default:
            DoLoop = false;
            break;
        }
    }
    --_IndentLevel;
    Write("statements", false);
}

void CompilationEngine::CompileLet()
{
    Write("letStatement", true);
    _IndentLevel += 1;
    ValidateKeyword(LET);
    ValidateIdentifier();

    if (ValidateSymbol('[')) {
        CompileExpression();
        ValidateSymbol(']');
    }

    ValidateSymbol('=');
    CompileExpression();
    ValidateSymbol(';');
    _IndentLevel -= 1;
    Write("letStatement", false);
}

void CompilationEngine::CompileIf()
{
    Write("ifStatement", true);
    _IndentLevel += 1;
    ValidateKeyword(IF);
    ValidateSymbol('(');
    CompileExpression();
    ValidateSymbol(')');
    ValidateSymbol('{');
    CompileStatements();
    ValidateSymbol('}');
    if (ValidateKeyword(ELSE)) {
        ValidateSymbol('{');
        CompileStatements();
        ValidateSymbol('}');
    }
    _IndentLevel -= 1;
    Write("ifStatement", false);
}

void CompilationEngine::CompileWhile()
{
    Write("whileStatement", true);
    _IndentLevel += 1;
    ValidateKeyword(WHILE);
    ValidateSymbol('(');
    CompileExpression();
    ValidateSymbol(')');
    ValidateSymbol('{');
    CompileStatements();
    ValidateSymbol('}');
    _IndentLevel -= 1;
    Write("whileStatement", false);
}

void CompilationEngine::CompileDo()
{
    Write("doStatement", true);
    _IndentLevel += 1;
    ValidateKeyword(DO);
    if(ValidateIdentifier())
        SubroutineCall();
    ValidateSymbol(';');
    _IndentLevel -= 1;
    Write("doStatement", false);
}

void CompilationEngine::CompileReturn()
{
    Write("returnStatement", true);
    _IndentLevel += 1;
    ValidateKeyword(RETURN);
    if (!ValidateSymbol(';')) {
        CompileExpression(); // optional
        ValidateSymbol(';');
    }
    _IndentLevel -= 1;
    Write("returnStatement", false);
}

void CompilationEngine::CompileExpression()
{
    Write("expression", true);
    ++_IndentLevel;
    CompileTerm();
    while (ValidateOperation()) {
        CompileTerm();
    }
    --_IndentLevel;
    Write("expression", false);

}

void CompilationEngine::CompileTerm()
{ 
    Write("term", true);
    ++_IndentLevel;
    if (_Tokenizer.TokenType() == INT_CONST || _Tokenizer.TokenType() == STRING_CONST || _KeywordConstant.count(_Tokenizer.StringVal()) > 0) {
        WriteToken(_Tokenizer.TokenType());
        _Tokenizer.Advance();
    }
    else if (ValidateIdentifier()) {
        if (ValidateSymbol('[')) {
            CompileExpression();
            ValidateSymbol(']');
        }
        else{
            SubroutineCall();
        }
    }
    else if(ValidateSymbol('(')) {
        CompileExpression();
        ValidateSymbol(')');
    }
    else if (ValidateSymbol('-') || ValidateSymbol('~')) {
        CompileTerm();
    }
    --_IndentLevel;
    Write("term", false);

}

int CompilationEngine::CompileExpressionList()
{
    Write("expressionList", true);
    ++_IndentLevel;
    if (LookAheadTerm()) {
        CompileExpression();
        while (ValidateSymbol(',')) {
            CompileExpression();
        }
    }
  
    --_IndentLevel;
    Write("expressionList", false);
    return 0;
}

bool CompilationEngine::LookAheadTerm() {
    if (_Tokenizer.TokenType() == INT_CONST || _Tokenizer.TokenType() == STRING_CONST 
        || _KeywordConstant.count(_Tokenizer.StringVal()) > 0
        || _Tokenizer.TokenType() == IDENTIFIER || _Tokenizer.Symbol() == '('
        || _Tokenizer.Symbol() == '-' || _Tokenizer.Symbol() == '~'
       )
    {
        return true;
    }
    return false;
}

void CompilationEngine::WriteToken(ETokenType ExpectedType)
{
    for (int i = 0; i < 2 * _IndentLevel; ++i) {
        _OutFile << ' ';
    }
    std::string Type;
    std::string Token;
    switch (_Tokenizer.TokenType()) {
    case KEYWORD:
        Type = "keyword";
        Token = _Tokenizer.StringVal();
        break;
    case SYMBOL:
        Type = "symbol";
        Token = ExtractSymbol(_Tokenizer.Symbol());
        break;
    case IDENTIFIER:
        Type = "identifier";
        Token = _Tokenizer.Identifier();
        break;
    case STRING_CONST:
        Type = "stringConstant";
        Token = _Tokenizer.StringVal();
        Token.erase(Token.begin());
        Token.erase(Token.end() - 1);
        break;
    case INT_CONST:
        Type = "integerConstant";
        Token = _Tokenizer.StringVal();
        break;
    default:
        Type = "NILL";
        break;
    }
    _OutFile
        << "<" << Type << "> "
        << Token << " </"
        << Type << ">\n";
}



std::string CompilationEngine::ExtractSymbol(char Token) {
    switch (Token) {
    case '<':
        return "&lt;";
    case '>':
        return "&gt;";
    case '/"':
        return "&quot;";
    case '&':
        return "&amp;";
    default:
        return std::string(1, Token);
    }
}

bool CompilationEngine::LookAheadKeyword(EKeyWord Expected) {
    if (_Tokenizer.KeyWord() == Expected)
        return true;
    return false;
}

bool Jack::CompilationEngine::ValidateOperation()
{
    if (_OperationSet.count(_Tokenizer.Symbol()) > 0) {
        WriteToken(SYMBOL);
        _Tokenizer.Advance();
        return true;
    }
    return false;
}



bool CompilationEngine::ValidateKeyword(EKeyWord ExpectedWord)
{
    if (_Tokenizer.KeyWord() != ExpectedWord)
        return false;
    WriteToken(KEYWORD);
    _Tokenizer.Advance();
    return true;
}

bool CompilationEngine::ValidateSymbol(char Symbol)
{
    if (_Tokenizer.Symbol() != Symbol)
        return false;

    WriteToken(SYMBOL);
    _Tokenizer.Advance();
    return true;
}

void Jack::CompilationEngine::Write(std::string s, bool bStart)
{
    for (int i = 0; i < 2 * _IndentLevel; ++i) {
        _OutFile << ' ';
    }
    if (bStart) _OutFile << "<" << s << ">\n";
    else _OutFile << "</" << s << ">\n";
}

bool CompilationEngine::ValidateIdentifier() {
    if (_Tokenizer.TokenType() != IDENTIFIER)
        return false;
    WriteToken(IDENTIFIER);
    _Tokenizer.Advance();
    return true;
}
void Jack::CompilationEngine::SubroutineCall()
{
    if (ValidateSymbol('(')) {
        CompileExpressionList();
        ValidateSymbol(')');
    }
    else if(ValidateSymbol('.'))
    {
        ValidateIdentifier();
        ValidateSymbol('(');
        CompileExpressionList();
        ValidateSymbol(')');
    }
}
bool CompilationEngine::ValidateType()
{
    if (!(ValidateKeyword(INT) || ValidateKeyword(CHAR) || ValidateKeyword(BOOLEAN) || ValidateIdentifier())) {
        _Error = true;
        return false;
    }
    return true;
}