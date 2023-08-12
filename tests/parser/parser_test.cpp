#include <gtest/gtest.h>
#include "parser/parser_interface.hpp"

TEST(ParserTest, BasicAssertions)
{
	using namespace stacklang::lexer;
	using namespace stacklang::parser;

	const auto code = "push 1\npop x\njmp 1\n ji 2\nread\nwrite\nend\n+\n-\n*\n/\n%\n<\n<=\n>\n>=\n!=\n=";
	auto parser = GetParser(code);
	auto astGot = parser->GetAST();
	std::vector<std::unique_ptr<BaseAstNode>> astExpected;
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::PUSH, 0,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::CONST, 0, std::string("1"))));
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::POP, 0,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::VAR, 0, std::string("x"))));
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::JMP, 0,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::VAR, 0, std::string("1"))));
	astExpected.emplace_back(MakeASTNode<SingleCommandAstNode>(Token::Type::JI, 0,
		MakeASTNodeAs<ValueableAstNode>(Token::Type::VAR, 0, std::string("2"))));
	astExpected.emplace_back(MakeASTNode<NoOperandCommandAstNode>(Token::Type::READ, 0));
	astExpected.emplace_back(MakeASTNode<NoOperandCommandAstNode>(Token::Type::WRITE, 0));
	astExpected.emplace_back(MakeASTNode<NoOperandCommandAstNode>(Token::Type::END, 0));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::PLUS, 0));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::MINUS, 0));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::MULTIPLY, 0));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::DIV, 0));
	astExpected.emplace_back(MakeASTNode<OperatorAstNode>(Token::Type::MOD, 0));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::LESS, 0));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::LESS_EQUAL, 0));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::GREATER, 0));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::GREATER_EQUAL, 0));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::NOT_EQUAL, 0));
	astExpected.emplace_back(MakeASTNode<RelationAstNode>(Token::Type::EQUAL, 0));
	
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
	using namespace stacklang::lexer;
	using namespace stacklang::parser;

	const auto code = "push write\npop 1\njmp x\n read x\n write 1\n";
	auto parser = GetParser(code);
	auto astGot = parser->GetAST();
	EXPECT_TRUE(astGot.size() == 0);
}
