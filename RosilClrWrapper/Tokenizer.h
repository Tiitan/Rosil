#pragma once

#include "../RosilCore/Src/Tokenizer.h"

namespace RosilClrWrapper
{
	public ref class Tokenizer
	{
		RosilCore::Tokenizer* _native_tokenizer;

	public:
		Tokenizer();
		~Tokenizer();
		bool Run(System::String^ script);
		// TODO: wrap get_tokens and get_errors
	};

}
