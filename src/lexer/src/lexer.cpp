#include <algorithm>
#include <iostream>
#include "lexer.hpp"
#include "lexer/lexer_interface.hpp"

namespace stacklang
{
    static constexpr auto kNewLine = '\n';
#define CREATE_TOKEN_KEYWORD(name, value) {Token::Type::name, value },
    static std::initializer_list<std::pair<Token::Type, std::string>> kKeywords
    {
        TOKEN_LIST_KEYWORD
    };
#undef CREATE_TOKEN_KEYWORD
    Lexer::Lexer(std::string_view code_) : m_code(code_), m_buffer(), m_currentCharIdx(0), m_currentLine(1)
    {

    }
    std::unique_ptr<ILexer> GetLexer(std::string_view code_)
    {
        return std::make_unique<Lexer>(code_);
    }
    Token Lexer::NextToken()
    {
        std::size_t beginIdx = m_currentCharIdx;
        for(; m_currentCharIdx < m_code.size(); ++m_currentCharIdx)
        {
            if(std::isalpha(m_code[m_currentCharIdx])) return handleIdentifier();
            if(std::isdigit(m_code[m_currentCharIdx])) return handleConstant();
            if(m_code[m_currentCharIdx] == kNewLine) return handleNewLine();
            if(std::isspace(m_code[m_currentCharIdx])) continue;

            Location locationUndefinedSymbol{beginIdx, m_currentCharIdx, m_currentLine};
            handlerUnexpectedSymbol(locationUndefinedSymbol);
            skipError();
            return MakeToken(Token::Type::ERROR, locationUndefinedSymbol);
        }
        Location location{beginIdx, m_currentCharIdx, m_currentLine};
        return MakeToken(Token::Type::END_OF_FILE, location);
    }
    Token Lexer::handleIdentifier()
    {
        std::size_t beginIdx = m_currentCharIdx;
        for(; m_currentCharIdx < m_code.size(); ++m_currentCharIdx)
        {
            if(std::isalpha(m_code[m_currentCharIdx]))
            {
                m_buffer.push_back(m_code[m_currentCharIdx]);
            }
            else if(std::isspace(m_code[m_currentCharIdx]))
            {
                break;
            }
            else
            {
                Location locationUndefinedSymbol{beginIdx, m_currentCharIdx, m_currentLine};
                handlerUnexpectedSymbol(locationUndefinedSymbol);
                skipError();
                m_buffer.clear();
                return MakeToken(Token::Type::ERROR, locationUndefinedSymbol);
            }
        }
        std::size_t endIdx = m_currentCharIdx - 1;
        Location location{beginIdx, endIdx, m_currentLine};
        const auto fnFindKeyword = [this](const auto& pair_) { return pair_.second == this->m_buffer; };
        const auto keywordIter = std::find_if(std::begin(kKeywords), std::end(kKeywords), fnFindKeyword);
        const auto isKeyword = keywordIter != std::end(kKeywords);
        auto token = isKeyword
                ? MakeToken(keywordIter->first, location)
                : MakeToken(Token::Type::VAR, location, this->m_buffer);
        m_buffer.clear();
        return token;
    }
    Token Lexer::handleConstant()
    {
        std::size_t beginIdx = m_currentCharIdx;
        for(; m_currentCharIdx < m_code.size(); ++m_currentCharIdx)
        {
            if(std::isdigit(m_code[m_currentCharIdx]))
            {
                m_buffer.push_back(m_code[m_currentCharIdx]);
            }
            else if(std::isspace(m_code[m_currentCharIdx]))
            {
                break;
            }
            else
            {
                Location locationUndefinedSymbol{beginIdx, m_currentCharIdx, m_currentLine};
                handlerUnexpectedSymbol(locationUndefinedSymbol);
                skipError();
                m_buffer.clear();
                return MakeToken(Token::Type::ERROR, locationUndefinedSymbol);
            }
        }
        std::size_t endIdx = m_currentCharIdx - 1;
        Location location{beginIdx, endIdx, m_currentLine};
        auto token = MakeToken(Token::Type::CONST, location, this->m_buffer);
        m_buffer.clear();
        return token;
    }
    Token Lexer::handleNewLine()
    {
        Location location{m_currentCharIdx, m_currentCharIdx, m_currentLine};
        auto token = MakeToken(Token::Type::NEWLINE, location);
        ++m_currentLine;
        ++m_currentCharIdx;
        return token;
    }
    void Lexer::handlerUnexpectedSymbol(Location location_)
    {
        std::cout << "Error on line " << location_.m_line << ": " << "Unexpected symbol '" << m_code[location_.m_end]
        << "'" <<  '\n';
        static constexpr std::size_t kMaxLenErrorStrHelp = 64;
        std::size_t lenErrorStrHelp = 0;
        for(std::size_t idx = location_.m_begin;
            idx < m_code.size() && lenErrorStrHelp < kMaxLenErrorStrHelp && m_code[idx] != kNewLine;
            ++lenErrorStrHelp, ++idx)
        {
            std::cout << m_code[idx];
        }
        std::cout <<  '\n';
        for(std::size_t i = location_.m_begin; i < location_.m_end; ++i)
        {
            std::cout << ' ';
        }
        std::cout << "^ HERE\n";
    }

    void Lexer::skipError()
    {
        for(std::size_t i = m_currentCharIdx; !std::isspace(m_code[i]) && i < m_code.size(); ++i)
        {
            ++m_currentCharIdx;
        }
    }
}