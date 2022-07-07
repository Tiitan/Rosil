#include "Tokenizer.h"
#include "../RosilCore/Src/Tokenizer.h"
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

using namespace RosilClrWrapper;
using namespace msclr::interop;

Tokenizer::Tokenizer():
    _nativeTokenizer(new RosilCore::Tokenizer()),
    _tokens(nullptr),
    _errors(nullptr)
{}

Tokenizer::~Tokenizer()
{
    delete _nativeTokenizer;
}

IEnumerable<Token^>^ Tokenizer::Tokens::get()
{
    if (_tokens == nullptr)
    {
        _tokens = gcnew List<Token^>();
        for each(const RosilCore::Token& coreToken in _nativeTokenizer->GetTokens())
            _tokens->Add(Token::ConvertCoreToken(coreToken));
    }
    return _tokens;
}

IEnumerable<Error^>^ Tokenizer::Errors::get()
{
    if (_errors == nullptr)
    {
        _errors = gcnew List<Error^>();
        for each(const RosilCore::Error& coreError in _nativeTokenizer->GetErrors())
            _errors->Add(Error::ConvertCoreError(coreError));
    }
    return _errors;
}

bool Tokenizer::Run(System::String^ script)
{
    // clear cached results in case of a previous run
    delete _tokens;
    _tokens = nullptr;
    delete _errors;
    _errors = nullptr;
    
    // convert rosil script from C# string to C++ string before calling native_tokenizer->Run
    marshal_context^ context = gcnew marshal_context();
    std::string stdScript = context->marshal_as<std::string>(script);

    const bool tokenizerSuccess = _nativeTokenizer->Run(stdScript);
    
    delete context;
    return tokenizerSuccess;
}
