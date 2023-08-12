#ifndef STACKLANG_INTERPRETER_HPP
#define STACKLANG_INTERPRETER_HPP

#include <memory>
#include <map>
#include <string>
#include <stack>
#include "interpreter/config.hpp"
#include "interpreter/interpreter_interface.hpp"
#include "parser/ast_nodes.hpp"

namespace stacklang::interpreter
{
	using namespace parser;

	class Interpreter : public IInterpreter
	{
		struct Var
		{
			using value_type = unsigned long;
			value_type m_data;
		};
	public:
		Interpreter(Config config_);

		void Run(const std::vector<std::unique_ptr<BaseAstNode>>& ast_) override;
	private:
		void handlePush(const std::unique_ptr<BaseAstNode>& pushNode_);
		void handlePop(const std::unique_ptr<BaseAstNode>& popNode_);
		void handleRead(const std::unique_ptr<BaseAstNode>& readNode_);
		void handleWrite(const std::unique_ptr<BaseAstNode>& writeNode_);

		void handleDiv();
		void handleMultiply();
		void handleMod();
		void handlePlus();
		void handleMinus();

		void handleLess();
		void handleLessEqual();
		void handleGreater();
		void handleGreaterEqual();
		void handleEqual();
		void handleNotEqual();

		typename Var::value_type getValFromStack();

		std::size_t handleJi(const std::unique_ptr<BaseAstNode>& writeNode_, const std::vector<std::unique_ptr<BaseAstNode>>& ast_, std::size_t currentIdx_);
		std::size_t handleJmp(const std::unique_ptr<BaseAstNode>& writeNode_, const std::vector<std::unique_ptr<BaseAstNode>>& ast_);


	private:
		Config m_config;
		std::map<std::string, Var> m_tableNames;
		std::stack<Var> m_stack;
	};
}
#endif