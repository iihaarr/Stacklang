#ifndef STACKLANG_INTERPRETER_INTERFACE_HPP
#define STACKLANG_INTERPRETER_INTERFACE_HPP

#include <vector>
#include <memory>
#include "parser/ast_nodes.hpp"
#include "config.hpp"

namespace stacklang::interpreter
{
	using namespace parser;

	struct IInterpreter
	{
		virtual void Run(const std::vector<std::unique_ptr<BaseAstNode>>& ast_) = 0;
	};

	std::unique_ptr<IInterpreter> GetInterpeter(Config config_);
}

#endif