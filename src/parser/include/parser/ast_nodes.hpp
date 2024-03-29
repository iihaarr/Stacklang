#ifndef STACKLANG_AST_NODES_HPP
#define STACKLANG_AST_NODES_HPP

#include <vector>
#include <memory>
#include <string>
#include "lexer/token.hpp"

namespace stacklang::parser
{
	using namespace lexer;

	struct BaseAstNode
	{
		BaseAstNode(Token::Type type_, std::size_t line_);

		Token::Type m_type;
		std::size_t m_line;
	};
	struct ValueableAstNode : public BaseAstNode
	{
		ValueableAstNode(Token::Type type_, std::size_t line_, std::string value_);

		std::string m_value;
	};
	struct NoOperandCommandAstNode : public BaseAstNode
	{
		NoOperandCommandAstNode(Token::Type commandType_, std::size_t line_);
	};
	struct SingleCommandAstNode : public BaseAstNode
	{
		SingleCommandAstNode(Token::Type commandType_, std::size_t line_, std::unique_ptr<ValueableAstNode> pOperand_);

		std::unique_ptr<ValueableAstNode> m_pOperand;
	};
	struct OperatorAstNode : public BaseAstNode
	{
		OperatorAstNode(Token::Type operatorType_, std::size_t line_);
	};
	struct RelationAstNode : public BaseAstNode
	{
		RelationAstNode(Token::Type operatorType_, std::size_t line_);
	};
	struct VariableAstNode : public ValueableAstNode
	{
		VariableAstNode(std::size_t line_, std::string value_);
	};
	struct ConstantAstNode : public ValueableAstNode
	{
		ConstantAstNode(std::size_t line_, std::string value_);
	};

	template<typename T, typename... Args>
	std::unique_ptr<BaseAstNode> MakeASTNode(Args&&... args_)
	{
		return std::make_unique<T>(std::forward<Args>(args_)...);
	}
	template<typename T, typename... Args>
	std::unique_ptr<T> MakeASTNodeAs(Args&&... args_)
	{
		return std::make_unique<T>(std::forward<Args>(args_)...);
	}
}

#endif 