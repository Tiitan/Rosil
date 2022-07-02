#include "Tokenizer.h"
#include <sstream>

using namespace RosilCore;

static const std::unordered_map<std::string,TokenCode> wordTokenTable = {
    {"true", TokenCode::TrueValue},
    {"false", TokenCode::FalseValue},
    
    {"and", TokenCode::And},
    {"or", TokenCode::Or},
    {"xor", TokenCode::Xor},
    
    {"if", TokenCode::If},
    {"while", TokenCode::While},
    {"else", TokenCode::Else},
    {"for", TokenCode::For},
    {"switch", TokenCode::Switch},
    {"case", TokenCode::Case},
    {"get", TokenCode::Get},
    {"set", TokenCode::Set},
    {"return", TokenCode::Return},
    
    {"bool", TokenCode::BoolType},
    {"int", TokenCode::IntType},
    {"float", TokenCode::FloatType},
    {"string", TokenCode::StringType},
    {"str", TokenCode::StringType},
    {"class", TokenCode::ClassType},
    {"struct", TokenCode::StructType},
    {"enum", TokenCode::EnumType},
    {"list", TokenCode::ClassType},
    {"dict", TokenCode::ClassType},
    {"import", TokenCode::Import},
    
    {"public", TokenCode::Public},
    {"private", TokenCode::Private},
    {"static", TokenCode::Static},
    {"const", TokenCode::Const},
};

static const std::unordered_map<std::string,TokenCode> twoCharTokenTable = {
    {"&&", TokenCode::And},
    {"||", TokenCode::Or},
    {"<=", TokenCode::InferiorOrEquals},
    {">=", TokenCode::SuperiorOrEquals},
    {"==", TokenCode::Equals},
    {"!=", TokenCode::NotEquals},
    
    {"+=", TokenCode::ModAssign},
    {"-=", TokenCode::SubAssign},
    {"/=", TokenCode::DivAssign},
    {"*=", TokenCode::MultAssign},
    {"%=", TokenCode::ModAssign},
    {"++", TokenCode::Increment},
    {"--", TokenCode::Decrement},
};

static const std::unordered_map<char,TokenCode> charTokenTable = {
    {'+', TokenCode::Add},
    {'-', TokenCode::Sub},
    {'/', TokenCode::Divide},
    {'*', TokenCode::Multiply},
    {'%', TokenCode::Modulo},

    {'^', TokenCode::Xor},
    {'<', TokenCode::Inferior},
    {'>', TokenCode::Superior},
    {'!', TokenCode::Invert},
    
    {'=', TokenCode::Assign},
    
    {'.', TokenCode::Accessor},
    {',', TokenCode::ParameterSeparator},
    {';', TokenCode::StatementSeparator},
    {':', TokenCode::OpenContext},
    {'(', TokenCode::OpenParenthesis},
    {')', TokenCode::CloseParenthesis},
    {'[', TokenCode::OpenBracket},
    {']', TokenCode::CloseBracket},
    {'{', TokenCode::OpenBrace},
    {'}', TokenCode::CloseBrace},
};

Tokenizer::Tokenizer():
    _lineIndex(0),
    _lastLineOpenedContext(false)
{
    
}

void Tokenizer::PushNextToken(char*& charCursor)
{
    // Go to next token
    while (*charCursor == '\t' || *charCursor == ' ')
        charCursor++;
    
    // Rest of line is commented out ?
    if (*charCursor == '/' && *(charCursor + 1) == '/')
    {
        *charCursor = 0;
        return;
    }
    
    // Check for a 2 char token (&&, ==, ...)
    const std::string nextTwoChar(charCursor, 2);
    const auto twoCharIt = twoCharTokenTable.find(nextTwoChar);
    if (twoCharIt != twoCharTokenTable.end())
    {
        _tokens.emplace_back(twoCharIt->second, nextTwoChar, _lineIndex);
        charCursor += 2;
        return;
    }

    // Check for a single char token (+, !, =, ...)
    const auto charIt = charTokenTable.find(*charCursor);
    if (charIt != charTokenTable.end())
    {
        if (charIt->second == TokenCode::OpenContext)
            _lastLineOpenedContext = true;
        
        _tokens.emplace_back(charIt->second, std::string(charCursor, 1), _lineIndex);
        charCursor++;
        return;
    }

    // Check for a string ("xxx")
    if (*charCursor == '"')
    {
        charCursor++;
        int stringCharCount = 0;
        while (*(charCursor + stringCharCount) && *(charCursor + stringCharCount) != '"')
            ++stringCharCount;
        // TODO: escape \" and replace with " in token 
        _tokens.emplace_back(TokenCode::Text, std::string(charCursor, stringCharCount), _lineIndex);
        
        if (!*(charCursor + stringCharCount))
            _errors.emplace_back(0, "Missing end of string.", _lineIndex);

        charCursor += stringCharCount + 1;
        return;
    }

    // Check for a number (12, 18.05)
    if (CharIsNumber(*charCursor))
    {
        TokenCode code = TokenCode::IntValue;
        int stringCharCount = 0;
        while (*(charCursor + stringCharCount) && CharIsNumber(*(charCursor + stringCharCount)))
        {
            ++stringCharCount;
            if (*(charCursor + stringCharCount) == '.')
            {
                if (code == TokenCode::FloatValue)
                    _errors.emplace_back(0, "Invalid number (multiple '.').", _lineIndex);
                code = TokenCode::FloatValue;
            }
        }
        
        _tokens.emplace_back(code, std::string(charCursor, stringCharCount), _lineIndex);

        charCursor += stringCharCount;
        return;
    }

    // Check for a label (class, variable, ...)
    if (CharIsLabel(*charCursor))
    {
        // Build label
        int stringCharCount = 0;
        while (*(charCursor + stringCharCount) && CharIsLabel(*(charCursor + stringCharCount)))
            ++stringCharCount;
        const std::string label(charCursor, stringCharCount);

        // Look for matching keyword
        TokenCode code = TokenCode::Label;
        const auto wordIt = wordTokenTable.find(label);
        if (wordIt != wordTokenTable.end())
            code = wordIt->second;
        // TODO replace Else with ElseIf if code==If and last token==Else
        _tokens.emplace_back(code, label, _lineIndex);
        charCursor += stringCharCount;
        return;
    }

    _errors.emplace_back(0, "Invalid character.", _lineIndex);
    ++charCursor;
}

bool Tokenizer::CharIsNumber(const char c)
{
    return c >= '0' && c <= '9' ||
        c == '.';
}

bool Tokenizer::CharIsLabel(const char c)
{
    return c == '_' ||
        c >= 'a' && c <= 'z' ||
        c >= 'A' && c <= 'Z' ||
        c >= '0' && c <= '9';
}

int Tokenizer::CountIndent(char*& charCursor)
{
    int indent = 0;
    int space  = 0;
    while (*charCursor == '\t' || *charCursor == ' ')
    {
        if (*charCursor == ' ')
            ++space;
        if (*charCursor == '\t' || space == 4)
            ++indent;
        if (space == 4)
            space = 0;
        ++charCursor;
    }
    
    if (*charCursor == '/' && *(charCursor + 1) == '/') // line is fully commented out ?
    {
        *charCursor = 0;
        return -1; // unused
    }
    
    if (space != 0)
        _errors.emplace_back(0, "Indenting spaces not multiple of 4.", _lineIndex);
    return indent;
}

bool Tokenizer::Run(std::istream& stream)
{
    _lineIndex = 0;
    char line[_lineBufferSize];
    int lastIndentCount = 0;
    while (stream.getline(line, _lineBufferSize))
    {
        ++_lineIndex;
        if (stream.gcount() > _lineBufferSize - 1)
            _errors.emplace_back(0, "Lines above 1024 character not yet supported", _lineIndex);

        char* c = line;

        // Close context using file indent
        const int currentIndentCount = CountIndent(c);
        if (*c) // skip empty and commented line
        {
            
            int indentChange = currentIndentCount - lastIndentCount;
            // TODO create context if indentChange = 1 && _lastLineOpenedContext == false instead of error
            if (indentChange > 1 || indentChange == 1 && _lastLineOpenedContext != true)
                _errors.emplace_back(0, "Indent error.", _lineIndex);
            while (indentChange++ < 0)
                _tokens.emplace_back(TokenCode::CloseContext, "", _lineIndex);
            lastIndentCount = currentIndentCount;
            _lastLineOpenedContext = false;
            // TODO: insert CloseContext if indentChange==0 && last token != NewContext, inline context like: while x > 0: x--; do()
            // TODO replace _lastLineOpenedContext bool with count to support for x: if y:
        }

        while (*c)
            PushNextToken(c);
    }
    
    return _errors.empty();
}

bool RosilCore::Tokenizer::Run(std::string& string)
{
    std::istringstream stringStream = std::istringstream(string);
    return Run(stringStream);
}

const std::list<Token>& Tokenizer::GetTokens()
{
    return _tokens;
}

const std::list<Error>& Tokenizer::GetErrors()
{
    return _errors;
}
