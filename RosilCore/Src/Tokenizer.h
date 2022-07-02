#pragma once

#include <list>
#include <iostream>
#include "TokenCode.h"
#include "LibExport.h"

// TODO: clean-up dll interface to remove std
#pragma warning(disable: 4251)  // std::list, std::string needs to have dll-interface

namespace RosilCore
{
    struct DLLEXPORT Token
    {
        TokenCode code;
        std::string value;
        int line;

        Token(const TokenCode code, std::string value, const int line) :
            code(code),
            value(std::move(value)),
            line(line)
        {}

        friend std::ostream& operator<< (std::ostream& out, const Token& t);
    };

    inline std::ostream& operator << (std::ostream& out, const Token& t)
    {
        return out << "(" << t.code << " \"" << t.value << "\")";
    }

    struct DLLEXPORT Error
    {
        int code;
        std::string message;
        int line;

        Error(const int code, std::string message, const int line) :
            code(code),
            message(std::move(message)),
            line(line)
        {}

        friend std::ostream& operator<< (std::ostream& out, const Error& e);
    };

    inline std::ostream& operator << (std::ostream& outs, const Error& e)
    {
        return outs << e.message << " l." << e.line << "(" << e.code << ")";
    }

    class DLLEXPORT Tokenizer
    {
        static const int _lineBufferSize = 1024; // currently max line length

        int _lineIndex; // current file line, used for debugging the scripts
        bool _lastLineOpenedContext; // flag making sure indent is increased after opening context

        // Rosil script tokenized ready for Lexer, initialized after successful Run
        std::list<Token> _tokens;

        // list of errors that happened while running. filled if Run returned false
        std::list<Error> _errors;

        /**
         * \brief Create the next token out of the string. update the char_cursor. change *c to 0 if comment.
         * \param charCursor reference to a pointer on the first character of the new token to append.
         */
        void PushNextToken(char*& charCursor);

        /**
         * \brief Count tabulation and 4-spaces, (check comments for empty line case)
         * \param charCursor line start cursor, updated to first token
         * \return number of indentation
         */
        int CountIndent(char*& charCursor);

        /**
         * \brief "[0-9]."
         * \param c character checked
         * \return true if character is part of a number (5, 0.14)
         */
        static bool CharIsNumber(const char c);

        /**
         * \brief "[a-z][A-Z][0-9]_"
         * \param c character checked
         * \return true if character is part of a label (class, _variableName)
         */
        static bool CharIsLabel(const char c);

    public:
        Tokenizer();

        const std::list<Token>& GetTokens();
        const std::list<Error>& GetErrors();

        /**
         * \brief split a file or a text into a list of string
         * \param stream open stream on a Rosil script
         * \return success ? check errors if false
         */
        bool  Run(std::istream& stream);
        bool  Run(std::string& string);
    };
}
// TODO: clean-up dll interface to remove std
#pragma warning(default: 4251)  // std::list, std::string needs to have dll-interface
