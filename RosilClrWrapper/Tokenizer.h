#pragma once

#include "../RosilCore/Src/Tokenizer.h"
#include "../RosilCore/Src/TokenCode.h"

using System::Collections::Generic::IEnumerable;
using System::Collections::Generic::List;
using System::String;

namespace RosilClrWrapper
{
	public ref struct Token
	{
		RosilCore::TokenCode code;
		int line;
		int startChar;
		int length;
		
		Token(const RosilCore::TokenCode code, const int line, const int startChar, const int length):
			code(code), line(line), startChar(startChar), length(length)
		{}
		
		static Token^ ConvertCoreToken(const RosilCore::Token& token)
		{
			return gcnew Token(token.code, token.line, token.startChar, token.length);
		}
	};

	public ref struct Error
	{
		int code;
		String^ message; // TODO fill
		int line;
		int startChar;
		int length;
		
		Error(const int code, String^ message, const int line, const int startChar, const int length):
			code(code), message(nullptr), line(line), startChar(startChar), length(length)
		{}
		
		static Error^ ConvertCoreError(const RosilCore::Error& error)
		{
			return gcnew Error(error.code, nullptr, error.line, error.startChar, error.length);
		}
	};
	
	public ref class Tokenizer
	{
		RosilCore::Tokenizer* _nativeTokenizer;
		List<Token^>^ _tokens;
		List<Error^>^ _errors;
		
	public:
		Tokenizer();
		~Tokenizer();
		
		// FIXME: the re-encoded std::string is converted into a stream (2nd copy!) and then copied into a buffer (3rd copy!),
		// the script should be passed as a stream but .net stream encapsulation into std::basic_istream is not straight forward;
		// or a proper Run version on string without buffer could be made.
		/**
		 * \brief Run the tokenizer on a script
		 * \param script rosil script to tokenize, the string is copied into a std::string with different encoding
		 * \return run finished without error ?
		 */
		bool Run(System::String^ script);
		
		property IEnumerable<Token^>^ Tokens { IEnumerable<Token^>^ get(); }
		property IEnumerable<Error^>^ Errors { IEnumerable<Error^>^ get(); }
	};

}
