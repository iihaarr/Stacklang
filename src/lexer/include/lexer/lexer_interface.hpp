#ifndef STACKLANG_LEXER_INTERFACE_HPP
#define STACKLANG_LEXER_INTERFACE_HPP

#include "lexer/token.hpp"

namespace stacklang
{
    struct ILexer
    {
        virtual Token NextToken() = 0;
        virtual ~ILexer() = default;
    };

    std::unique_ptr<ILexer> GetLexer(std::string_view code_);
}

#endif //STACKLANG_LEXER_INTERFACE_HPP
