#include <fstream>
#include "interpreter/interpreter_interface.hpp"
#include "parser/parser_interface.hpp"
#include <Windows.h>
#include <iostream>

static constexpr auto kLexemsFlag = "--lexems";
static constexpr auto kFilename = "--filename";

std::initializer_list<const char*> flags
{
	kLexemsFlag,
	kFilename,
};

int main(int argc, const char** argv)
{
	stacklang::interpreter::Config config;
	std::string filename;
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], kLexemsFlag) == 0)
		{
			config.m_isPrintLexems = true;
		}
		else if (strcmp(argv[i], kFilename) == 0)
		{
			if (i + 1 <= argc)
			{
				filename = argv[i + 1];
				++i;
			}
		}
	}

	std::ifstream fstream(filename);
	if (!fstream.is_open())
	{
		std::cout << "ERROR: Can't open file: " << filename << '\n';
	}

	std::string code((std::istreambuf_iterator<char>(fstream)),
		std::istreambuf_iterator<char>());
	stacklang::interpreter::GetInterpeter(config)->Run(stacklang::parser::GetParser(code)->GetAST());
	return 0;
}