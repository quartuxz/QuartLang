#pragma once
#include <map>
#include <functional>
#include "Parser.h"
#include "DataStructure.h"
#include "builtinFunctions.h"
#include "programs.h"


//turns a stream of tokens into a program class
class Recognizer
{
private:

	Logger *m_logger;

	Program m_program;
	Parser* m_parser;


	size_t m_entryPoint;

	void m_makeProgram();
public:
	Recognizer(Parser* parser, Logger *logger);
	
	const Program *getProgram()const noexcept;
	Program *getProgram()noexcept;
};
