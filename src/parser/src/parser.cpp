#include <iostream>
#include <map>
#include "parser/parser_interface.hpp"
#include "parser.hpp"

namespace stacklang
{
	static constexpr auto kNewLine = '\n';
	static constexpr std::size_t kMaxLenErrorStrHelp = 64;
	static constexpr auto kExpectedConstOrVar = "Expected token 'CONST' or 'VAR', got '";
	static constexpr auto kExpectedNewLine = "Expected token 'NEWLINE' or 'EOF', got '";
	static constexpr auto kExpectedConst = "Expected token 'CONST', got '";
	static constexpr auto kExpectedVar = "Expected token 'VAR', got '";

	std::unique_ptr<IParser> GetParser(std::string_view code_)
	{
		return std::make_unique<Parser>((code_));
	}

	Parser::Parser(std::string_view code_) : m_pLexer(GetLexer(code_))
	{
		m_tokenHandlers.insert({ Token::Type::PUSH, &Parser::handlePushCommand });
		m_tokenHandlers.insert({ Token::Type::POP, &Parser::handlePopCommand });
		m_tokenHandlers.insert({ Token::Type::READ, &Parser::handleReadCommand });
		m_tokenHandlers.insert({ Token::Type::WRITE, &Parser::handleWriteCommand });
		m_tokenHandlers.insert({ Token::Type::JI, &Parser::handleJiCommand });
		m_tokenHandlers.insert({ Token::Type::JMP, &Parser::handleJmpCommand });
		m_tokenHandlers.insert({ Token::Type::END, &Parser::handleEndCommand });

		m_tokenHandlers.insert({ Token::Type::PLUS, &Parser::handleOperator });
		m_tokenHandlers.insert({ Token::Type::MINUS, &Parser::handleOperator });
		m_tokenHandlers.insert({ Token::Type::MULTIPLY, &Parser::handleOperator });
		m_tokenHandlers.insert({ Token::Type::DIV, &Parser::handleOperator });
		m_tokenHandlers.insert({ Token::Type::MOD, &Parser::handleOperator });

		m_tokenHandlers.insert({ Token::Type::EQUAL, &Parser::handleRelation });
		m_tokenHandlers.insert({ Token::Type::NOT_EQUAL, &Parser::handleRelation });
		m_tokenHandlers.insert({ Token::Type::GREATER, &Parser::handleRelation });
		m_tokenHandlers.insert({ Token::Type::GREATER_EQUAL, &Parser::handleRelation });
		m_tokenHandlers.insert({ Token::Type::LESS, &Parser::handleRelation });
		m_tokenHandlers.insert({ Token::Type::LESS_EQUAL, &Parser::handleRelation });
	}

	bool isConst(Token::Type type_)
	{
		return type_ == Token::Type::CONST;
	}

	bool isVar(Token::Type type_)
	{
		return type_ == Token::Type::VAR;
	}

	bool isConstOrVar(Token::Type type_)
	{
		return isConst(type_) || isVar(type_);
	}

	bool isNewLineOrEOF(Token::Type type_)
	{
		return type_ == Token::Type::NEWLINE || type_ == Token::Type::END_OF_FILE;
	}

	std::vector<std::unique_ptr<BaseAstNode>> Parser::GetAST()
	{
		std::vector<std::unique_ptr<BaseAstNode>> nodes;
		bool hasError = false;
		for (auto token = m_pLexer->NextToken(); token.m_type != Token::Type::END_OF_FILE; token = m_pLexer->NextToken())
		{
			if (token.m_type == Token::Type::ERROR)
				continue;

			auto node = (this->*m_tokenHandlers[token.m_type])(token);
			if (node->m_type == Token::Type::ERROR)
			{
				skipError();
				hasError = true;
			}
			else
			{
				nodes.emplace_back(std::move(node));
			}
		}
		if (hasError)
		{
			nodes.clear();
		}
		return nodes;
	}

	std::unique_ptr<BaseAstNode> Parser::handleError(const Token& errorToken_)
	{
		skipError();
		return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
	}

	void Parser::printLineError(Location location_) const
	{
		const auto& code = m_pLexer->GetCode();
		std::size_t lenErrorStrHelp = 0;
		for (std::size_t idx = location_.m_begin;
			idx < code.size() && lenErrorStrHelp < kMaxLenErrorStrHelp && code[idx] != kNewLine;
			++lenErrorStrHelp, ++idx)
		{
			std::cout << code[idx];
		}
		std::cout << '\n';
		for (std::size_t i = location_.m_begin; i < location_.m_end; ++i)
		{
			std::cout << ' ';
		}
		std::cout << "^ HERE\n";
	}

	std::unique_ptr<BaseAstNode> Parser::handlePushCommand(const Token& pushToken_)
	{
		const auto& code = m_pLexer->GetCode();
		auto token = m_pLexer->NextToken();
		if (!isConstOrVar(token.m_type))
		{
			auto pushBeginIdx = token.m_location.m_begin;
			for (; code[pushBeginIdx] != 'p'; --pushBeginIdx);
			printExpected(kExpectedConstOrVar, Location{ pushBeginIdx, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		auto valueableNode = MakeASTNodeAs<ValueableAstNode>(token.m_type, *token.m_text);
		const auto valueNodeLocation = token.m_location;
		token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ valueNodeLocation.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<SingleCommandAstNode>(Token::Type::PUSH, std::move(valueableNode));
	}

	std::unique_ptr<BaseAstNode> Parser::handlePopCommand(const Token& popToken_)
	{
		const auto& code = m_pLexer->GetCode();
		auto token = m_pLexer->NextToken();
		if (!isVar(token.m_type))
		{
			auto popBeginIdx = token.m_location.m_begin;
			for (; code[popBeginIdx] != 'p'; --popBeginIdx);
			popBeginIdx -= 2;
			printExpected(kExpectedVar, Location{ popBeginIdx, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		auto varNode = MakeASTNodeAs<VariableAstNode>(*token.m_text);
		const auto varNodeLocation = token.m_location;
		token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ varNodeLocation.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<SingleCommandAstNode>(Token::Type::POP, std::move(varNode));
	}

	std::unique_ptr<BaseAstNode> Parser::handleJiCommand(const Token& jiToken_)
	{
		const auto& code = m_pLexer->GetCode();
		auto token = m_pLexer->NextToken();
		if (!isConst(token.m_type))
		{
			auto popBeginIdx = token.m_location.m_begin;
			for (; code[popBeginIdx] != 'j'; --popBeginIdx);
			printExpected(kExpectedConst, Location{ popBeginIdx, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		auto constNode = MakeASTNodeAs<ConstantAstNode>(*token.m_text);
		const auto constNodeLocation = token.m_location;
		token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ constNodeLocation.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<SingleCommandAstNode>(Token::Type::JI, std::move(constNode));
	}

	std::unique_ptr<BaseAstNode> Parser::handleJmpCommand(const Token& jmpToken_)
	{
		const auto& code = m_pLexer->GetCode();
		auto token = m_pLexer->NextToken();
		if (!isConst(token.m_type))
		{
			auto popBeginIdx = token.m_location.m_begin;
			for (; code[popBeginIdx] != 'j'; --popBeginIdx);
			printExpected(kExpectedConst, Location{ popBeginIdx, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		auto constNode = MakeASTNodeAs<ConstantAstNode>(*token.m_text);
		const auto constNodeLocation = token.m_location;
		token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ constNodeLocation.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<SingleCommandAstNode>(Token::Type::JMP, std::move(constNode));
	}

	std::unique_ptr<BaseAstNode> Parser::handleReadCommand(const Token& readToken_)
	{
		auto token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ readToken_.m_location.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<NoOperandCommandAstNode>(Token::Type::READ);
	}

	std::unique_ptr<BaseAstNode> Parser::handleWriteCommand(const Token& writeToken_)
	{
		auto token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ writeToken_.m_location.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<NoOperandCommandAstNode>(Token::Type::WRITE);
	}

	std::unique_ptr<BaseAstNode> Parser::handleEndCommand(const Token& endToken_)
	{
		auto token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ endToken_.m_location.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<NoOperandCommandAstNode>(Token::Type::END);
	}

	std::unique_ptr<BaseAstNode> Parser::handleOperator(const Token& opToken_)
	{
		auto token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ opToken_.m_location.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<OperatorAstNode>(opToken_.m_type);
	}

	std::unique_ptr<BaseAstNode> Parser::handleRelation(const Token& relToken_)
	{
		auto token = m_pLexer->NextToken();
		if (!isNewLineOrEOF(token.m_type))
		{
			printExpected(kExpectedNewLine, Location{ relToken_.m_location.m_begin, token.m_location.m_begin, token.m_location.m_line }, token.m_type);
			return MakeASTNode<BaseAstNode>(Token::Type::ERROR);
		}
		return MakeASTNode<RelationAstNode>(relToken_.m_type);
	}

	void Parser::printExpected(std::string_view errorMsg_, Location loc_, Token::Type typeGot_)
	{
		std::cout << "Syntax error on line " << loc_.m_line << ": " << errorMsg_ << GetTokenDescription(typeGot_) << "'\n";

		printLineError(loc_);
	}

	void Parser::skipError()
	{
		for (auto token = m_pLexer->NextToken();
			token.m_type != Token::Type::END_OF_FILE && !isNewLineOrEOF(token.m_type);
			token = m_pLexer->NextToken());
	}
}