#pragma once
#include <string>
#include <map>
#define MUST_LOG

class Logger
{
private:
	std::map<std::string, std::string> m_logs;
public:
	void log(std::string origin, std::string contents);
};

