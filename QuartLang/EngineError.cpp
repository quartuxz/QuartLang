#include "EngineError.h"
#include <sstream>

std::map<engineErrorType, std::string> engineErrorTypeNames = {
    {engineErrorType::tagNotFound, "tag not found"},
    {engineErrorType::functionNotFound, "function not found"},
    {engineErrorType::builtinError, "builtin function error"},

};



EngineError::EngineError(engineErrorType errorType, const programContent& errorOrigin, const std::string& message, const std::vector<std::string>& stackTrace) :
    m_errorType(errorType),
    m_errorOrigin(errorOrigin),
    m_message(message),
    m_stackTrace(stackTrace)
{

    std::stringstream stackTraceMessage;

    for (auto x : m_stackTrace)
    {
        stackTraceMessage << "->" << x;
    }

    std::stringstream ss;
    ss << "ENGINE ERROR!: \n\t" << "|error type: " << engineErrorTypeNames[m_errorType] << ", " << m_errorOrigin.getString() << ", stack trace: " << stackTraceMessage.str() << ", message: (" << m_message << ")|";
    m_errorMessage = ss.str();
}

const char* EngineError::what() const
{
    return m_errorMessage.c_str();
}