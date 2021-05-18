#pragma once
#include "Recognizer.h"
#include "Logger.h"

 enum class runType {
	success
};


class Engine
{
private:
	Logger* m_logger;
	Program* m_program;

	runType m_run(const Subprogram* currentProgram, DataStructure* p_result, const std::map<std::string, variableDeclaration*>& p_variables = std::map<std::string, variableDeclaration*>());

public:
	Engine(Program *program, Logger *logger);
	
	runType run();
};

