#include "Tokenizer.h"
#include "../RosilCore/Src/Tokenizer.h"
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

using namespace RosilClrWrapper;
using namespace msclr::interop;

Tokenizer::Tokenizer()
{
    _native_tokenizer = new RosilCore::Tokenizer();
}

Tokenizer::~Tokenizer()
{
    delete _native_tokenizer;
}

bool Tokenizer::Run(System::String^ script)
{
    // convert rosil script from C# string to C++ string before calling native_tokenizer->Run
    marshal_context^ context = gcnew marshal_context();
    std::string stdScript = context->marshal_as<std::string>(script);

    bool tokenizer_sucess = _native_tokenizer->Run(stdScript);

    delete context;
    return tokenizer_sucess;
}

