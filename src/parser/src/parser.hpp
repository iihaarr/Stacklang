#ifndef STACKLANG_PARSER_HPP
#define STACKLANG_PARSER_HPP

#include <string>
#include <map>
#include "lexer/lexer_interface.hpp"
#include "parser/parser_interface.hpp"
#include <string_view>

namespace stacklang
{
	class Parser : public IParser
	{
		using ParserTokenHandler = std::unique_ptr<BaseAstNode>(Parser::*)(const Token&);
	public:
		Parser(std::string_view code_);
		
		std::vector<std::unique_ptr<BaseAstNode>> GetAST() override;
	private:
		std::unique_ptr<BaseAstNode> handlePushCommand(const Token& pushToken_);
		std::unique_ptr<BaseAstNode> handlePopCommand(const Token& popToken_);
		std::unique_ptr<BaseAstNode> handleReadCommand(const Token& readToken_);
		std::unique_ptr<BaseAstNode> handleWriteCommand(const Token& writeToken_);
		std::unique_ptr<BaseAstNode> handleEndCommand(const Token& endToken_);
		std::unique_ptr<BaseAstNode> handleJiCommand(const Token& jiToken_);
		std::unique_ptr<BaseAstNode> handleJmpCommand(const Token& jmpToken_);
		std::unique_ptr<BaseAstNode> handleOperator(const Token& opToken_);
		std::unique_ptr<BaseAstNode> handleRelation(const Token& relToken_);
		std::unique_ptr<BaseAstNode> handleError(const Token& errorToken_);

		void printLineError(Location location_) const;
		void printExpected(std::string_view errorMsg_, Location loc_, Token::Type typeGot_);
		void skipError();
	private:
		std::unique_ptr<ILexer> m_pLexer;
		std::map<Token::Type, ParserTokenHandler> m_tokenHandlers;
	};
}

#endif