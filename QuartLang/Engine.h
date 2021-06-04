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

	
public:
	Engine(const Program *program, Logger *logger);
	
	runType run();
};

