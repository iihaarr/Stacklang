#include "lexer/token.hpp"
#include "lexer/location.hpp"

namespace stacklang
{
    Token::Token(Token::Type type_, Location location_)
        : m_type(type_), m_location(location_), m_text(nullptr)
    {

    }
    Token::Token(Token::Type type_, Location location_, const std::string& text_)
        : m_type(type_), m_location(location_), m_text(std::make_unique<std::string>(text_))
    {

    }
    const char* GetTokenDescription(Token::Type type_)
    {
        switch (type_)
        {
#define CREATE_TOKEN(name_, descr_) \
        case Token::Type::name_: \
            return descr_;
#define CREATE_TOKEN_KEYWORD(name, value) CREATE_TOKEN(name, value)
            TOKEN_LIST_KEYWORD
            TOKEN_LIST
#undef CREATE_TOKEN
#undef CREATE_TOKEN_KEYWORD
        }
    }
}