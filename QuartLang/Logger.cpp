#include "Logger.h"
#include <iostream>

void Logger::log(std::string origin, std::string contents)
{
	if (m_logging) {
		std::clog << origin << " : " << contents << std::endl;
	}

}

void Logger::toggleLogging(bool toggle)
{
	m_logging = toggle;
}
