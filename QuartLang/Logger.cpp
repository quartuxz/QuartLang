#include "Logger.h"
#include <iostream>

void Logger::log(std::string origin, std::string contents)
{
	std::clog << origin << " : " << contents << std::endl;
}