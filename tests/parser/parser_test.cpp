#include <gtest/gtest.h>
#include "parser/parser_interface.hpp"

TEST(ParserTest, BasicAssertions)
{
	using namespace stacklang;

	const auto code = "push 1\npop x\njmp 1\n ji 2\nread\nwrite\nend\n+\n-\n*\n/\n%\n<\n<=\n>\n>=\n!=\n=";
	auto parser = GetParser(code);
	auto astGot = parser->GetAST();
	std::vector<std::unique_ptr<BaseAstNode>> astExpected;
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::PUSH,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::CONST, std::string("1"))));
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::POP,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::VAR, std::string("x"))));
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::JMP,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::VAR, std::string("1"))));
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::JI,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::VAR, std::string("2"))));
	astExpected.emplace_back(MakeASTNode<NoOperandCommandAstNode>(Token::Type::READ));
	astExpected.emplace_back(MakeASTNode<NoOperandCommandAstNode>(Token::Type::WRITE));
	astExpected.emplace_back(MakeASTNode<NoOperandCommandAstNode>(Token::Type::END));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::PLUS));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::MINUS));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::MULTIPLY));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::DIV));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::MOD));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::LESS));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::LESS_EQUAL));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::GREATER));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::GREATER_EQUAL));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::NOT_EQUAL));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::EQUAL));
	
	EXPECT_TRUE(astGot.size() == astExpected.size());
	for (std::size_t i = 0; i < astGot.size(); ++i)
	{
		EXPECT_TRUE(astGot[i]->m_type == astExpected[i]->m_type);
		switch (astGot[i]->m_type)
		{
		case Token::Type::PUSH:
		case Token::Type::POP:
		case Token::Type::JMP:
		case Token::Type::JI:
		{
			auto* a1 = static_cast<SingleCommandAstNode*>(astGot[i].get());
			auto* a2 = static_cast<SingleCommandAstNode*>(astExpected[i].get());
			EXPECT_TRUE(a1);
			EXPECT_TRUE(a2);
			EXPECT_TRUE(a1->m_pOperand->m_value == a2->m_pOperand->m_value);
			break;
		}
		}
	}
}
TEST(ParserErrorTest, BasicAssertions)
{
	using namespace stacklang;

	const auto code = "push write\npop 1\njmp x\n read x\n write 1\n";
	auto parser = GetParser(code);
	auto astGot = parser->GetAST();
	EXPECT_TRUE(astGot.size() == 0);
}
