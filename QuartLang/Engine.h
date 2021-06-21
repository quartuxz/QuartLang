#pragma once
#include "Parser.h"
#include "Logger.h"
#include <exception>

 enum class runType {
	success
};


 enum class engineErrorType {
	tagNotFound,
 };

 extern std::map<engineErrorType, std::string> engineErrorTypeNames;

 class engineError : public std::exception {
 private:
	 engineErrorType m_errorType;
	 programContent m_errorOrigin;
	 std::string m_message;
	 std::vector<std::string> m_stackTrace;
	 std::string m_errorMessage;
 public:
	 engineError(engineErrorType errorType, const programContent& errorOrigin, const std::string &message, const std::vector<std::string>& stackTrace );
	 const char* what()const override;
 };


class Engine
{
private:
	Logger* m_logger;
	const Program* m_program;

	const DictionaryLexer* m_dict;
	
public:
	Engine(const Program *program, const DictionaryLexer *dict , Logger *logger);
	

	static void cleanUpThreads();

	runType run();
};

