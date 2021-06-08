#pragma once
#include "Parser.h"
#include "Logger.h"

 enum class runType {
	success
};


class Engine
{
private:
	Logger* m_logger;
	const Program* m_program;

	const DictionaryLexer* m_dict;
	
public:
	Engine(const Program *program, const DictionaryLexer *dict , Logger *logger);
	
	runType run();
};

