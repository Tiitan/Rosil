#include <fstream>
#include <iostream>
#include "../../RosilCore/Src/Tokenizer.h" // TODO setup include path, fix to <Tokenizer.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Missing file path." << std::endl;
        return EXIT_FAILURE;
    }
    
    std::ifstream fileStream(argv[1]);
    auto tokenizer = RosilCore::Tokenizer();
    if (tokenizer.Run(fileStream))
    {
        for (auto const &i: tokenizer.GetTokens()) {
            std::cout << i;
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Failed to tokenize:" << std::endl;
        for (auto const &i: tokenizer.GetErrors()) {
            std::cout << i << std::endl;
        }
    }
    
    return EXIT_SUCCESS;
}
