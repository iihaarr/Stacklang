#include <gtest/gtest.h>
#include "lexer/lexer_interface.hpp"

TEST(LexerTestTokens, BasicAssertions)
{
    using namespace stacklang::lexer;
    const auto code = "push pop;12313\n hello 123123\n12312k111 write\n read\n+;12312312\n - % / >=;131231";
    auto lexer = GetLexer(code);
    std::vector<Token> tokens;
    for(auto token = lexer->NextToken(); token.m_type != Token::Type::END_OF_FILE; token = lexer->NextToken())
    {
        if(token.m_type != Token::Type::ERROR)
            tokens.emplace_back(std::move(token));
    }
    Location emptyLocation{};
    std::vector<Token> expectedTokens;
    expectedTokens.reserve(9);
    expectedTokens.emplace_back(Token::Type::PUSH, emptyLocation);
    expectedTokens.emplace_back(Token::Type::POP, emptyLocation);
    expectedTokens.emplace_back(Token::Type::VAR, emptyLocation, std::string("hello"));
    expectedTokens.emplace_back(Token::Type::CONST, emptyLocation,std::string("123123"));
    expectedTokens.emplace_back(Token::Type::NEWLINE, emptyLocation);
    expectedTokens.emplace_back(Token::Type::WRITE, emptyLocation);
    expectedTokens.emplace_back(Token::Type::NEWLINE, emptyLocation);
    expectedTokens.emplace_back(Token::Type::READ, emptyLocation);
    expectedTokens.emplace_back(Token::Type::NEWLINE, emptyLocation);
    expectedTokens.emplace_back(Token::Type::PLUS , emptyLocation);
    expectedTokens.emplace_back(Token::Type::MINUS, emptyLocation);
    expectedTokens.emplace_back(Token::Type::MOD, emptyLocation);
    expectedTokens.emplace_back(Token::Type::DIV, emptyLocation);
    expectedTokens.emplace_back(Token::Type::GREATER_EQUAL, emptyLocation);
    EXPECT_TRUE(tokens.size() == expectedTokens.size());
    for(std::size_t i = 0; i < tokens.size(); ++i)
    {
        EXPECT_TRUE(tokens[i].m_type == expectedTokens[i].m_type);
        if(tokens[i].m_type == Token::Type::CONST
            || tokens[i].m_type == Token::Type::VAR)
        {
            EXPECT_TRUE(*(tokens[i].m_text) == *(expectedTokens[i].m_text));
        }
    }
}
TEST(LexerTestRelationsToken, BasicAssertions)
{
    using namespace stacklang::lexer;
    const auto code = "< > <= >= = != ! <! !>";
    auto lexer = GetLexer(code);
    std::vector<Token> tokens;
    for (auto token = lexer->NextToken(); token.m_type != Token::Type::END_OF_FILE; token = lexer->NextToken())
    {
        if (token.m_type != Token::Type::ERROR)
            tokens.emplace_back(std::move(token));
    }
    Location emptyLocation{};
    std::vector<Token> expectedTokens;
    expectedTokens.emplace_back(Token::Type::LESS, emptyLocation);
    expectedTokens.emplace_back(Token::Type::GREATER, emptyLocation);
    expectedTokens.emplace_back(Token::Type::LESS_EQUAL, emptyLocation);
    expectedTokens.emplace_back(Token::Type::GREATER_EQUAL, emptyLocation);
    expectedTokens.emplace_back(Token::Type::EQUAL, emptyLocation);
    expectedTokens.emplace_back(Token::Type::NOT_EQUAL, emptyLocation);
    EXPECT_TRUE(tokens.size() == expectedTokens.size());
    for (std::size_t i = 0; i < tokens.size(); ++i)
    {
        EXPECT_TRUE(tokens[i].m_type == expectedTokens[i].m_type);
    }
}