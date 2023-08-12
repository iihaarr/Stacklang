#include <algorithm>
#include <iostream>
#include "lexer.hpp"
#include "lexer/lexer_interface.hpp"

namespace stacklang::lexer
{
    static constexpr auto kNewLine = '\n';
    static constexpr auto kCommentCh = ';';
    static constexpr auto kSpace = ' ';
    static constexpr std::size_t kMaxLenErrorStrHelp = 64;
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
    bool isRelation(char ch_)
    {
        return ch_ == '=' || ch_ == '>' || ch_ == '<' || ch_ == '!';
    }
    bool isOperator(char ch_)
    {
        return ch_ == '+' || ch_ == '-' || ch_ == '*' || ch_ == '%' || ch_ == '/';
    }
    Token::Type getTypeRelationByChar(char ch_)
    {
        switch (ch_)
        {
        case '<': return Token::Type::LESS;
        case '>': return Token::Type::GREATER;
        case '=': return Token::Type::EQUAL;
        case '!': return Token::Type::NOT;
        default: return Token::Type::ERROR;
        }
    }
    Token::Type getTypeRelationByChars(char c1_, char c2_)
    {
        switch (c1_)
        {
        case '<':
        {
            switch (c2_)
            {
            case '=': return Token::Type::LESS_EQUAL;
            default: return Token::Type::ERROR;
            }
        }
        case '>':
        {
            switch (c2_)
            {
            case '=': return Token::Type::GREATER_EQUAL;
            default: return Token::Type::ERROR;
            }
        }
        case '!':
        {
            switch (c2_)
            {
            case '=': return Token::Type::NOT_EQUAL;
            default: return Token::Type::ERROR;
            }
        }
        default: return Token::Type::ERROR;
        }
    }
    Token::Type getTypeOperator(char ch_)
    {
        switch (ch_)
        {
        case '+': return Token::Type::PLUS;
        case '-': return Token::Type::MINUS;
        case '*': return Token::Type::MULTIPLY;
        case '/': return Token::Type::DIV;
        case '%': return Token::Type::MOD;
        default: return Token::Type::ERROR;
        }
    }
    Token Lexer::NextToken()
    {
        std::size_t beginIdx = m_currentCharIdx;
        for(; m_currentCharIdx < m_code.size(); ++m_currentCharIdx)
        {
            if (std::isalpha(currentChar())) return handleIdentifier();
            if (std::isdigit(currentChar())) return handleConstant();
            if (currentChar() == kNewLine) return handleNewLine();
            if (currentChar() == kSpace) continue;
            if (isRelation(currentChar())) return handleRelation();
            if (isOperator(currentChar())) return handleOperator();
            if (currentChar() == kCommentCh)
            {
                skipComment();
                continue;
            }

            Location locationUndefinedSymbol{beginIdx, m_currentCharIdx, m_currentLine};
            handleUnexpectedSymbol(locationUndefinedSymbol);
            skipError();
            return MakeToken(Token::Type::ERROR, locationUndefinedSymbol);
        }
        Location location{beginIdx, m_currentCharIdx, m_currentLine};
        return MakeToken(Token::Type::END_OF_FILE, location);
    }
    const std::string& Lexer::GetCode() const
    {
        return m_code;
    }
    Token Lexer::handleIdentifier()
    {
        std::size_t beginIdx = m_currentCharIdx;
        for(; m_currentCharIdx < m_code.size(); ++m_currentCharIdx)
        {
            auto curCh = currentChar();
            if(std::isalpha(curCh))
            {
                m_buffer.push_back(curCh);
            }
            else if(std::isspace(curCh) || curCh == kCommentCh)
            {
                break;
            }
            else
            {
                Location locationUndefinedSymbol{beginIdx, m_currentCharIdx, m_currentLine};
                handleUnexpectedSymbol(locationUndefinedSymbol);
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
            auto curCh = currentChar();
            if(std::isdigit(curCh))
            {
                m_buffer.push_back(curCh);
            }
            else if(std::isspace(curCh) || curCh == kCommentCh)
            {
                break;
            }
            else
            {
                Location locationUndefinedSymbol{beginIdx, m_currentCharIdx, m_currentLine};
                handleUnexpectedSymbol(locationUndefinedSymbol);
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
    Token Lexer::handleRelation()
    {
        std::size_t beginIdx = m_currentCharIdx;
        char chRelFirst = nextChar();
        auto typeRelFirst = getTypeRelationByChar(chRelFirst);
        char chRelSecond = nextChar();
        if (!isRelation(chRelSecond))
        {
            revertChar();
            std::size_t endIdx = beginIdx;
            if (typeRelFirst == Token::Type::NOT)
            {
                Location location{ beginIdx, beginIdx, m_currentLine };
                handleNotRelationWithoutOtherSymRel(location);
                skipError();
                return MakeToken(Token::Type::ERROR, location);
            }
            return MakeToken(typeRelFirst, Location{ beginIdx, endIdx, m_currentLine });
        }
        else
        {
            auto typeRel = getTypeRelationByChars(chRelFirst, chRelSecond);
            Location location{ beginIdx, m_currentCharIdx - 1, m_currentLine };
            if (typeRel == Token::Type::ERROR)
            {
                handleRelationError(location);
                skipError();
                return MakeToken(Token::Type::ERROR, location);
            }
            return MakeToken(typeRel, location);
        }
    }
    Token Lexer::handleOperator()
    {
        Location loc{ m_currentCharIdx, m_currentCharIdx, m_currentLine };
        char opCh = currentChar();
        ++m_currentCharIdx;
        return MakeToken(getTypeOperator(opCh), loc);
    }
    char Lexer::nextChar()
    {
        return m_code[m_currentCharIdx++];
    }
    char Lexer::currentChar()
    {
        return m_code[m_currentCharIdx];
    }
    void Lexer::revertChar()
    {
        --m_currentCharIdx;
    }
    void Lexer::handleUnexpectedSymbol(Location location_)
    {
        std::cout << "Error on line " << location_.m_line << ": " << "Unexpected symbol '" << m_code[location_.m_end]
        << "'" <<  '\n';

        printLineError(location_);
    }
    void Lexer::handleNotRelationWithoutOtherSymRel(Location location_)
    {
        std::cout << "Error on line " << location_.m_line << ": " << "Unexpected symbol '" << m_code[location_.m_end]
            << "', expected '='\n";

        printLineError(location_);
    }
    void Lexer::handleRelationError(Location location_)
    {
        std::cout << "Error on line " << location_.m_line << ": " << "Unexpected relation '" << m_code[location_.m_begin]
            << m_code[location_.m_end] << "'\n";

        printLineError(location_);
    }
    void Lexer::skipError()
    {
        for (std::size_t i = m_currentCharIdx; !std::isspace(m_code[i]) && i < m_code.size(); ++i, ++m_currentCharIdx);
    }
    void Lexer::skipComment()
    {
        for (std::size_t i = m_currentCharIdx; currentChar() != kNewLine && i < m_code.size(); ++i, ++m_currentCharIdx);
    }
    void Lexer::printLineError(Location location_)
    {
        std::size_t lenErrorStrHelp = 0;
        for (std::size_t idx = location_.m_begin;
            idx < m_code.size() && lenErrorStrHelp < kMaxLenErrorStrHelp && m_code[idx] != kNewLine;
            ++lenErrorStrHelp, ++idx)
        {
            std::cout << m_code[idx];
        }
        std::cout << '\n';
        for (std::size_t i = location_.m_begin; i < location_.m_end; ++i)
        {
            std::cout << ' ';
        }
        std::cout << "^ HERE\n";
        std::cout << "----------------------\n";
    }
}