#ifndef STACKLANG_LEXER_HPP
#define STACKLANG_LEXER_HPP

#include "lexer/token.hpp"
#include "lexer/location.hpp"
#include "lexer/lexer_interface.hpp"

namespace stacklang
{
    class Lexer : public ILexer
    {
    public:
        explicit Lexer(std::string_view code_);
        Token NextToken() override;
    private:
        void handlerUnexpectedSymbol(Location location_);
        void skipError();
        Token handleIdentifier();
        Token handleConstant();
        Token handleNewLine();
    private:
        std::string m_code;
        std::string m_buffer;
        std::size_t m_currentCharIdx;
        std::size_t m_currentLine;
    };
}

#endif //STACKLANG_LEXER_HPP
