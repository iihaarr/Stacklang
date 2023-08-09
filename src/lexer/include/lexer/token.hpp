#ifndef STACKLANG_TOKEN_HPP
#define STACKLANG_TOKEN_HPP

#include <memory>
#include <vector>
#include <string>
#include "tokens.hpp"
#include "lexer/location.hpp"

namespace stacklang
{
    struct Token
    {
        enum class Type
        {
#define CREATE_TOKEN(name_, _) name_,
#define CREATE_TOKEN_KEYWORD(name, value) CREATE_TOKEN(name, value)
            TOKEN_LIST_KEYWORD
            TOKEN_LIST
#undef CREATE_TOKEN
#undef CREATE_TOKEN_KEYWORD
        };

        Token(Type type_, Location location_);
        Token(Type type_, Location location_, const std::string& text_);

        Token(Token&&) noexcept = default;
        Token(const Token&) = delete;
        Token& operator=(const Token&) = delete;
        Token& operator=(Token&&) = default;

        Type m_type;
        Location m_location;
        std::unique_ptr<std::string> m_text;
    };

    template<typename... Args>
    Token MakeToken(Args&&... args_)
    {
        return Token(std::forward<Args>(args_)...);
    }
}

#endif //STACKLANG_TOKEN_HPP

