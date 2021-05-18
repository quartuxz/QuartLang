#pragma once
#include <map>
#include <functional>
#include "Lexer.h"
#include "DataStructure.h"
#include "builtinFunctions.h"
#include "programs.h"


//turns a stream of tokens into a program class
class Parser
{
private:

	Logger *m_logger;

	Program m_program;
	Lexer* m_lexer;


	size_t m_entryPoint;

	void m_makeProgram();
public:
	Parser(Lexer* Lexer, Logger *logger);
	
	const Program *getProgram()const noexcept;
	Program *getProgram()noexcept;
};
