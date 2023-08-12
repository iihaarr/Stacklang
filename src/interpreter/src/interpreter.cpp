#include <iostream>
#include "interpreter.hpp"

namespace stacklang::interpreter
{
	using namespace lexer;

	Interpreter::Interpreter(Config config_)
		: m_config(config_)
	{

	}

	std::unique_ptr<IInterpreter> GetInterpeter(Config config_)
	{
		return std::make_unique<Interpreter>(config_);
	}

	void printLexems(const std::vector<std::unique_ptr<BaseAstNode>>& ast_)
	{
		for (const auto& node : ast_)
		{
			switch (node->m_type)
			{
				case Token::Type::PUSH:
				case Token::Type::POP:
				case Token::Type::JI:
				case Token::Type::JMP:
				{
					const auto* pSingle = static_cast<SingleCommandAstNode*>(node.get());
					std::cout << "Lexem: " << GetTokenDescription(node->m_type) << "; Value = " << pSingle->m_pOperand->m_value << '\n';
					break;
				}
				default:
				{
					std::cout << "Lexem: " << GetTokenDescription(node->m_type) << '\n';
					break;
				}
			}
		}
	}

	void Interpreter::Run(const std::vector<std::unique_ptr<BaseAstNode>>& ast_)
	{
		if (m_config.m_isPrintLexems)
		{
			printLexems(ast_);
		}

		for (std::size_t i = 0; i < ast_.size(); ++i)
		{
			const auto& node = ast_[i];
			switch (node->m_type)
			{
			case Token::Type::PUSH:
				handlePush(node);
				break;
			case Token::Type::POP:
				handlePop(node);
				break;
			case Token::Type::READ:
				handleRead(node);
				break;
			case Token::Type::WRITE:
				handleWrite(node);
				break;
			case Token::Type::JI:
				i = handleJi(node, ast_, i);
				break;
			case Token::Type::JMP:
				i = handleJmp(node, ast_);
				break;
			case Token::Type::END:
				i = ast_.size();
				break;
			case Token::Type::DIV:
				handleDiv();
				break;
			case Token::Type::MOD:
				handleMod();
				break;
			case Token::Type::PLUS:
				handlePlus();
				break;
			case Token::Type::MINUS:
				handleMinus();
				break;
			case Token::Type::MULTIPLY:
				handleMultiply();
				break;
			case Token::Type::LESS:
				handleLess();
				break;
			case Token::Type::LESS_EQUAL:
				handleLessEqual();
				break;
			case Token::Type::GREATER:
				handleGreater();
				break;
			case Token::Type::GREATER_EQUAL:
				handleGreaterEqual();
				break;
			case Token::Type::EQUAL:
				handleEqual();
				break;
			case Token::Type::NOT_EQUAL:
				handleNotEqual();
				break;
			}
		}
	}

	void Interpreter::handlePush(const std::unique_ptr<BaseAstNode>& pushNode_)
	{
		const auto* pNode = static_cast<SingleCommandAstNode*>(pushNode_.get());
		if (pNode->m_pOperand->m_type == Token::Type::CONST)
		{
			m_stack.push(Var{ std::stoul(pNode->m_pOperand->m_value) });
		}
		else
		{
			const auto varIter = m_tableNames.find(pNode->m_pOperand->m_value);
			m_stack.push(varIter->second);
		}
	}

	void Interpreter::handlePop(const std::unique_ptr<BaseAstNode>& popNode_)
	{
		const auto* pNode = static_cast<SingleCommandAstNode*>(popNode_.get());
		m_tableNames[pNode->m_pOperand->m_value] = m_stack.top();
		m_stack.pop();
	}

	void Interpreter::handleRead(const std::unique_ptr<BaseAstNode>& readNode_)
	{
		std::string input;
		std::cout << "Waiting input: ";
		std::cin >> input;

		m_stack.push(Var{ std::stoul(input) });
	}
	void Interpreter::handleWrite(const std::unique_ptr<BaseAstNode>& writeNode_)
	{
		std::cout << m_stack.top().m_data << '\n';
		m_stack.pop();
	}

	void Interpreter::handleDiv()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs / rhs });
	}

	void Interpreter::handleMultiply()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs * rhs });
	}

	void Interpreter::handleMinus()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs - rhs });
	}

	void Interpreter::handleLess()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs < rhs });
	}

	void Interpreter::handleLessEqual()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs <= rhs });
	}

	void Interpreter::handleGreater()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs > rhs });
	}

	void Interpreter::handleGreaterEqual()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs >= rhs });
	}

	void Interpreter::handleEqual()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs == rhs });
	}

	void Interpreter::handleNotEqual()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs != rhs });
	}

	void Interpreter::handlePlus()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs + rhs });
	}

	void Interpreter::handleMod()
	{
		auto rhs = getValFromStack();
		auto lhs = getValFromStack();
		m_stack.push(Var{ lhs % rhs });
	}

	std::size_t Interpreter::handleJi(const std::unique_ptr<BaseAstNode>& writeNode_, const std::vector<std::unique_ptr<BaseAstNode>>& ast_, std::size_t currentIdx_)
	{
		const auto* pNode = static_cast<SingleCommandAstNode*>(writeNode_.get());
		auto topPositive = m_stack.top().m_data > 0;
		m_stack.pop();
		if (topPositive)
		{
			auto line = std::stoul(pNode->m_pOperand->m_value);
			for (std::size_t i = 0; i < ast_.size(); ++i)
			{
				if (ast_[i]->m_line == line)
					return i - 1;
			}
		}
		return currentIdx_;
	}

	std::size_t Interpreter::handleJmp(const std::unique_ptr<BaseAstNode>& writeNode_, const std::vector<std::unique_ptr<BaseAstNode>>& ast_)
	{
		const auto* pNode = static_cast<SingleCommandAstNode*>(writeNode_.get());
		auto line = std::stoul(pNode->m_pOperand->m_value);
		for (std::size_t i = 0; i < ast_.size(); ++i)
		{
			if (ast_[i]->m_line == line)
				return i - 1;
		}
		return ast_.size();
	}

	typename Interpreter::Var::value_type Interpreter::getValFromStack()
	{
		auto data = m_stack.top().m_data;
		m_stack.pop();
		return data;
	}
}