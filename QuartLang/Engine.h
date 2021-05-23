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
	Program* m_program;

	runType m_run(const Subprogram* currentProgram, int startPoint = 0,DataStructure* p_result = nullptr, const std::map<std::string, DataStructure*>& p_variables = std::map<std::string, DataStructure*>());

public:
	Engine(Program *program, Logger *logger);
	
	runType run();
};

