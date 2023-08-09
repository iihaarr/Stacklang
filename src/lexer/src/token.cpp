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
/*
    Token::Token(Token&& other_) noexcept
        : m_type(std::move(other_.m_type)), m_location(std::move(other_.m_location)), m_text(std::move(other_.m_text))
    {
        other_.m_type = Type::ERROR;
        other_.m_location = {};
        other_.m_text = nullptr;
    }

    Token::Token(const Token& other_)
        : m_type(other_.m_type), m_location(other_.m_location), m_text(std::make_unique<std::string>(*other_.m_text))
    {

    }
    Token &Token::operator=(const Token& other_) {
        if(this == &other_)
            return *this;

        m_type = other_.m_type;
        m_location = other_.m_location;
        return *this;
    }*/
}