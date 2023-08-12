#include "parser/ast_nodes.hpp"

namespace stacklang::parser
{
	using namespace lexer;

	BaseAstNode::BaseAstNode(Token::Type type_, std::size_t line_)
		: m_type(type_), m_line(line_)
	{

	}
	ValueableAstNode::ValueableAstNode(Token::Type type_, std::size_t line_, std::string value_)
		: BaseAstNode(type_, line_), m_value(std::move(value_))
	{

	}
	NoOperandCommandAstNode::NoOperandCommandAstNode(Token::Type commandType_, std::size_t line_)
		: BaseAstNode(commandType_, line_)
	{

	}
	SingleCommandAstNode::SingleCommandAstNode(Token::Type commandType_, std::size_t line_, std::unique_ptr<ValueableAstNode> pOperand_)
		: BaseAstNode(commandType_, line_), m_pOperand(std::move(pOperand_))
	{

	}
	RelationAstNode::RelationAstNode(Token::Type relationType_, std::size_t line_)
		: BaseAstNode(relationType_, line_)
	{

	}
	OperatorAstNode::OperatorAstNode(Token::Type operatorType_, std::size_t line_)
		: BaseAstNode(operatorType_, line_)
	{

	}
	VariableAstNode::VariableAstNode(std::size_t line_, std::string value_)
		: ValueableAstNode(Token::Type::VAR, line_, std::move(value_))
	{

	}
	ConstantAstNode::ConstantAstNode(std::size_t line_, std::string value_)
		: ValueableAstNode(Token::Type::CONST, line_, std::move(value_))
	{

	}
}
