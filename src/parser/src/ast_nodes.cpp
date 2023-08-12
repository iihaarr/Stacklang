#include "parser/ast_nodes.hpp"

namespace stacklang
{
	BaseAstNode::BaseAstNode(Token::Type type_)
		: m_type(type_)
	{

	}
	ValueableAstNode::ValueableAstNode(Token::Type type_, std::string value_)
		: BaseAstNode(type_), m_value(std::move(value_))
	{

	}
	NoOperandCommandAstNode::NoOperandCommandAstNode(Token::Type commandType_)
		: BaseAstNode(commandType_)
	{

	}
	SingleCommandAstNode::SingleCommandAstNode(Token::Type commandType_, std::unique_ptr<ValueableAstNode> pOperand_)
		: BaseAstNode(commandType_), m_pOperand(std::move(pOperand_))
	{

	}
	RelationAstNode::RelationAstNode(Token::Type relationType_)
		: BaseAstNode(relationType_)
	{

	}
	OperatorAstNode::OperatorAstNode(Token::Type operatorType_)
		: BaseAstNode(operatorType_)
	{

	}
	VariableAstNode::VariableAstNode(std::string value_)
		: ValueableAstNode(Token::Type::VAR, std::move(value_))
	{

	}
	ConstantAstNode::ConstantAstNode(std::string value_)
		: ValueableAstNode(Token::Type::CONST, std::move(value_))
	{

	}
}
