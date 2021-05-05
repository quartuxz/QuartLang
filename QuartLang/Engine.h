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
public:
	Engine(Program *program, Logger *logger);
	
	runType run();
};

