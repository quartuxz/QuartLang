#pragma once
#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include "ProgramContent.h"

enum class engineErrorType {
	tagNotFound, functionNotFound, builtinError
};

extern std::map<engineErrorType, std::string> engineErrorTypeNames;

class EngineError : public std::exception {
private:
	engineErrorType m_errorType;
	programContent m_errorOrigin;
	std::string m_message;
	std::vector<std::string> m_stackTrace;
	std::string m_errorMessage;
public:
	EngineError(engineErrorType errorType, const programContent& errorOrigin, const std::string& message, const std::vector<std::string>& stackTrace);
	const char* what()const override;
};
