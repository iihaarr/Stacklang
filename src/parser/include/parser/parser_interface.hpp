#ifndef STACKLANG_PARSER_INTERFACE_HPP
#define STACKLANG_PARSER_INTERFACE_HPP

#include <string>
#include "ast_nodes.hpp"

namespace stacklang
{
	struct IParser
	{
		virtual std::vector<std::unique_ptr<BaseAstNode>> GetAST() = 0;
	};

	std::unique_ptr<IParser> GetParser(std::string_view code_);
}

#endif //STACKLANG_PARSER_INTERFACE_HPP