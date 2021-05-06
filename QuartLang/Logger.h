#pragma once
#include <string>
#include <map>
#define MUST_LOG_ISHLENG

class Logger
{
private:
	std::map<std::string, std::string> m_logs;
	bool m_logging = true;
public:
	void log(std::string origin, std::string contents);
	void toggleLogging(bool toggle);
};

#ifdef MUST_LOG_ISHLENG
#define LOG_ISHLENG(instance, origin, contents) instance.log(origin, contents)
#else
#define LOG_ISHLENG(instance,origin,contents)
#endif
